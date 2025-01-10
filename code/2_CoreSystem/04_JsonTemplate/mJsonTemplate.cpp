#include "mJsonTemplate.h"

#ifdef USE_MODULE_CORE__JSON_TEMPLATE

int8_t mJsonTemplate::Tasker(uint8_t function, JsonParserObject obj)
{

  if(function == TASK_TEMPLATES__LOAD_MODULE){
    Template_Load();
  }

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      // Pre_Init();
    break;
    #ifdef ENABLE_DEVFEATURE__FILESYSTEM__LOAD_HARDCODED_TEMPLATES_INTO_FILESYSTEM
    case TASK_TEMPLATES__MOVE_HARDCODED_TEMPLATES_INTO_FILESYSTEM:
      Templates__SaveHardcodedTemplateToFilesystem();
    break;
    #endif
  }

  // if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  // switch(function){
  //   /************
  //    * PERIODIC SECTION * 
  //   *******************/
  //   case TASK_EVERY_SECOND:  
  //   break;
  //    case TASK_LOOP:
  //   //   UfsExecuteCommandFileLoop();
  //   break;
  //   case TASK_EVERY_FIVE_SECOND:    
  //     #ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS
  //     SystemTask__Execute_Module_Data_Save();
  //     #endif // ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS
  //   break;
  //   case TASK_EVERY_MINUTE:
  //     // #ifdef ENABLE_DEVFEATURE__SAVE_MODULE_DATA // This will in the future only occur once an hour, or before planned boot
  //     #ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_MINUTE
  //     SystemTask__Execute_Module_Data_Save();
  //     #endif // ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_MINUTE
  //     // #endif     
  //   break;  
  //   case TASK_EVERY_FIVE_MINUTE:
  //     #ifdef ENABLE_SYSTEM_SETTINGS_IN_I2C
  //     JsonFile_Save__Stored_Module();
  //     JsonFile_Save__Stored_Secure();
  //     #endif // ENABLE_SYSTEM_SETTINGS_IN_I2C
       
  //   break;
  //   /************
  //    * COMMANDS SECTION * 
  //   *******************/
  //   case TASK_JSON_COMMAND_ID:
  //     parse_JSONCommand(obj);
  //   break;
  //   /************
  //    * MQTT SECTION * 
  //   *******************/
  //   #ifdef USE_MODULE_NETWORK_MQTT
  //   case TASK_MQTT_HANDLERS_INIT:
  //     MQTTHandler_Init();
  //   break;
  //   case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
  //     MQTTHandler_Rate();
  //   break;
  //   case TASK_MQTT_SENDER:
  //     MQTTHandler_Sender();
  //   break;
  //   case TASK_MQTT_CONNECTED:
  //     MQTTHandler_RefreshAll();
  //   break;
  //   #endif //USE_MODULE_NETWORK_MQTT
    // }  

} // END Tasker


/**
 * @brief Boot order
 * ** from filesystem
 * ** any hardcoded templates
 * ** none, load as default device
 * 
 */
void mJsonTemplate::Template_Load()
{


    ReadModuleTemplateFromProgmem();
    // ParseModuleTemplate(); //probably goes elsewhere


}


/**
 * @brief Function reads templates from progmem if available, then calls TemplateParser
 * @note JsonParser needs to have the buffer on the stack so it can be edited (token replacing), so must be read from progmem
 * @return bool if there was a template in progmem
 * */
