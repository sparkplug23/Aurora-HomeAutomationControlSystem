/*
  mHeating_Commands.cpp - Home Heating Controller

  Copyright (C) 2021  Michael

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "mHVAC.h"

#ifdef USE_MODULE_CONTROLLER_HVAC_REMOTE

#ifdef USE_MODULE_NETWORK_WEBSERVER

int8_t mHVAC::Tasker_Web(uint8_t function){

  switch(function){    
    case TASK_WEB_COMMAND:
      //WebCommand_Parse();
    break;  
    case TASK_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;    
    case TASK_WEB_ADD_ROOT_MODULE_TABLE_CONTAINER:
      WebAppend_Root_Draw_Table();
    break;
    case TASK_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table();
    break;
    case TASK_WEB_APPEND_ROOT_BUTTONS:
      WebAppend_Root_ControlUI();
    break;
  }
  return 0;

}


/**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
***********ANYTHING ABOVE, MUST BE FROM 2020***************************************************   Huge rewrite
**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
*/

const char kButtonTitle_Timers_Upstairs[] PROGMEM = "OFF|1hr|1.5hr|2hr";
const char kButtonTitle_Timers_Downstairs[] PROGMEM = "OFF|1hr|1.5hr|2hr";
const char kButtonTitle_Timers_Immersion[] PROGMEM = "OFF|30m|1hr|2hr";
const char kButtonTitle_Timers_Boiler[] PROGMEM = "OFF|30m|1hr|2hr";

// const uint8_t kButtonTitle_Timers_Upstairs_Value[] PROGMEM = {0,60,90,120};
// const uint8_t kButtonTitle_Timers_Downstairs_Value[] PROGMEM = {0,60,90,120};
// const uint8_t kButtonTitle_Timers_Immersion_Value[] PROGMEM = {0,30,60,120};
// const uint8_t kButtonTitle_Timers_Boiler_Value[] PROGMEM = {0,30,60,120};

const char kButtonTitle_Temps_Upstairs[]    PROGMEM = "OFF|20&deg;C|23&deg;C|25&deg;C";
const char kButtonTitle_Temps_Downstairs[]  PROGMEM = "OFF|20&deg;C|23&deg;C|25&deg;C";
const char kButtonTitle_Temps_Immersion[]   PROGMEM = "OFF|30&deg;C|40&deg;C|50&deg;C";
const char kButtonTitle_Temps_Boiler[]      PROGMEM = "OFF|30&deg;C|40&deg;C|50&deg;C";



void mHVAC::WebAppend_Root_Draw_Table(){

  char buffer[50];

  // WebAppend_Draw_Table_FP(PSTR("prog_status_tab"),PSTR("%s Status"), array, arrlen);
  // WebAppend_Draw_Table_GetDevice_FP(PSTR("prog_status_tab"),PSTR("%s Status"), array, arrlen);
  
  // if(settings.fShowTable){
      BufferWriterI->Append_P(PSTR("{t}"));
    for(int ii=0;ii<4;ii++){
      BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
        BufferWriterI->Append_P(PSTR("<td>%s Status</td>"), GetDeviceNameLongbyIDCtr(ii,buffer, sizeof(buffer)));
        BufferWriterI->Append_P(PSTR("<td><span class='%s'>%s</span></td>"),"prog_status_tab","?");   
      BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
    }    
    
      BufferWriterI->Append_P(PSTR("{t2}"));
  // }

}


