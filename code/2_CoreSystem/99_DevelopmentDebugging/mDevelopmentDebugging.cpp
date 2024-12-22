/**
 * @file mDevelopmentDebugging.cpp
 * 
 * @author your name (you@domain.com)
 * @brief Special tasker that will be used to develop code, so I can remove it from other builds to move closer to a release version
 * 
 * 1) A way to print DEFINES that are being phased out
 * 2) Idea, configure another laptop (surface book) and make duplicate of project, run on HVAC and try get crash errors
 * 
 * @version 0.1
 * @date 2022-03-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "mDevelopmentDebugging.h"


int8_t mDevelopmentDebugging::Tasker(uint8_t function, JsonParserObject obj){


  switch(function){
    case TASK_INIT:
      //

      #ifdef ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
      // pCONT->monitor_task.push_back(TASK_LOOP);           // Add TASK_LOOP to the monitor list
      // pCONT->monitor_task.push_back(TASK_EVERY_SECOND);   // Add TASK_EVERY_SECOND to the monitor list
      pCONT->monitor_task.push_back(TASKER_FUNCTION_TYPES(0));   // Any
      #endif

    break;
  }

  switch(function){
    
    case TASK_LOOP: {
      
    }break;
    case TASK_EVERY_SECOND:{

      #ifdef ENABLE_DEBUG_SPLASH_SYSTEM_PERFORMANCE_METRICS_TO_SERIAL
        ALOG_INF( PSTR(PM_COMMAND_SVALUE_NVALUE), PM_LOOPSSEC, pCONT_sup->activity.cycles_per_sec);
        ALOG_INF( PSTR(PM_COMMAND_SVALUE_NVALUE), PM_FREEHEAP, ESP.getFreeHeap());
      #endif // ENABLE_DEBUG_SPLASH_SYSTEM_PERFORMANCE_METRICS_TO_SERIAL

      // ALOG_INF( PSTR("loops_per_second %d"), pCONT_sup->loops_per_second);
      // ALOG_INF( PSTR("this_cycle_ratio %d"), pCONT_sup->this_cycle_ratio);
      // ALOG_INF( PSTR("loop_load_avg %d"), pCONT_set->loop_load_avg);

      #ifdef ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
      for (const auto& metrics : pCONT->task_metrics) {
          // Get the module name based on the unique ID
          const char* module_name = pCONT->GetModuleName(metrics.unique_id);

          Serial.printf("Task: %d|%S, \tMax: %d us, \tMin: %d us, \tAvg: %d us\n\r", 
                        metrics.task_id, module_name, metrics.max_time, metrics.min_time, metrics.avg_time);
      }
      #endif
        
    }break;
    case TASK_EVERY_FIVE_SECOND:

    break;
    case TASK_EVERY_MINUTE:

      SubTask_Show_Defines_Ready_To_Phase_Out();

    break;
    case TASK_EVERY_MIDNIGHT:
    
    break;
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
  }

}//end


void mDevelopmentDebugging::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

	if(jtok = obj["DebugInput"].getObject()["Float1"])
	{
		debug_data.input_float1 = jtok.getFloat();
    char buffer[20];
    mSupport::float2CString(debug_data.input_float1, JSON_VARIABLE_FLOAT_PRECISION_LENGTH, buffer);    
    ALOG_INF(PSTR("DebugInput Float1: %s"), buffer);
	}

  #ifdef ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
	if(jtok = obj["Debug"].getObject()["ResetTaskMetrics"])
	{
    // Reset all task metrics
    for (auto& metrics : pCONT->task_metrics) {
        metrics.max_time = 0;        // Reset max time to 0
        metrics.min_time = UINT32_MAX; // Set min time to the highest possible value to ensure it gets updated correctly
        metrics.total_time = 0;      // Reset total time
        metrics.count = 0;           // Reset count
        metrics.avg_time = 0;        // Reset average time
    }
	}
  #endif // ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS



    
}

void mDevelopmentDebugging::SubTask_Show_Defines_Ready_To_Phase_Out()
{
  
  /**
   * @brief When a define has been stripped from code, but not templates, this will alert user to ensure new commands
   * are properly integrated into the newer code format.    * 
   */
  #ifdef ENABLE_DEVFEATURE_SET_ESP32_RGB_DATAPIN_BY_TEMPLATE
    #error "ENABLE_DEVFEATURE_SET_ESP32_RGB_DATAPIN_BY_TEMPLATE -- Can be removed"
  #endif 
  #ifdef PINSET_TEMP_METHOD_RGB_PIN_RGB
    #error "PINSET_TEMP_METHOD_RGB_PIN_RGB -- Can be removed"
  #endif 
  #ifdef ENABLE_DEVFEATURE_BUTTON_HANDLER_V2
  #error "ENABLE_DEVFEATURE_BUTTON_HANDLER_V2 - Needs to be removed"
  #endif
  #ifdef ENABLE_DEVFEATURE_BUTTON_MULTIPRESS
  #error "ENABLE_DEVFEATURE_BUTTON_MULTIPRESS - Needs to be removed"
  #endif
  #ifdef ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
  #error "ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS"
  #endif
  #ifdef ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
  #error "ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE"
  #endif
  #ifdef ENABLE_DEVFEATURE_MOVING_GETCOLOUR_AND_PALETTE_TO_RAM
  #error "ENABLE_DEVFEATURE_MOVING_GETCOLOUR_AND_PALETTE_TO_RAM"
  #endif
  #ifdef ENABLE_DEVFEATURE_REMOVE_BRIGHTNESS_RANDOMNESS_INSIDE_APPLY_BRIGHTNESS
  #error "ENABLE_DEVFEATURE_REMOVE_BRIGHTNESS_RANDOMNESS_INSIDE_APPLY_BRIGHTNESS
  #endif
  #ifdef ENABLE_DEVFEATURE_FIX_STRIPSIZE_LENGTH_ISSUE_XMAS2022
  #error "ENABLE_DEVFEATURE_FIX_STRIPSIZE_LENGTH_ISSUE_XMAS2022
  #endif
  #ifdef ENABLE_DEVFEATURE_REMOVE_SEG_STOP_INDEX
  #error "ENABLE_DEVFEATURE_REMOVE_SEG_STOP_INDEX
  #endif
  #ifdef ENABLE_DEVFEATURE_COLOUR_PALETTE_MERGED
  #error "ENABLE_DEVFEATURE_COLOUR_PALETTE_MERGED
  #endif
  #ifdef ENABLE_DEVFEATURE_CANSHOW_VERSION3
  #error "ENABLE_DEVFEATURE_CANSHOW_VERSION3
  #endif
  #ifdef ENABLE_DEVFEATURE_FORCED_REMOVE_091122
  #error "ENABLE_DEVFEATURE_FORCED_REMOVE_091122
  #endif
  #ifdef ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
  #error "ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
  #endif
  
  #ifdef ENABLE_DEVFEATURE_REMOVING_HSBID
  #error "ENABLE_DEVFEATURE_REMOVING_HSBID
  #endif
  #ifdef ENABLE_DEVFEATURE_PALETTE__VECTORED
  #error "ENABLE_DEVFEATURE_PALETTE__VECTORED
  #endif
  #ifdef ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
  #error "ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
  #endif
  #ifdef ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
  #error "ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
  #endif
  #ifdef ENABLE_DEVFEATURE__PIXEL_USE_I2S_FOR_BUS
  #error "ENABLE_DEVFEATURE__PIXEL_USE_I2S_FOR_BUS
  #endif
  #ifdef ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28
  #warning "ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28
  #endif
  #ifdef ENABLE_DEBUG_MULTIPIN
  #error "ENABLE_DEBUG_MULTIPIN
  #endif
  #ifdef DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
  #error "DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
  #endif

  #ifdef ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
  #error "ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT"
  #endif
  #ifdef ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
  #error "ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD"
  #endif
  #ifdef ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS
  #warning "ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS phase out"
  #endif
  #ifdef ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE
  #error "ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE"
  #endif
  #ifdef ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
  #error "ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN"
  #endif 
  #ifdef ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
  #error "ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS"
  #endif 

  /**
   * @brief Remove ASAP
   * 
   */
  #ifdef STRIP_SIZE_MAX
  #error "STRIP_SIZE_MAX is no longer used since multi-pin"
  #endif

  /*
  To be used sometimes
  */
  #ifdef ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS
  #warning "ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS should stay, but I dont want this enabled by default until motion bug is fixed"
  #endif
  #ifdef ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL
  #error "ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL"
  #endif

}
