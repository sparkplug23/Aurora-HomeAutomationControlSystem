/**
 * @file mFileSystem.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-07
 * 
 * There are two types of filesystems:
 * 
 * 1) Internal flash filesystem
 * 2) SD Card filesystem
 * https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html
 * @copyright Copyright (c) 2023
 * 
 * Step 1: build now and be able to view files on the SD card in the webui editor
 * 
 */

#include "mFileSystem.h"

#ifdef USE_MODULE_CORE_FILESYSTEM

/**
 * @brief 
 * 
 * Three files are needed:
 * "/config_system.json" -- to be moved to flash later
 * "/config_modules.json"
 * "/config_secure.json"
 * 
 * 
 */


//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

void mFileSystem::JsonFile_Save__Stored_Module()
{
  
  ALOG_INF( PSTR("JsonFile_Save__Stored_Module") );

  const char* file_path = "/config_module.json";
  char buffer[100] = {0};

  File file;  
  // Open file for writing, if it does not exist, create it
  // Seek is placed at the start of the file, contents will be overwriten
  file = FILE_SYSTEM.open(file_path, "w+");
  
  if(!file) 
  {
    ALOG_ERR(PSTR("Failed to open \"%s\""), file_path);
    return;
  }

  if(!JBI->RequestLock(GetModuleUniqueID())){
    return;
  }
 
  JBI->Start();
    JBI->Add(PM_UTC_TIME, tkr_time->GetDateAndTime(DT_UTC).c_str());
    JBI->Add(PSTR("millis"), millis());
    // pCONT->Tasker_Interface(TASK_FILESYSTEM_APPEND__CONFIG_MODULES__ID);
  JBI->End();

  file.print(JBI->GetBufferPtr());
  file.close();
    
  ALOG_INF(PSTR("Writing file \"%s\""), JBI->GetBufferPtr());

  JBI->ReleaseLock();

}

/**
 * @brief Test if file exists, if not, load default template from progmem if it exists
 * 
 */
void mFileSystem::JsonFile_Load__Stored_Module_Or_Default_Template()
{

  DEBUG_LINE_HERE;

  bool force_default_template = false; // ie on reset
 
  if(!JsonFile_Load__Stored_Module() || force_default_template){
    ALOG_INF(PSTR("No config_module.json file found, loading default template from progmem"));
    pCONT->Tasker_Interface(TASK_TEMPLATE_MODULE_LOAD_AFTER_INIT_DEFAULT_CONFIG_ID);
  }

  DEBUG_LINE_HERE;

}


bool mFileSystem::JsonFile_Load__Stored_Module()
{
  
  ALOG_INF( PSTR("JsonFile_Load__Stored_Module") );

  File file;  
  const char* file_path = "/config_module.json";
  
  // Open file for read only
  file = FILE_SYSTEM.open(file_path, "r");
  
  if (!file) {
    ALOG_ERR(PSTR("Failed to open \"%s\""), file_path);
    return false;
  }

  Serial.printf("Read from file [%d] \n\r", file.available());

  if(!JBI->RequestLock(GetModuleUniqueID())){
    return false;
  }
 
  // Read into local buffer, this should be locked from async access using buffer class method
  D_DATA_BUFFER_CLEAR();
  uint8_t* buffer_p = (uint8_t*)data_buffer.payload.ctr;
  file.read(buffer_p, file.available());
  file.close();
  
  data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);

  ALOG_INF( PSTR(DEBUG_INSERT_PAGE_BREAK "Loaded file = \"%d|%s\""),data_buffer.payload.length_used, data_buffer.payload.ctr);

  pCONT->Tasker_Interface(TASK_JSON_COMMAND_ID);

  JBI->ReleaseLock();

  return true;
  
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

void mFileSystem::JsonFile_Save__Stored_Secure()
{
  
  ALOG_INF( PSTR("JsonFile_Save__Stored_Secure") );

  const char* file_path = "/config_secure.json";
  char buffer[100] = {0};

  File file;  
  // Open file for writing, if it does not exist, create it
  // Seek is placed at the start of the file, contents will be overwriten
  file = FILE_SYSTEM.open(file_path, "w+");
  
  if(!file) 
  {
    ALOG_ERR(PSTR("Failed to open \"%s\""), file_path);
    return;
  }

  if(!JBI->RequestLock(GetModuleUniqueID())){
    return;
  }
 
  JBI->Start();
    JBI->Add(PM_UTC_TIME, tkr_time->GetDateAndTime(DT_UTC).c_str() );
    JBI->Add(PSTR("millis"), millis());
    pCONT->Tasker_Interface(TASK_FILESYSTEM_APPEND__Stored_Secure__ID);
  JBI->End();

  file.print(JBI->GetBufferPtr());
  file.close();
    
  ALOG_INF(PSTR("Writing file \"%s\""), JBI->GetBufferPtr());

  JBI->ReleaseLock();

}

