
#include "mTankVolume.h"

#ifdef USE_MODULE_CONTROLLER_TANKVOLUME
    #ifdef USE_MODULE_NETWORK_WEBSERVER




// void mTankVolume::WebAppend_Root_Draw_Table(){

//   //WebAppend_Root_Draw_RGBLive();
//   // if(animation.mode_id == ANIMATION_MODE_EFFECTS_ID){
//   //   WebAppend_Root_Draw_RGBPalette();
//   // // }
//   // WebAppend_Root_Draw_PaletteSelect_Placeholder();
//   tkr_web->WebAppend_Root_Draw_Table_dList(8,"oil_table", kTitle_TableTitles_Root);
  
// }


// //append to internal buffer if any root messages table
// void mTankVolume::WebAppend_Root_Status_Table(){

//   char buffer[50];
  
//   JBI->Array_Start("oil_table");// Class name
//   for(int row=0;row<8;row++){
//     JBI->Object_Start();
//       JBI->Add("id",row);
//       switch(row){
//         default:
//         case 0: JBI->Add("ih","GetAnimationStatusCtr(buffer)"); break;
//         // case 1:
//         //   JBI->Add_FV("ih",PSTR("\"%d/%d (secs)\""), 
//         //     animation.cycle_time__rate_ms/1000, 
//         //     animation.time_ms.val/1000
//         //   );
//         // break;
//         // case 2: 
//         //   JBI->Add_FV("ih",PSTR("\"%d%% [#%d]\""),
//         //     animation.transition.pixels_to_update_as_percentage.val, 
//         //     GetPixelsToUpdateAsNumberFromPercentage(animation.transition.pixels_to_update_as_percentage.val)
//         //   );
//         // break;
//         // case 3: JBI->Add("ih",GetTransitionOrderName(buffer)); break;
//         // case 4: JBI->Add("ih",GetAnimationModeName(buffer, sizeof(buffer)));   break;
//         // case 5: JBI->Add("ih",GetFlasherFunctionName(buffer)); break;
//         // case 6: JBI->Add_FV("ih",PSTR("\"%d (%s)\""), (int)power_rating.power,"W"); break;
//         // case 7:
//         //   if(!animation.auto_time_off_secs){ //off
//         //     JBI->Add("ih","Unset");
//         //   }else{
//         //     JBI->Add_FV("ih",PSTR("\"%d (%s)\""),
//         //       animation.auto_time_off_secs,"secs"
//         //     );
//         //   }
//         // break;
//       } //switch
    
//     JBI->Object_End();
//   }
//   JBI->Array_End();
  

//   // char colour_button[8];
//   // char button_ids[] = {ANIMATION_MODE_EFFECTS_ID, ANIMATION_MODE_SCENE_ID, 
//   // #ifdef ENABLE_FEATURE_PIXEL__MODE_NOTIFICATION
//   // ANIMATION_MODE_NOTIFICATIONS_ID, 
//   // #endif
//   // ANIMATION_MODE_NONE_ID};
  
//   // JBI->Array_Start("animod");// Class name
//   // for(int row=0;row<sizeof(button_ids);row++){
//   //   if(button_ids[row] == animation.mode_id){
//   //     sprintf(colour_button,"%s\0",COLOR_BUTTON_HIGHLIGHT); //selected
//   //   }else{
//   //     sprintf(colour_button,"%s\0",COLOR_BUTTON); //NOT selected
//   //   }        
//   //   JBI->Object_Start();
//   //     JBI->Add("id",row);
//   //     JBI->Add("bc",colour_button);
//   //   JBI->Object_End();
//   // }  
//   // JBI->Array_End();

// }


// void mTankVolume::WebAppend_Root_ControlUI(){

//   // char buffer[50];

//   // BufferWriterI->Append_P(HTTP_MSG_SLIDER_TITLE_JUSTIFIED,PSTR("Animation Mode Select"),"");

//   // uint8_t animation_mode_list_ids[] = {
//   //   ANIMATION_MODE_EFFECTS_ID, ANIMATION_MODE_SCENE_ID, 
//   //   #ifdef ENABLE_FEATURE_PIXEL__MODE_NOTIFICATION
//   //   ANIMATION_MODE_NOTIFICATIONS_ID,
//   //   #endif    
//   //   ANIMATION_MODE_NONE_ID
//   // };

//   // BufferWriterI->Append_P(PSTR("{t}<tr>"));
//   // for(uint8_t button_id=0;button_id<sizeof(animation_mode_list_ids);button_id++){
//   //   BufferWriterI->Append_P(HTTP_DEVICE_CONTROL_BUTTON_JSON_VARIABLE_INSERTS_HANDLE_IHR2,
//   //                             100/sizeof(animation_mode_list_ids),
//   //                             "", //no span
//   //                             "animod" " " "buttonh",
//   //                             D_ANIMATIONMODE, 
//   //                             GetAnimationModeNameByID(animation_mode_list_ids[button_id], buffer, sizeof(buffer)),
//   //                             GetAnimationModeNameByID(animation_mode_list_ids[button_id], buffer, sizeof(buffer)),
//   //                             ""
//   //                           );                   
//   // }
//   // BufferWriterI->Append_P("%s",PSTR("</tr>{t2}"));
  
// }



// void mTankVolume::WebPage_Root_AddHandlers(){