//append to internal buffer if any root messages table
void mHVAC::WebAppend_Root_Status_Table(){
  
  // if(settings.fShowTable){

  JBI->Array_Start("prog_status_tab");// Class name 
    for(int device_id=0;device_id<4;device_id++){      
      JBI->Object_Start();
        JBI->Add("id",device_id);
        JBI->Add("ih",heating_status[device_id].message_ctr);  
      JBI->Object_End();
    }
  JBI->Array_End();

  //need nested for loops to cover all values


//   //need to add memory for what was set, ie button 1 hr needs to be remembered, not just T-60mins and counting
  
  
int8_t kButtonTitle_Timers_Upstairs_Value[] = {-1,60,90,120};
int8_t kButtonTitle_Timers_Downstairs_Value[] = {-1,60,90,120};
int8_t kButtonTitle_Timers_Immersion_Value[] = {-1,30,60,120};
int8_t kButtonTitle_Timers_Boiler_Value[] = {-1,30,60,120};

int8_t kButtonTitle_Temps_Upstairs_Value[] = {-1,20,23,25};
int8_t kButtonTitle_Temps_Downstairs_Value[] = {-1,20,23,25};
int8_t kButtonTitle_Temps_Immersion_Value[] = {-1,30,40,50};
int8_t kButtonTitle_Temps_Boiler_Value[] = {-1,30,40,50};

// int8_t kButtonTitle_Timers_Upstairs_Value[] = {-1,2,3,4};//0,60,90,120};
// int8_t kButtonTitle_Timers_Downstairs_Value[] = {5,6,7,8};//{0,60,90,120};
// int8_t kButtonTitle_Timers_Immersion_Value[] = {10,11,12,13};//{0,30,60,120};
// int8_t kButtonTitle_Timers_Boiler_Value[] = {14,15,16,17};//}{0,30,60,120};



  uint8_t button_Counter = 0;
    char buffer[20];
    JBI->Array_Start(WEB_HANDLE_BUTTON_NAME_TIMER_SET);// Class name
    
  for(uint8_t device_id=0;device_id<4;device_id++){

    int8_t* _kButtonTitle = nullptr;
    switch(device_id){
      default:
      case DEVICE_US_ID: _kButtonTitle = kButtonTitle_Timers_Upstairs_Value;   break;
      case DEVICE_DS_ID: _kButtonTitle = kButtonTitle_Timers_Downstairs_Value; break;
      case DEVICE_IH_ID: _kButtonTitle = kButtonTitle_Timers_Immersion_Value;  break;
      case DEVICE_WB_ID: _kButtonTitle = kButtonTitle_Timers_Boiler_Value;     break;
    }

    for(int row=0;row<4;row++){  
      JBI->Object_Start();
        JBI->Add("id",button_Counter++);        
        if(program_timers[device_id].time_minutes_on_start == _kButtonTitle[row]){
          sprintf(buffer, "#00ff00");
          // sprintf(buffer, "MATCH%d==%d %d",program_timers[row].time_minutes_on_start,_kButtonTitle[row], row, button_Counter);
        }else{
          sprintf(buffer, "#4d8d90");
          // sprintf(buffer, "%d==%d %d",program_timers[row].time_minutes_on_start,_kButtonTitle[row], row, button_Counter);
        }
        // AddLog(LOG_LEVEL_DEV_TEST,PSTR("mins start %d == button[%d]%d"),program_timers[row].time_minutes_on_start,row,_kButtonTitle[row]);
        JBI->Add("bc",buffer);//program_timers[row].time_minutes_on_start ? "#00ff00" : "#ee2200" );
      JBI->Object_End();
    }

  }

    JBI->Array_End();
  /*

  
  for(uint8_t device_id=0;device_id<4;device_id++){

    const char* button_timers_title = nullptr;
    const char* button_temps_title = nullptr;

    switch(device_id){
      case DEVICE_US_ID:
        button_timers_title = kButtonTitle_Timers_Upstairs;
        button_temps_title = kButtonTitle_Temps_Upstairs;
      break;
      case DEVICE_DS_ID:
        button_timers_title = kButtonTitle_Timers_Downstairs;
        button_temps_title = kButtonTitle_Temps_Downstairs;
      break;
      case DEVICE_IH_ID:
      default:
        button_timers_title = kButtonTitle_Timers_Immersion;
        button_temps_title = kButtonTitle_Temps_Immersion;
      break;
      case DEVICE_WB_ID:
        button_timers_title = kButtonTitle_Timers_Boiler;
        button_temps_title = kButtonTitle_Temps_Boiler;
      break;
    }
   
    BufferWriterI->Append_P("{t}<tr>%s Programs</tr>{t2}",GetDeviceNameLongbyIDCtr(device_id, buffer));

    // Timers
    BufferWriterI->Append_P("{t}<tr>");
    for(uint8_t button_id=0;button_id<4;button_id++){
      char stemp[20];
      sprintf(button_handle_ctr,"%s%d\0",WEB_HANDLE_BUTTON_NAME_TIMER_SET,device_id);
      BufferWriterI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_VARIABLE2_HANDLE_IHR, 
                                    100/4, 
                                    WEB_HANDLE_BUTTON_NAME_TIMER_SET,
                                    button_handle_ctr, 
                                    program_timers[device_id].mapped_defaults[button_id],                                       
                                    pCONT_sup->GetTextIndexed_P(stemp, sizeof(stemp), button_id, button_timers_title),""
                                  );
    }
    BufferWriterI->Append_P("</tr>");

    */

  // JBI->Array_Start(WEB_HANDLE_BUTTON_NAME_TEMP_SET);// Class name
  // for(int row=0;row<4;row++){  
  //   JBI->Object_Start();
  //     JBI->Add("id",row);
  //     JBI->Add("bc",program_timers[row].time_minutes_on ? "#00ff00" : "#ee2200" );
  //   JBI->Object_End();
  // }
  // JBI->Array_End();

  
  // uint8_t 
  button_Counter = 0;
    // char buffer[20];
    JBI->Array_Start(WEB_HANDLE_BUTTON_NAME_TEMP_SET);// Class name
    
  for(uint8_t device_id=0;device_id<4;device_id++){

    int8_t* _kButtonTitle = nullptr;
    switch(device_id){
      default:
      case DEVICE_US_ID: _kButtonTitle = kButtonTitle_Temps_Upstairs_Value;   break;
      case DEVICE_DS_ID: _kButtonTitle = kButtonTitle_Temps_Downstairs_Value; break;
      case DEVICE_IH_ID: _kButtonTitle = kButtonTitle_Temps_Immersion_Value;  break;
      case DEVICE_WB_ID: _kButtonTitle = kButtonTitle_Temps_Boiler_Value;     break;
    }

    for(int row=0;row<4;row++){  
      JBI->Object_Start();
        JBI->Add("id",button_Counter++);  
        
        //if not running, its off
        if(program_temps[device_id].time_running.on||program_temps[device_id].time_maintaining.on){

          if(program_temps[device_id].temp.desired == _kButtonTitle[row]){
            sprintf(buffer, "#00ff00");
            // sprintf(buffer, "MATCH%d==%d %d",program_temps[row].time_minutes_on_start,_kButtonTitle[row], row, button_Counter);
          }else{
            sprintf(buffer, "#4d8d90");
            // sprintf(buffer, "%d==%d %d",program_temps[row].time_minutes_on_start,_kButtonTitle[row], row, button_Counter);
          }
           //AddLog(LOG_LEVEL_DEV_TEST,PSTR("mins start %d == button[%d]%d"),program_temps[row].temp.desired,row,_kButtonTitle[row]);
        }else{
          
            sprintf(buffer, "#4d8d90");
        }

       
        JBI->Add("bc",buffer);//program_temps[row].time_minutes_on_start ? "#00ff00" : "#ee2200" );
      JBI->Object_End();
    }

  }

    JBI->Array_End();
  
}


