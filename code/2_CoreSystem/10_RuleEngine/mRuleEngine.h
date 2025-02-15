

#ifndef _MRULEENGINE_H
#define _MRULEENGINE_H

#define D_UNIQUE_MODULE_CORE_RULES_FRIENDLY_ID 2010 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CORE_RULES

//, public mEvent

#include <stdint.h>
#include "3_Network/10_MQTT/mMQTT.h"

#include "1_TaskerManager/mTaskerInterface.h"

class mRuleEngine :
  public mTaskerInterface
{
        
  public:
    #define D_MAX_RULES 5
    
    static constexpr const char* PM_MODULE_CORE_RULES_CTR = D_MODULE_CORE_RULES_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_RULES_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_RULES_FRIENDLY_ID; }


    struct SETTINGS{

      bool loaded_default_for_moduled = false;
    }settings;  

    uint8_t* data = nullptr;
    uint16_t _dataLen = 0;
    uint16_t _dataUsedLen = 0;

    #define D_COMMAND_BUFFER_LENGTH 255
    struct JSONCOMMANDS{
      char data[D_COMMAND_BUFFER_LENGTH] = {0}; //use | delims
      uint8_t bytes_used = 0;
      int8_t delims_used = 0;
    }jsonbuffer;

    void ShowRuleAddLogByIndex(uint8_t show_type = 0); // 0 = basic indexed, 1 = with names
    void ShowRuleEvent_AddLog(uint8_t show_type = 0);

    bool AppendEventToRules(mEvent::EVENT_PART* trigger, mEvent::EVENT_PART* command);

    void RulesLoad_From_Progmem();
    void AppendRule_FromDefault_UsingName(const char* name);

    // need a group of functions to "add" and remove from the buffer,
    // what about buffers? they can includes ids


    // mRuleEngine(uint16_t _data_buffer_size){

    //   _dataLen = _data_buffer_size;
    //   bool result = allocateData(_data_buffer_size);

    // };
    mRuleEngine(){};

    // ~mRuleEngine(){
    //   deallocateData();
    // };

    uint8_t GetConfiguredCount();
    uint8_t GetEnabledCount();

    // bool allocateData(uint16_t len){
    //   // Serial.println("allocateData");
    //   if (data && _dataLen == len) return true; //already allocated
    //   deallocateData();
    //   data = new (std::nothrow) byte[len];
    //   if (!data) return false; //allocation failed
    //   _dataLen = len;
    //   for(int i=0;i<_dataLen;i++){data[i] = 0;}
    //   _dataUsedLen = 0;
    //   //   memset(data, 0, len*sizeof(TYPE));
    //   return true;
    // }

    // void deallocateData(){
    //   delete[] data;
    //   data = nullptr;
    //   _dataLen = 0;
    // };

    // // External function to get instance
    // static RuleEngine* GetInstance();
    // /* Here will be the instance stored. */
    // static RuleEngine* instance;

    // Allocate data buffer using pointers
    void AllocateEventBuffer(uint8_t* p_to_buffer, uint8_t bytes_to_add){

      // uint8_t index_start = _dataUsedLen + 1;
      p_to_buffer = &data[_dataUsedLen];
      _dataUsedLen += bytes_to_add;

    }
    
    /**
     * creating a few templates, can be done in comment only, then generated at runtime within scope
     * // Relay 
     * struct RELAY{
     *  uint8_t index;
     *  uint8_t state;
     *  uint8_t index;
     * }
     * */


    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    void parse_JSONCommand(JsonParserObject obj);
 
    enum RULE_DATA_ENCODING_IDS{
        ENCODING_BYTES_ID,
        RULE_DATA_ENCODING_8B8B16B_BYTES_ID,
        RULE_DATA_ENCODING_16BIT_ID,
        RULE_DATA_ENCODING_LENGTH_ID
    };
    