//   /**
//    * Pages
//    * */


//   /**
//    * Styles
//    * */

//   // tkr_web->pWebServer->on(WEB_HANDLER_STYLE_TOPBAR, HTTP_GET, [this](AsyncWebServerRequest *request){ 
//   //   Handle_Style_TopBar(request);
//   // });


//   /**
//    * Javascripts (code)
//    * */

//   // pWebServer->on(WEB_HANDLER_SCRIPT_WEB_DATA_FETCHER, HTTP_GET, [this](AsyncWebServerRequest *request){ 
//   //   Serial.println(WEB_HANDLER_SCRIPT_WEB_DATA_FETCHER); Serial.flush();      
//   //   request->send_P(200,"text/javascript",SCRIPT_WEB_DATA_FETCHER);
//   // });

//   // Root page listeners
//   // tkr_web->pWebServer->on(WEB_HANDLE_JSON_LIVEPIXELS, [this](AsyncWebServerRequest *request){
//   //   WebSend_JSON_RootPage_LiveviewPixels(request); 
//   // });
//   // tkr_web->pWebServer->on("/fetch/rgb_palette.json", [this](AsyncWebServerRequest *request){  //works
//   //   Serial.println("/fetch/rgb/palette.json"); Serial.flush();
//   //   WebSend_JSON_RootPage_Palette_AJ(request); 
//   // });

  
//   /**
//    * JSON data refresh
//    * */

//   // tkr_web->pWebServer->on(WEB_HANDLE_JSON_OIL_SENSOR_TABLE, HTTP_GET, [this](AsyncWebServerRequest *request){ 
//   //   WebSend_JSON_Table(request);    
//   // });

//   // pWebServer->on(WEB_HANDLER_SCRIPT_WEB_DATA_FETCHER, HTTP_GET, [this](AsyncWebServerRequest *request){ 
//   //   Serial.println(WEB_HANDLER_SCRIPT_WEB_DATA_FETCHER); Serial.flush();      
//   //   request->send_P(200,"text/javascript",SCRIPT_WEB_DATA_FETCHER);
//   // });




// }


// /**
//  * Send only the needed message (where another will call all modules to group send)
//  * */
// void mTankVolume::WebSend_JSON_Table(AsyncWebServerRequest *request){
    
//   if(tkr_web->RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
//   uint16_t freemem_start = ESP.getFreeHeap();  
//   DynamicJsonDocument doc(400);  

//   JsonObject root = doc.to<JsonObject>();
  
//   ConstructRoot_JSON_Table(root);
  
//   memset(data_buffer.payload.ctr,0,sizeof(data_buffer.payload.ctr));
//   serializeJson(doc,data_buffer.payload.ctr);
//   request->send(200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);
    
//   uint16_t freemem_used = freemem_start - ESP.getFreeHeap();
//   AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_ASYNC WEB_HANLDE_JSON_WEB_TOP_BAR " Ef::%s=%d"),"freemem_used",freemem_used);
//   //freemem_usage.WebSend_JSON_WebServer_TopBar = freemem_used;

// } // end function


// void mTankVolume::ConstructRoot_JSON_Table(JsonObject root){

// 	JsonArray classname_obj = root.createNestedArray("tab_oil");

//   for(int row=0;row<1;row++){
//     switch(row){
//       case 0:{        
//         char table_row[25]; memset(table_row,0,sizeof(table_row));        
//         // sprintf(table_row,"%s&deg;%c",climate.ptr->temperature_ctr,pCONT_sup->TempUnit());
        
//         char float_ctr[10];
//         memset(float_ctr,0,sizeof(float_ctr));
        
//       #ifdef USE_MODULE_SENSORS_DS18X
//         float height = 120-pCONT_ult->GetDistanceCMReading();
//         #else
//         float height = 120-0;


//         #endif
//         dtostrf(height,4,2,float_ctr);
//         sprintf(table_row,"%s (cm)",float_ctr);

//         JsonObject element_obj = classname_obj.createNestedObject();
//           element_obj["id"] = row;
//           element_obj["ih"] = table_row;
//           // Make function that takes range_limit_array and returns colours assigned to them eg colour = GetColourForRange(range_array,current_number)
//           // if(climate.ptr->temperature<=25){
//           //   element_obj["fc"] = "#00ff00"; //create variable/use webcolour ids
//           // }else
//           // if(climate.ptr->temperature>25){
//           //   element_obj["fc"] = "#fcba03";
//           // }else{
//           //   element_obj["fc"] = "#0000003";
//           // }
//       }break;
//       // case 1:{        
//       //   char table_row[25]; memset(table_row,0,sizeof(table_row));       
//       //   sprintf(table_row,"%s",motion_detect.detected_rtc_ctr);
//       //   JsonObject element_obj = classname_obj.createNestedObject();
//       //     element_obj["id"] = row;
//       //     element_obj["ih"] = table_row;
//       //     // if(climate.ptr->humidity>75){
//       //     //   element_obj["fc"] = "#ff0000"; //create variable/use webcolour ids
//       //     // }else
//       //     // {
//       //     //   element_obj["fc"] = "#0ff000";
//       //     // }
//       // }break;
//     } //end switch 
//   } // end for

// } // end function





    #endif // USE_MODULE_NETWORK_WEBSERVER

#endif