// void mHVAC::WebAppend_Root_Draw_Program_Buttons(){
  void mHVAC::WebAppend_Root_ControlUI(){//}
  char listheading[30];
  // BufferWriterI->Append_P("\"%s\":[{\"ihr\":\"","heat_program_buttons_draw");
  // BufferWriterI->Append_P("%s",PSTR("{t}"));
  // BufferWriterI->Append_P(PSTR("<tr><td> Liveview</td></tr>"));//GetPaletteFriendlyName(),GetNumberOfColoursInPalette(static_palettes.ptr));
  // BufferWriterI->Append_P(PSTR("<tr><td><div class='heat_program_buttons'style='width:100%%;height:%dpx';></div></td></tr>"),
  //                                                     1); //pixel height option for liveview

char buffer[30];
                                               
  char button_handle_ctr[20]; 
  memset(button_handle_ctr,0,sizeof(button_handle_ctr));
    
  for(uint8_t device_id=0;device_id<4;device_id++){

    const char* button_timers_title = nullptr;
    const char* button_temps_title = nullptr;

    switch(device_id){
      case DEVICE_US_ID:
        button_timers_title = kButtonTitle_Timers_Upstairs;
        button_temps_title = kButtonTitle_Temps_Upstairs;
      break;
      case DEVICE_DS_ID:
        button_timers_title = kButtonTitle_Timers_Downstairs;
        button_temps_title = kButtonTitle_Temps_Downstairs;
      break;
      case DEVICE_IH_ID:
      default:
        button_timers_title = kButtonTitle_Timers_Immersion;
        button_temps_title = kButtonTitle_Temps_Immersion;
      break;
      case DEVICE_WB_ID:
        button_timers_title = kButtonTitle_Timers_Boiler;
        button_temps_title = kButtonTitle_Temps_Boiler;
      break;
    }
   
    BufferWriterI->Append_P("{t}<tr>%s Programs</tr>{t2}",GetDeviceNameLongbyIDCtr(device_id, buffer, sizeof(buffer)));

    // Timers
    BufferWriterI->Append_P("{t}<tr>");
    for(uint8_t button_id=0;button_id<4;button_id++){
      char stemp[20];
      sprintf(button_handle_ctr,"%s%d\0",WEB_HANDLE_BUTTON_NAME_TIMER_SET,device_id);
      BufferWriterI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_VARIABLE2_HANDLE_IHR, 
                                    100/4, 
                                    "buttonh " WEB_HANDLE_BUTTON_NAME_TIMER_SET,
                                    button_handle_ctr, 
                                    program_timers[device_id].mapped_defaults[button_id],                                       
                                    pCONT_sup->GetTextIndexed_P(stemp, sizeof(stemp), button_id, button_timers_title),""
                                  );
    }
    BufferWriterI->Append_P("</tr>");

    // TEMPS
    if((device_id==DEVICE_WB_ID)||(device_id==DEVICE_IH_ID)){
      BufferWriterI->Append_P("<tr>");
      for(uint8_t button_id=0;button_id<4;button_id++){
        char stemp[20];
        sprintf(button_handle_ctr,"%s%d\0",WEB_HANDLE_BUTTON_NAME_TEMP_SET, device_id);
        BufferWriterI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_VARIABLE2_HANDLE_IHR, 
                                      100/4, 
                                      "buttonh " WEB_HANDLE_BUTTON_NAME_TEMP_SET,
                                      button_handle_ctr, 
                                      program_temps[device_id].temp.mapped_defaults[button_id],                                       
                                      pCONT_sup->GetTextIndexed_P(stemp, sizeof(stemp), button_id, button_temps_title),""
                                    );
      }
      BufferWriterI->Append_P("</tr>{t2}");
    }
  }

}