uint8_t rule_count2 = 0;
    
    // /***
    //  * 
    //  * Future change:
    //  * Events should become their own class, to hold things that happen.
    //  * Rules can then only be turned on when needed in the primary loop, equally, I should add a check to only go through rules if any are active with a flag
    //  * 
    //  * 
    //  * */
    // struct EVENT_PART{
    //   /**
    //    * Buttons, Relays
    //    * */
    //   uint16_t module_id;
    //   /**
    //    * InputChange, Timer
    //    */
    //   uint16_t function_id;
    //   /**
    //    * ?? or buffer[0]
    //    * */
    //   uint8_t device_id;
    //   /**
    //    * Values
    //    * */
    //   struct VALUES{
    //     uint8_t encoding = 0;
    //     uint8_t data[5];
    //     uint8_t length = 0;
    //   }value;
    //   /**
    //    * jsoncommands stored in a dlist
    //    * */
    //   int8_t json_commands_dlist_id = -1;
    // };

    struct RULES{
      bool flag_enabled = false;
      bool flag_configured = false;
      mEvent::EVENT_PART trigger;
      mEvent::EVENT_PART command; //introduce multiple commands? Create a vector of rules?
    }rules[D_MAX_RULES];

    // This will need to become an array or queue, that way consecutive rules can trigger at the same time
    mEvent::EVENT_PART event_triggered;


    uint8_t rules_active_index = 0;



    //move into class
    // void Reset(){
    //   Reset(&EventTriggered);
    // }
    void Reset(mEvent::EVENT_PART* e){
      e->module_id = 0;
      e->function_id = 0;
      // e->index = 0;
      // e->state = 0;
    };
    void Add_All(mEvent::EVENT_PART* e, uint16_t _module_id=0, uint8_t _index=0, uint8_t _state=0){
      e->module_id = _module_id;
      // e->function_id = _function_id;
      // e->index = _index;
      // e->state = _state;
    };
    void Encoding(mEvent::EVENT_PART* e, uint8_t encoding){
      e->value.encoding = encoding;
    };

    // If no event is passed, assume its the triggered_event
    void Reset(){
      event_triggered.module_id = 0;
      event_triggered.function_id = 0;
      // event_triggered.index = 0;
      // event_triggered.state = 0;
    };
    // Maybe Addlog_p method for share "arg" list
    void Add_All(uint16_t _module_id=0, uint8_t _index=0, uint8_t _state=0){
      event_triggered.module_id = _module_id;
      event_triggered.device_id = _index;
      event_triggered.value.data[0] = _state;
    };
    void NewEvent(uint16_t _module_id=0, uint8_t _index=0, uint8_t _state=0){
      Reset();
      event_triggered.module_id = _module_id;
      event_triggered.device_id = _index;
      event_triggered.value.data[0] = _state;
    };
    void NewEventRun(uint16_t _module_id=0, uint16_t function_event = 0, uint8_t _index=0, uint8_t _state=0); // needs to be in cpp
    bool NewEventRun_NumArg(uint16_t _module_id=0, uint16_t function_event=0, uint8_t _index=0, uint8_t _data_length=0, ...); 


    void Encoding( uint8_t encoding){
      event_triggered.value.encoding = encoding;
    };
    //create arg version
    void New_Event(uint16_t _module_id=0, uint8_t _device_id=0, uint8_t data1=0, uint8_t data2=0){
      Reset();
      event_triggered.module_id = _module_id;
      event_triggered.device_id = _device_id;
      event_triggered.value.data[0] = data1;
      event_triggered.value.data[1] = data2;
      event_triggered.value.encoding = 0; //bytes
      event_triggered.value.length = 1; //bytes


    }

    void DefaultRuleForModule();

    #ifdef USE_MODULE_TEMPLATE_SONOFF_4CHPRO
    void DefaultRule_Sonoff_4CHPRO();
    #endif // USE_MODULE_TEMPLATE_SONOFF_4CHPRO
    #ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
    void DefaultRule_Sonoff_iFan03();
    #endif // USE_MODULE_TEMPLATE_SONOFF_IFAN03
    #ifdef USE_MODULE_TEMPLATE_SHELLY_DIMMER2
    void DefaultRule_Shelly_Dimmer2();
    #endif // USE_MODULE_TEMPLATE_SHELLY_DIMMER2
    #ifdef USE_MODULE_TEMPLATE_SHELLY_2P5
    void DefaultRule_Shelly_2p5();
    #endif // USE_MODULE_TEMPLATE_SHELLY_2P5

    
    void parsesub_Rule_Part(JsonParserObject jobj, mEvent::EVENT_PART* event);

    
    bool Tasker_Rules_Interface(uint16_t function);
    // void Tasker_Rules_Init();

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);


  #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    std::vector<struct handler<mRuleEngine>*> mqtthandler_list;
    struct handler<mRuleEngine> mqtthandler_settings;
    struct handler<mRuleEngine> mqtthandler_state_ifchanged;
  #endif //  USE_MODULE_NETWORK_MQTT

};

// #define mRuleEngineI RuleEngine::GetInstance()
// #define REI RuleEngineI



#endif

#endif //ifdef USE_MODULE_CORE_RULES