void mFileSystem::JsonFile_Load__Stored_Secure()
{
  
  ALOG_INF( PSTR("JsonFile_Load__Stored_Secure") );

  File file;  
  const char* file_path = "/config_secure.json";
  
  // Open file for read only
  file = FILE_SYSTEM.open(file_path, "r");
  
  if (!file) {
    ALOG_ERR(PSTR("Failed to open \"%s\""), file_path);
    return;
  }

  Serial.printf("Read from file [%d]: \n\r", file.available());

  if(!JBI->RequestLock(GetModuleUniqueID())){
    return;
  }
 
  // Read into local buffer, this should be locked from async access using buffer class method
  D_DATA_BUFFER_CLEAR();
  uint8_t* buffer_p = (uint8_t*)data_buffer.payload.ctr;
  file.read(buffer_p, file.available());
  file.close();
  
  data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);

  ALOG_INF( PSTR("Loaded file = \"%d|%s\""),data_buffer.payload.length_used, data_buffer.payload.ctr);

  pCONT->Tasker_Interface(TASK_JSON_COMMAND_ID);

  JBI->ReleaseLock();
  
}




//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//// Shared functions: For all modules to pass its file name and bytes to save, or byte to load and the position to write into
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

void mFileSystem::ByteFile_Save(char* filename_With_extension, uint8_t* buffer, uint16_t buflen) // where to write the data from
{
  
  ALOG_INF( PSTR("ByteFile_Save") );

  File file;  
  // Open file for writing, if it does not exist, create it
  // Seek is placed at the start of the file, contents will be overwriten
  file = FILE_SYSTEM.open(filename_With_extension, "w+");
  
  if(!file) 
  {
    ALOG_ERR(PSTR("Failed to open \"%s\""), filename_With_extension);
    return;
  }

  file.write((const uint8_t*)buffer, buflen);
  file.close();
    
  ALOG_INF(PSTR("Writing file (%s) \"%s\""), filename_With_extension, buffer);

}

uint32_t mFileSystem::ByteFile_Load(char* filename_With_extension, uint8_t* buffer, uint16_t buflen) // where to write the data into
{
  
  ALOG_INF( PSTR("ByteFile_Load") );

  File file;  
  // Open file for writing, if it does not exist, create it
  // Seek is placed at the start of the file, contents will be overwriten
  file = FILE_SYSTEM.open(filename_With_extension, "r");

  if(!file) 
  {
    ALOG_ERR(PSTR("Failed to open \"%s\""), filename_With_extension);
    return 0;
  }


  uint32_t filesize = file.size();

  ALOG_INF(PSTR("Reading file \"%s\" %d bytes (expected %d bytes)"), filename_With_extension, filesize, buflen);

  if(filesize != buflen){
    ALOG_ERR(PSTR("File size mismatch, expected %d bytes, got %d bytes"), buflen, filesize);
    file.close();
    return 0;
  }else{
    ALOG_ERR(PSTR("File file.read(buffer, filesize)"));
    file.read(buffer, filesize);
  }

  file.close();
//must return the file size, note that if the read does not match the expected (ie file mismatch in internal memory), then the file is corrupted and must be reset to default
  return 0;
  
}



void mFileSystem::JSONFile_Save(char* filename_With_extension, char* buffer, uint16_t buflen) // where to write the data from
{
  
  ALOG_INF( PSTR("JSONFile_Save") );

  File file;  
  // Open file for writing, if it does not exist, create it
  // Seek is placed at the start of the file, contents will be overwriten
  file = FILE_SYSTEM.open(filename_With_extension, "w+");
  
  if(!file) 
  {
    ALOG_ERR(PSTR("Failed to open \"%s\""), filename_With_extension);
    return;
  }

  file.write((const uint8_t*)buffer, buflen);
  file.close();
    
  ALOG_INF(PSTR("Writing file (%s) \"%s\""), filename_With_extension, buffer);

}


void mFileSystem::JSONFile_Load(char* filename_With_extension, char* buffer, uint16_t buflen) // where to write the data into
{

  ALOG_INF( PSTR("JSONFile_Load") );

}

#endif // USE_MODULE_CORE_FILESYSTEM
