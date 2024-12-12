/**
 * @file mAnimatorLight_Mixer.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-13
 * 
 * Sequencer will likely be replaced by playlists
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

#ifdef ENABLE_FEATURE_LIGHTING__SEQUENCER 


void mAnimatorLight::Init_Sequencer()
{

  Load_Sequencer(100); // Load default sequences

  // Start with sequencer off, lights off, so a reset does not turn things on again without my permission

  #ifdef ENABLE_DEVFEATURE_SEQUENCER__ENABLE_TIME_RESTRAINTS
  sequencer_runtime.Enable_TimeRestraints = true;
  #endif



} // Init_Sequencer


void mAnimatorLight::Load_Sequencer(uint8_t id)
{

  sequencer_item_list.clear(); // clear all sequences
  memset(&sequencer_runtime, 0, sizeof(sequencer_runtime)); // clear all sequences

  struct SEQUENCER_ITEM seq_t;

  /**
   * @brief Unload all sequences
   * 
   */
  if(id == 0)
  {
    sequencer_runtime.loaded_sequence_set = 0;
    mqtthandler_automation_sequencer.flags.SendNow = true;
    ALOG_INF(PSTR("Load_Sequencer(%d) Unload, disabled"), id);
    return;
  }

  #ifdef ENABLE_FEATURE_SEQUENCER__LOAD_DEVICE_LIST
  SubLoad_Sequencer_Device(id);
  #endif 

  // sequencer_runtime.loaded_sequence_set = id;

}

void mAnimatorLight::SetSequenceTimes(uint16_t secs)
{  
  for (auto &seq_t:sequencer_item_list)
  {
    seq_t.seconds_on = secs;
  }  
}


void mAnimatorLight::SubTask_Sequencer()
{

  uint8_t flag_reset = false;

  if(sequencer_runtime.loaded_sequence_set == 0)
  {
    // Not active, do nothing
    return;
  }

  
  if( mTime::TimeReachedNonReset(&sequencer_runtime.tSaved_Tick, 1000) ) // NonReset, so if sequence is not found, then it will retry the next index
  {

    if(sequencer_item_list.size() > 0)
    {

      if(sequencer_runtime.seconds_remaining_on_item >= 1) // still active
      {
        sequencer_runtime.seconds_remaining_on_item--;
        sequencer_runtime.tSaved_Tick = millis();
        ALOG_DBM(PSTR("sequencer_runtime.seconds_remaining_on_item = %d"), sequencer_runtime.seconds_remaining_on_item);
      
        size_t prev_index = (sequencer_runtime.active_sequence_index + sequencer_item_list.size() - 1) % sequencer_item_list.size();

        ALOG_INF(PSTR("Seq: %d/%d %ds \n\r                        %s"), 
            prev_index, sequencer_item_list.size(),
            sequencer_runtime.seconds_remaining_on_item,
            sequencer_item_list[prev_index].description.c_str()
        );
     
      
      }else 
      if(sequencer_runtime.seconds_remaining_on_item == 0) // still active
      {      
        
        struct SEQUENCER_ITEM sequencer_item_t = sequencer_item_list[sequencer_runtime.active_sequence_index];

        #ifdef ENABLE_DEVFEATURE_SEQUENCER__ENABLE_TIME_RESTRAINTS
        /**
         * @brief If the current preset sequence is not within the time range, then skip it (return early and rely on this function to be called again on next loop)
         * 
         */
        if(sequencer_runtime.Enable_TimeRestraints)
        {
          if( !pCONT_time->IsShortTimeWithinRange(sequencer_item_t.time_enabled.start, sequencer_item_t.time_enabled.end) && sequencer_item_t.time_enabled.isArmed)
          {
            ALOG_INF(PSTR("IsShortTimeWithinRange SKIPPED, not within time range"));
            
            sequencer_runtime.active_sequence_index++; if(sequencer_runtime.active_sequence_index >= sequencer_item_list.size()){ sequencer_runtime.active_sequence_index = 0; } // move to next sequence

            sequencer_runtime.tSaved_Tick = millis(); // rate limit to 1 second

            return;
          }
        }


        #endif // ENABLE_DEVFEATURE_SEQUENCER__ENABLE_TIME_RESTRAINTS
        #ifdef sequencer_runtime.remote_openhab_limit_flashing

        if(sequencer_runtime.remote_openhab_limit_flashing)
        {
          if(sequencer_item_t.time_enabled.isArmed)
          {

          }
        }
        
        #endif

        ALOG_INF(PSTR("sequencer_runtime.seconds_remaining_on_item FINISHED, Trigger and move on"));
        ALOG_INF(PSTR("Json = \"%s\""), sequencer_item_t.json_command);
        pCONT->JSONCommand_Run((char*)sequencer_item_t.json_command.c_str());

        sequencer_runtime.seconds_remaining_on_item = sequencer_item_t.seconds_on; // Set timeon for THIS effect

        sequencer_runtime.active_sequence_index++; // move to next sequence
        sequencer_runtime.tSaved_Tick = millis();

        // When sequence loaded, reset the update time
        // or rather, if animation is not finished, force it to finish before json executes on effect function command

        /**
         * @brief Load new
         **/
        if(sequencer_runtime.active_sequence_index >= sequencer_item_list.size())
        {
          sequencer_runtime.active_sequence_index = 0;
        }

        mqtthandler_automation_sequencer.flags.SendNow = true;
      }


      // ALOG_INF(PSTR("Json = \"%s\""), sequencer_item_t.json_command);
      sequencer_runtime.tSaved_Tick = millis();
      
    }


  }


} // SubTask_Sequencer


uint8_t mAnimatorLight::ConstructJSON_Sequencer(uint8_t json_level, bool json_appending)
{

  char buffer[100] = {0};

  JBI->Start();

  JBI->Add("LimitLevel", sequencer_runtime.remote_openhab_limit_flashing);

    JBI->Add("Index", sequencer_runtime.active_sequence_index);
    JBI->Add("ListCount", sequencer_item_list.size());
    uint32_t total_seconds = 0;
    for(auto seq_t:sequencer_item_list)
    {
      total_seconds += seq_t.seconds_on;
    }
    JBI->Add("TotalSeconds", total_seconds);

    JBI->Array_Start(PSTR("SequenceList"));
    for(auto seq_t:sequencer_item_list)
    {
      JBI->Object_Start();

        JBI->Add("SecondsOn", seq_t.seconds_on);
        JBI->Add("Description", seq_t.description.c_str());

        seq_t.time_enabled.start = mTime::MakeTimeShort(16,30,1);
        seq_t.time_enabled.end   = mTime::MakeTimeShort(23,31,1);

        
        JBI->Object_Start_F(PSTR("EnabledTimes"));

        JBI->Add("Start", mTime::GetTimeStrFromTimeShort(seq_t.time_enabled.start).c_str() );
        JBI->Add("End", mTime::GetTimeStrFromTimeShort(seq_t.time_enabled.end).c_str() );

      
        JBI->Object_End();

        // JBI->Add("Json", seq_t.json_command);

      JBI->Object_End();
      
    }
    JBI->Array_End();
 

  return JBI->End();

} // ConstructJSON_Sequencer


#endif // ENABLE_FEATURE_LIGHTING__SEQUENCER

#endif //USE_MODULE_LIGHTS_ANIMATOR