void mHVAC::WebPage_Root_AddHandlers(){
  
  /**
   * Root Page 
   * */
  // pCONT_web->pWebServer->on("/" "main" "/root_draw.json", HTTP_GET, [this](AsyncWebServerRequest *request){
  //   Web_Root_Draw(request);
  // })

}



// void mHVAC::WebCommand_Parse(void)
// {
  
//   if(pCONT_web->request_web_command == nullptr){ return; }

//   char tmp[100];

// //  selectorlist.amount = 0;
//   uint8_t  arg_value = 0;

//   char arg_ctr[30]; memset(arg_ctr,0,sizeof(arg_ctr));

//   // // check palette selector
//   // sprintf(arg_ctr,"relay0_toggle\0");
//   // if (pCONT_web->pWebServer->hasParam("relay0_toggle")) {
//   //   pCONT_web->WebGetArg(arg_ctr, tmp, sizeof(tmp));
//   //   arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//   //   // animation.palette = arg_value;

//   //   ExecuteCommandPower(0,POWER_TOGGLE,SRC_MQTT);

//   //   // update_all = true; //refresh all
//     ALOG_TST(PSTR(D_LOG_RELAYS "hasParam test"));//,arg_ctr,arg_value);
//   // }

//   for(uint8_t relay_id=0;relay_id<4;relay_id++){
    
//     sprintf(arg_ctr,"%s%d",WEB_HANDLE_BUTTON_NAME_TIMER_SET,relay_id);

//     if (pCONT_web->request_web_command->hasParam(arg_ctr)) {
//       pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//       arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//       // animation.brightness = BrtN2F(arg_value);
//       ALOG_TST(PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %d"),arg_ctr,arg_value,relay_id);
//       // ALOG_TST(PSTR(D_LOG_NEO "animation.brightness=%d"),arg_value);
//       // SetRefreshLEDs();

    
//     functionhandler_programs_timers.flags.run_now = true;
//       // program_timers[relay_id].mapped_defaults[]
//       program_timers[relay_id].time_minutes_on = arg_value;
//     program_timers[relay_id].time_minutes_on_start = program_timers[relay_id].time_minutes_on;

//       // ExecuteCommandPower(relay_id,POWER_TOGGLE,SRC_MQTT);
//     }

//   }



//   for(uint8_t relay_id=0;relay_id<4;relay_id++){
    
//     sprintf(arg_ctr,"%s%d",WEB_HANDLE_BUTTON_NAME_TEMP_SET,relay_id);

//     if (pCONT_web->request_web_command->hasParam(arg_ctr)) {
//       pCONT_web->WebGetArg(pCONT_web->request_web_command, arg_ctr, tmp, sizeof(tmp));
//       arg_value = (!strlen(tmp)) ? 0 : atoi(tmp);
//       // animation.brightness = BrtN2F(arg_value);
//       ALOG_TST(PSTR(D_LOG_NEO "hasParam(\"%s\")=%d %d"),arg_ctr,arg_value,relay_id);
//       // ALOG_TST(PSTR(D_LOG_NEO "animation.brightness=%d"),arg_value);
//       // SetRefreshLEDs();

    
//       // program_timers[relay_id].mapped_defaults[]
//       program_temps[relay_id].temp.desired = (float)arg_value;

//       // ExecuteCommandPower(relay_id,POWER_TOGGLE,SRC_MQTT);
//     }

//   }

// }

#endif // USE_MODULE_NETWORK_WEBSERVER


#endif