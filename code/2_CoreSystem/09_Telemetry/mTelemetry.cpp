/**
 * @file    mTelemetry.cpp
 * @author  Michael Doone (michaeldoonehub@gmail.com)
 * @brief   Generate JSON of coresystem values
 * @version 1.0
 * @date    2022-04-20
 * 
 * @copyright Copyright (c) 2022
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
#include "mTelemetry.h"

int8_t mTelemetry::Tasker(uint8_t function, JsonParserObject obj)
{
  

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:
      if (serial_messages_remaining_to_send > 0) {
        serial_messages_remaining_to_send--;
        auto handle = mqtthandler_list[serial_messages_remaining_to_send];
        uint8_t fSendPayload = CALL_MEMBER_FUNCTION(*this, handle->ConstructJSON_function)(handle->json_level, true);\
        ALOG_INF(PSTR(D_LOG_TELEMETRY ">>>>>>>%d/%d%S<<<<<<<<\n\r%s"), 
          mqtthandler_list.size()-serial_messages_remaining_to_send,
          mqtthandler_list.size(),
          mqtthandler_list[serial_messages_remaining_to_send]->postfix_topic,
          JBI->GetBuffer());
      }    
    break;
    case TASK_UPTIME_30_SECONDS:
      serial_messages_remaining_to_send = mqtthandler_list.size();
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      pCONT_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      pCONT_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      pCONT_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif //USE_MODULE_NETWORK_MQTT
    /************
     * WEB SECTION * 
    *******************/   
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_ADD_HANDLER:    
      WebPage_Root_AddHandlers();
    break;
    #endif // USE_MODULE_NETWORK_WEBSERVER
  }

}


#ifdef USE_MODULE_NETWORK_WEBSERVER
void mTelemetry::WebPage_Root_AddHandlers()
{
  #ifdef ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL
  #ifdef USE_MODULE_NETWORK_MQTT
  CODE_BLOCK__MQTTHandler_AddWebURL_PayloadRequests();
  #endif
  #endif
}
#endif //  #ifdef USE_MODULE_NETWORK_WEBSERVER


#ifdef USE_MODULE_NETWORK_MQTT

void mTelemetry::MQTTHandler_Init()
{

  handler<mTelemetry>* ptr;
  
  ptr = &mqtthandler_lwt_online;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = 3600; // Hourly 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_LWT_ONLINE_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_LWT_ONLINE_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_LWT_Online;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_health;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  #if defined(ENABLE_DEVFEATURE_DEBUG_SLOW_LOOPS) || defined(ENABLE_DEVFEATURE_DEBUG_POINT_EVERY_SECOND_HEALTH_PACKETS)
  ptr->tRateSecs = 1; 
  #else
  ptr->tRateSecs = DEFAULT_MQTT_SYSTEM_MINIMAL_RATE_SECS; 
  #endif // ENABLE_DEVFEATURE_DEBUG_SLOW_LOOPS
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_REDUCE_AFTER_1_MINUTES_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_HEALTH_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Health;
  mqtthandler_list.push_back(ptr);

  #ifndef FIRMWARE_MINIMAL2
  
  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_log;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_LOG_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Log;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_firmware;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_FIRMWARE_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Firmware;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_memory;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_MEMORY_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Memory;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_network;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_NETWORK_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Network;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_mqtt;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_MQTT_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_MQTT;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_time;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_TIME_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Time;
  mqtthandler_list.push_back(ptr);
    
  ptr = &mqtthandler_reboot;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_REBOOT_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Reboot;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_reboot_event;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR;  
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_ALL;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_REBOOT_EVENT_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Reboot;
  mqtthandler_list.push_back(ptr);

  // #ifdef ENABLE_MQTT_DEBUG_TELEMETRY
  
  ptr = &mqtthandler_devices;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_DEVICES_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_Devices;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_debug_pins;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_PINS_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_Pins;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_debug_template;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_TEMPLATE_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_Template;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_debug_moduleinterface;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_MODULETEMPLATE_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_ModuleInterface;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_debug_system_saved_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_SYSTEM_STORED_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_System_Stored_Settings;
  mqtthandler_list.push_back(ptr);

  #ifdef ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  ptr = &mqtthandler_debug_tasker_interface_performance; 
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = 60; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_TASKER_INTERFACE_PERFORMANCE;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_Tasker_Interface_Performance;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES

  #ifdef ENABLE_DEVFEATURE__SETTINGS_STORAGE__SEND_DEBUG_MQTT_MESSAGES
  ptr = &mqtthandler_debug__settings_storage;
  ptr->handler_id = MQTT_HANDLER_SYSTEM_DEBUG_SETTINGS_STORAGE;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.retain = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_SETTINGS_STORAGE_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug__Settings_Storage;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_DEVFEATURE__SETTINGS_STORAGE__SEND_DEBUG_MQTT_MESSAGES

  // #endif // ENABLE_MQTT_DEBUG_TELEMETRY
  
  ptr = &mqtthandler_debug_minimal;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = DEFAULT_MQTT_SYSTEM_MINIMAL_RATE_SECS;
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_MODULEMINIMAL_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_Minimal;
  mqtthandler_list.push_back(ptr);

  #endif // FIRMWARE_MINIMAL2

} 

#endif // USE_MODULE_NETWORK_MQTT