bool mJsonTemplate::ReadModuleTemplateFromProgmem(){

DEBUG_LINE_HERE2

  #ifdef USE_MODULE_TEMPLATE
    uint16_t progmem_size = sizeof(MODULE_TEMPLATE);
    progmem_size = progmem_size>MODULE_TEMPLATE_MAX_SIZE?MODULE_TEMPLATE_MAX_SIZE:progmem_size;
    // create parse buffer
    char buffer[progmem_size];
    // Read into local
DEBUG_LINE_HERE2
    memcpy_P(buffer,MODULE_TEMPLATE,sizeof(MODULE_TEMPLATE));
    
DEBUG_LINE_HERE2
    tkr_set->runtime.template_loading.status.module = mSettings::TemplateSource::HEADER_TEMPLATE;

    #ifdef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
    #ifdef ENABLE_LOG_LEVEL_INFO
    ALOG_INF(PSTR("MODULE_TEMPLATE Load"));// = \"%s\""), buffer);
    ALOG_DBG(PSTR("Load = \"%s\""), buffer);
    #endif // ENABLE_LOG_LEVEL_INFO
    #endif // ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

    #ifdef ENABLE_DEBUG_POINT_MODULE_TEMPLATE_BOOT_SPLASH
    ALOG_INF( PSTR("mHardwarePins::ReadModuleTemplateFromProgmem [%d bytes] = \"%s\""), progmem_size, buffer);
    #endif // ENABLE_DEBUG_POINT_MODULE_TEMPLATE_BOOT_SPLASH

DEBUG_LINE_HERE2
    pCONT_pins->ModuleTemplate__ParseCJSONBuffer(buffer);
DEBUG_LINE_HERE2
    return true;
  #else 
    #error "USE_MODULE_TEMPLATE not defined"
    return false;
  #endif // MODULE_TEMPLATE
  
}



#ifdef ENABLE_DEVFEATURE__FILESYSTEM__LOAD_HARDCODED_TEMPLATES_INTO_FILESYSTEM
void mJsonTemplate::Templates__SaveHardcodedTemplateToFilesystem()
{
  ALOG_HGL(PSTR("Templates__SaveHardcodedTemplateToFilesystem"));

  File file; 
  char file_path[50] = {0};

  #ifdef USE_MODULE_TEMPLATE
    snprintf(file_path,sizeof(file_path),"/tmplt_module.json");
 
    // Open file for writing, if it does not exist, create it
    // Seek is placed at the start of the file, contents will be overwriten
    file = FILE_SYSTEM.open(file_path, "w+");
    
    if(file) 
    {
      ALOG_ERR(PSTR("Writing \"%s\""), file_path);
      file.print(MODULE_TEMPLATE);
      file.close();    
    }

  #endif // USE_MODULE_TEMPLATE
  #ifdef USE_FUNCTION_TEMPLATE
    snprintf(file_path,sizeof(file_path),"/tmplt_function.json");
 
    // Open file for writing, if it does not exist, create it
    // Seek is placed at the start of the file, contents will be overwriten
    file = FILE_SYSTEM.open(file_path, "w+");
    
    if(file) 
    {
      ALOG_ERR(PSTR("Writing \"%s\""), file_path);
      file.print(FUNCTION_TEMPLATE);
      file.close();    
    }

  #endif // USE_FUNCTION_TEMPLATE
  #ifdef USE_LIGHTING_TEMPLATE
    snprintf(file_path,sizeof(file_path),"/tmplt_lighting.json");
 
    // Open file for writing, if it does not exist, create it
    // Seek is placed at the start of the file, contents will be overwriten
    file = FILE_SYSTEM.open(file_path, "w+");
    
    if(file) 
    {
      ALOG_ERR(PSTR("Writing \"%s\""), file_path);
      file.print(LIGHTING_TEMPLATE);
      file.close();    
    }

  #endif // USE_LIGHTING_TEMPLATE
  #ifdef USE_RULES_TEMPLATE
    snprintf(file_path,sizeof(file_path),"/tmplt_rules.json");
 
    // Open file for writing, if it does not exist, create it
    // Seek is placed at the start of the file, contents will be overwriten
    file = FILE_SYSTEM.open(file_path, "w+");
    
    if(file) 
    {
      ALOG_ERR(PSTR("Writing \"%s\""), file_path);
      file.print(RULES_TEMPLATE);
      file.close();    
    }

  #endif // USE_RULES_TEMPLATE

}
#endif // ENABLE_DEVFEATURE__FILESYSTEM__LOAD_HARDCODED_TEMPLATES_INTO_FILESYSTEM




#endif // USE_MODULE_CORE_I2C
