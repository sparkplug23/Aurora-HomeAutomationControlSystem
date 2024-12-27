
#include "mDeviceNameList.h"


/* Null, because instance will be initialized on demand. */
DeviceNameList* DeviceNameList::instance = nullptr;

DeviceNameList* DeviceNameList::GetInstance(){
  if (instance == nullptr){
    instance = new DeviceNameList();
    // PRINT_FLUSHED("instance = new JsonBuilder()");
  }
  return instance;
}



//returns length
// Other variations needed:
//  - Overwrite existing devicename
//  - Remove by name OR by ID
int8_t DeviceNameList::AddDeviceName(const char* name_ctr, int16_t unique_module_id, int8_t device_id){

  char* buffer = name_buffer.ptr;
  uint16_t buffer_length = strlen(buffer);

  #ifdef ENABLE_LOG_LEVEL_INFO
  //AddLog(LOG_LEVEL_INFO,PSTR("mSettings::AddDeviceName len=%d"),buffer_length);
  #endif// ENABLE_LOG_LEVEL_INFO

  buffer_length = buffer_length;// > DEVICENAMEBUFFER_NAME_BUFFER_LENGTH ? DEVICENAMEBUFFER_NAME_BUFFER_LENGTH : buffer_length;
  
  #ifdef USE_LOG
  //AddLog(LOG_LEVEL_INFO,PSTR("name_bufferB=%s"), buffer);
  #endif


  // Check indexing
  uint8_t index = 0;
  if(buffer_length){ //if anything in buffer
    index = CountCharInCtr(buffer,'|'); // first | indicates index from 0
  }
  
  // Check not above "id" limits
  if(index >= (number_buffer.length-1)){// (DEVICENAMEBUFFER_NAME_INDEX_LENGTH-1)){
    return 0; // too many devices in list
  }

  uint16_t new_buffer_length = buffer_length+strlen(name_ctr);

    #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("AddDeviceName(%s,%d,%d) %d index=%d"), 
  //   name_ctr, 
  //   unique_module_id, 
  //   device_id, 
  //   buffer_length, 
  //   index
  // );
    #endif// ENABLE_LOG_LEVEL_INFO
  //AddLog(LOG_LEVEL_INFO,PSTR("name_bufferB=%s"), buffer);

  // Write name to next slot
  if(new_buffer_length<name_buffer.length){
    buffer_length+=sprintf(buffer+buffer_length, "%s|", name_ctr); 
    #ifdef ENABLE_LOG_LEVEL_INFO
    // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("AddDeviceName ADDED + \"%s\""),name_ctr); 
    #endif // ENABLE_LOG_LEVEL_INFO
  }

  // Add index to next slot
  // number_buffer.unique_group_ids[index] = unique_module_id;
  // number_buffer.index_ids[index] = device_id;

  number_buffer.unique_group_ids[index] = unique_module_id;
  number_buffer.index_ids[index] = device_id;

  number_buffer.currently_used_indexed++;

    #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
  AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("name_bufferE=%s"), buffer);
    #endif // ENABLE_LOG_LEVEL_COMMANDS

    #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog_Array(LOG_LEVEL_DEBUG_MORE,PSTR("unique_module_id"),number_buffer.unique_group_ids,(int16_t)DEVICENAMEBUFFER_NAME_INDEX_LENGTH);
  // AddLog_Array(LOG_LEVEL_DEBUG_MORE,PSTR("device_id"),number_buffer.index_ids,(int8_t)DEVICENAMEBUFFER_NAME_INDEX_LENGTH);
    #endif// ENABLE_LOG_LEVEL_INFO

}

int8_t DeviceNameList::RemoveDeviceName(const char* name_ctr, int16_t unique_module_id, int8_t device_id){
  // Find name, shift results to remove found name
  // Not needed until I want to remove values
}


// const char* DeviceNameList::GetDeviceNameWithUniqueNumber(int16_t module_id, int8_t device_id, char* buffer, uint16_t buffer_size){

// }

// const char* DeviceNameList::GetDeviceNameWithEnumNumber(int16_t module_id, int8_t device_id, char* buffer, uint16_t buffer_size, bool flag_respond_nomatch_if_not_found){

// // DEBUG_LINE_HERE;

//       ALOG_INF( PSTR("\t\t\t\t\n\r PHASEOUT direct use, PROBABLY WRONG ------------------------------------- DeviceNameList::GetDeviceNameWithEnumNumber buffer_size %d"),buffer_size);
//   //convert enum number to unqiue#pragma region 

//   module_id = pCONT->GetModuleUniqueIDbyVectorIndex(module_id);

// // DEBUG_LINE_HERE;
//   int8_t found_index = -1;
//   // Check if class & id match
//   for(int i=0;i<DEVICENAMEBUFFER_NAME_INDEX_LENGTH;i++){
    
// // DEBUG_LINE_HERE;
//     // if((number_buffer.unique_group_ids[i]==module_id)&&(number_buffer.index_ids[i]==device_id)){
    
//     if((    
//       number_buffer.unique_group_ids[i]==module_id)&&(number_buffer.index_ids[i]==device_id)){
//       found_index = i;
      
// // DEBUG_LINE_HERE;
//       // ALOG_INF( PSTR("DeviceNameList::GetDeviceNameWithEnumNumber found_index %d"),i);
      
//       break;
//     }
//   }
// // DEBUG_LINE_HERE;
//   //future, if none found, have a list of the prefered defaults, relay%d, sensor%d etc

//   if(found_index == -1)
//   {
// // DEBUG_LINE_HERE;
//     if(flag_respond_nomatch_if_not_found)
//     {
//       memcpy(buffer,PM_SEARCH_NOMATCH,sizeof(PM_SEARCH_NOMATCH));
//       #ifdef ENABLE_LOG_LEVEL_INFO
//       ALOG_TST(PSTR("F::%s >> %s"),__FUNCTION__,PM_SEARCH_NOMATCH);
//       #endif // ENABLE_LOG_LEVEL_INFO
    

//     }
//     else
//     {  

// // DEBUG_LINE_HERE;
// // I will need to add an increasing index here with its module name, but important to check how many of that index exist. For now, simply "-1"

//       snprintf(buffer, buffer_size, "%S_%02d", pCONT->GetModuleName(module_id), device_id);

//       #ifdef ENABLE_LOG_LEVEL_INFO
//       ALOG_DBM( PSTR("F::%s >> %s"),__FUNCTION__,buffer);
//       #endif // ENABLE_LOG_LEVEL_INFO
//     }
//     return buffer;
//   }

// // DEBUG_LINE_HERE;
//   char* name_buffer2 = name_buffer.ptr;

//   #ifdef ENABLE_LOG_LEVEL_INFO
//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("GetDeviceNameWithEnumNumber len=%d"),strlen(buffer));
//   #endif // ENABLE_LOG_LEVEL_INFO

// // DEBUG_LINE_HERE;
//   // gets first index from the array, where we start at the position the desired name is the next name
//   pCONT_sup->GetTextIndexed(buffer, buffer_size, found_index, name_buffer2);

// // DEBUG_LINE_HERE;
//   #ifdef ENABLE_LOG_LEVEL_INFO
//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("GetDeviceNameWithEnumNumber=%s"),buffer);
//   #endif // ENABLE_LOG_LEVEL_INFO
  
//   // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("GetDeviceNameWithEnumNumber &name_buffer[index]=%s"),&name_buffer[index]);

// // DEBUG_LINE_HERE;
//   if(buffer == nullptr){
//     memcpy(buffer,PM_SEARCH_NOMATCH,sizeof(PM_SEARCH_NOMATCH));
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_ERROR, PSTR("F::%s ERROR >> %s"),__FUNCTION__,PM_SEARCH_NOMATCH);
//     #endif // ENABLE_LOG_LEVEL_INFO
//   }

// // DEBUG_LINE_HERE;
//   return buffer;
  
// }


const char* DeviceNameList::GetDeviceName_WithModuleUniqueID(int16_t unique_module_id, int8_t device_id, char* buffer, uint16_t buffer_size, bool flag_respond_nomatch_if_not_found)
{

  ALOG_DBM( PSTR("GetDeviceNameWithEnumNumber(%d,%d)"),unique_module_id, device_id);


// DEBUG_LINE_HERE;

// DEBUG_LINE_HERE;

      // ALOG_INF( PSTR("\t\t\t\t\n\r ------------------------------------- DeviceNameList::GetDeviceNameWithEnumNumber buffer_size %d"),buffer_size);
  //convert enum number to unqiue#pragma region 

  //convert enum number to unqiue#pragma region 

  // module_id = pCONT->GetModuleUniqueIDbyVectorIndex(module_id);

// DEBUG_LINE_HERE;
  int8_t found_index = -1;
  // Check if class & id match
  for(int i=0;i<DEVICENAMEBUFFER_NAME_INDEX_LENGTH;i++)
  {
    
// DEBUG_LINE_HERE;
    // if((number_buffer.unique_group_ids[i]==module_id)&&(number_buffer.index_ids[i]==device_id)){
      
      // ALOG_INF( PSTR("number_buffer.unique_group_ids[i]==unique_module_id)&&(number_buffer.index_ids[i]==device_id)\n\r[%d] %d==%d \t %d==%d"),i, number_buffer.unique_group_ids[i],unique_module_id,number_buffer.index_ids[i],device_id);
      
    
    if((    
      number_buffer.unique_group_ids[i]==unique_module_id)&&(number_buffer.index_ids[i]==device_id)){
      found_index = i;
      
// DEBUG_LINE_HERE;
      // ALOG_INF( PSTR("DeviceNameList::GetDeviceNameWithEnumNumber found_index %d"),i);
      
      break;
    }
  }
// DEBUG_LINE_HERE;
  //future, if none found, have a list of the prefered defaults, relay%d, sensor%d etc

  if(found_index == -1)
  {
// DEBUG_LINE_HERE;
    if(flag_respond_nomatch_if_not_found)
    {
      memcpy(buffer,PM_SEARCH_NOMATCH,sizeof(PM_SEARCH_NOMATCH));
      #ifdef ENABLE_LOG_LEVEL_INFO
      // ALOG_TST(PSTR("F::%s >> %s"),__FUNCTION__,PM_SEARCH_NOMATCH);
      ALOG_TST(PSTR("F::GetDeviceName >> %s"), PM_SEARCH_NOMATCH);
      #endif // ENABLE_LOG_LEVEL_INFO
    

    }
    else
    {  

// DEBUG_LINE_HERE;
// I will need to add an increasing index here with its module name, but important to check how many of that index exist. For now, simply "-1"

/**
 * @brief If unknown, use unique random name
 * 
 */

      // snprintf(buffer, buffer_size, "%S_%02d", pCONT->GetModuleName(unique_module_id), device_id);

      snprintf(buffer, buffer_size, "%S_Unknown_%03d", pCONT->GetModuleName(unique_module_id), random(1000));//device_id);

      #ifdef ENABLE_LOG_LEVEL_INFO
      // ALOG_INF( PSTR("F::%s >> %s"),__FUNCTION__,buffer);
      ALOG_WRN(PSTR("F::GetDeviceName >> %s"), buffer);
      #endif // ENABLE_LOG_LEVEL_INFO
    }
    return buffer;
  }

// DEBUG_LINE_HERE;
  char* name_buffer2 = name_buffer.ptr;

  // ALOG_INF( PSTR("GetDeviceNameWithEnumNumber len=%d"),strlen(buffer));

// DEBUG_LINE_HERE;


// found_index is across the entire buffer, but I also need to track found_index WITHIN the class


  // gets first index from the array, where we start at the position the desired name is the next name
  char* p = pCONT_sup->GetTextIndexed(buffer, buffer_size, found_index, name_buffer2);

// ALOG_INF( PSTR("name_buffer2=%s"),name_buffer2);
// ALOG_INF( PSTR("found_index=%d"),found_index);
// ALOG_INF( PSTR("GetDeviceNameWithEnumNumber=%s"),buffer);
// ALOG_INF( PSTR("p=%s"),p);
  
  // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("GetDeviceNameWithEnumNumber &name_buffer[index]=%s"),&name_buffer[index]);

// DEBUG_LINE_HERE;
  if(buffer == nullptr){
    memcpy(buffer,PM_SEARCH_NOMATCH,sizeof(PM_SEARCH_NOMATCH));
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_ERROR, PSTR("F::%s ERROR >> %s"),__FUNCTION__,PM_SEARCH_NOMATCH);
    #endif // ENABLE_LOG_LEVEL_INFO
  }

// DEBUG_LINE_HERE;
  return buffer;
  
}

bool DeviceNameList::GetModuleAndSensorIDs(const char* module_name, const char* sensor_name, uint16_t* out_module_id, uint8_t* out_sensor_id) {
  //  ALOG_INF(PSTR("GetModuleAndSensorIDsByName(%s, %s)"), module_name, sensor_name);

    int8_t found_index = -1;
    char buffer[64];  // Adjust the size according to your needs

    // DEBUG_LINE_HERE
        // ALOG_INF(PSTR("name_buffer.ptr %s"), name_buffer.ptr);
        // ALOG_INF(PSTR("DLI->GetLengthIndexUsed() %d"), DLI->GetLengthIndexUsed());

    // Iterate over the buffer to find the matching module name and sensor name
    for (int i = 0; i < DLI->GetLengthIndexUsed(); i++) {
        const char* current_module_name = pCONT->GetModuleName(number_buffer.unique_group_ids[i]);
        const char* current_sensor_name = pCONT_sup->GetTextIndexed(buffer, sizeof(buffer), i, name_buffer.ptr);

        // ALOG_INF(PSTR("GetModuleAndSensorIDs %s %s"), current_module_name, current_sensor_name);

    // DEBUG_LINE_HERE
        if (current_module_name && current_sensor_name) {
            if (strcmp(current_module_name, module_name) == 0 && strcmp(current_sensor_name, sensor_name) == 0) {
                found_index = i;
                break;
            }
        }
    }

    // DEBUG_LINE_HERE
    if (found_index != -1) {
        // Match found, return the IDs
        *out_module_id = number_buffer.unique_group_ids[found_index];
        *out_sensor_id = number_buffer.index_ids[found_index];
        ALOG_DBM(PSTR("Match found: Module ID = %d, Sensor ID = %d"), *out_module_id, *out_sensor_id);
        return true;
    } else {
        // No match found
        ALOG_WRN(PSTR("No match found for Module Name = %s, Sensor = %s"), module_name, sensor_name);
        return false;
    }
}

bool DeviceNameList::GetModuleAndSensorIDs(uint16_t module_id, const char* sensor_name, uint16_t* out_module_id, uint8_t* out_sensor_id) {
    ALOG_DBM(PSTR("GetModuleAndSensorIDs(%d, %s)"), module_id, sensor_name);

    int8_t found_index = -1;
    char buffer[64];  // Adjust the size according to your needs

    // Check if the provided module ID and sensor name match any in the buffers
    for(int i = 0; i < DEVICENAMEBUFFER_NAME_INDEX_LENGTH; i++) {
        uint16_t current_module_id = number_buffer.unique_group_ids[i];
        const char* current_sensor_name = pCONT_sup->GetTextIndexed(buffer, sizeof(buffer), number_buffer.index_ids[i], name_buffer.ptr);

        if (current_sensor_name) {
            if (current_module_id == module_id && strcmp(current_sensor_name, sensor_name) == 0) {
                found_index = i;
                break;
            }
        }
    }

    if (found_index != -1) {
        // Match found, return the IDs
        *out_module_id = number_buffer.unique_group_ids[found_index];
        *out_sensor_id = number_buffer.index_ids[found_index];
        ALOG_INF(PSTR("Match found: Module ID = %d, Sensor ID = %d"), *out_module_id, *out_sensor_id);
        return true;
    } else {
        // No match found
        ALOG_WRN(PSTR("No match found for Module ID = %d, Sensor = %s"), module_id, sensor_name);
        return false;
    }
}




// /**
//  * @brief USes no buffer, originally written to fix animation dynamic palette names, I only want the pointer (though null?)
//  * 
//  * @param module_id 
//  * @param device_id 
//  * @param buffer 
//  * @param buffer_size 
//  * @param flag_respond_nomatch_if_not_found 
//  * @return const char* 
//  */
// const char* DeviceNameList::GetDeviceNameWithEnumNumber_NoBuffer(int16_t module_id, int8_t device_id)
// {//}, char* buffer, uint16_t buffer_size, bool flag_respond_nomatch_if_not_found){

//   //convert enum number to unqiue
//   module_id = pCONT->GetModuleUniqueIDbyVectorIndex(module_id);

//   int8_t found_index = -1;
//   // Check if class & id match
//   for(int i=0;i<DEVICENAMEBUFFER_NAME_INDEX_LENGTH;i++){
    
//     // if((number_buffer.unique_group_ids[i]==module_id)&&(number_buffer.index_ids[i]==device_id)){
    
//     if((    
//       number_buffer.unique_group_ids[i]==module_id)&&(number_buffer.index_ids[i]==device_id)){
//       found_index = i;
//       #ifdef ENABLE_LOG_LEVEL_INFO
//       // AddLog(LOG_LEVEL_DEV_TEST,PSTR("DeviceNameList::GetDeviceNameWithEnumNumber found_index %d"),i);
//       #endif // ENABLE_LOG_LEVEL_INFO
//       break;
//     }
//   }
//   //future, if none found, have a list of the prefered defaults, relay%d, sensor%d etc

//   if(found_index == -1)
//   {
//     if(flag_respond_nomatch_if_not_found)
//     {
//       memcpy(buffer,PM_SEARCH_NOMATCH,sizeof(PM_SEARCH_NOMATCH));
//       #ifdef ENABLE_LOG_LEVEL_INFO
//       ALOG_TST(PSTR("F::%s >> %s"),__FUNCTION__,PM_SEARCH_NOMATCH);
//       #endif // ENABLE_LOG_LEVEL_INFO
    

//     }
//     else
//     {  

// // I will need to add an increasing index here with its module name, but important to check how many of that index exist. For now, simply "-1"

//       snprintf(buffer, buffer_size, "%S_%02d", pCONT->GetModuleFriendlyName(pCONT->GetEnumVectorIndexbyModuleUniqueID(module_id)), device_id);

//       #ifdef ENABLE_LOG_LEVEL_INFO
//       ALOG_DBM( PSTR("F::%s >> %s"),__FUNCTION__,buffer);
//       #endif // ENABLE_LOG_LEVEL_INFO
//     }
//     return buffer;
//   }

//   char* name_buffer2 = name_buffer.ptr;

//   #ifdef ENABLE_LOG_LEVEL_INFO
//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("GetDeviceNameWithEnumNumber len=%d"),strlen(buffer));
//   #endif // ENABLE_LOG_LEVEL_INFO

//   // gets first index from the array, where we start at the position the desired name is the next name
//   pCONT_sup->GetTextIndexed(buffer, buffer_size, found_index, name_buffer2);

//   #ifdef ENABLE_LOG_LEVEL_INFO
//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("GetDeviceNameWithEnumNumber=%s"),buffer);
//   #endif // ENABLE_LOG_LEVEL_INFO
  
//   // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("GetDeviceNameWithEnumNumber &name_buffer[index]=%s"),&name_buffer[index]);

//   if(buffer == nullptr){
//     memcpy(buffer,PM_SEARCH_NOMATCH,sizeof(PM_SEARCH_NOMATCH));
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_ERROR, PSTR("F::%s ERROR >> %s"),__FUNCTION__,PM_SEARCH_NOMATCH);
//     #endif // ENABLE_LOG_LEVEL_INFO
//   }

//   return buffer;
  
// }

// search for name, return its index position using pointers
// search for name within devicebuffer, return its class/device id's
// int8_t mSettings::GetDeviceIDbyName(int8_t* unique_module_id, int8_t* device_id, char* name_tofind){

//   // pCONT_sup->GetText


// uint8_t GetDevice_


//   return *device_id;
// }
// int16_t mSettings::GetDeviceIDbyName(const char* name_tofind, int8_t device_id, int8_t unique_module_id){
// //   return GetDeviceIDbyName(name_tofind, Settings.device_name_buffer.name_buffer, &device_id, &unique_module_id);
// // }



// if unique_module_id is nullptr (default value), then ignore matching class first and return first instance of device name
// if unique_module_id is passed, then limit matching results to include that class



char* DeviceNameList::GetTextIndexed(char* destination, size_t destination_size, uint16_t index, const char* haystack)
{

  // DEBUG_LINE_HERE;
  // Returns empty string if not found
  // Returns text of found
  char* write = destination;
  const char* read = haystack;

      // Serial.println(index);
      // Serial.println(destination_size);
      // Serial.printf("destination_size=%d\n\r", destination_size);

  index++;
  while (index--) {
    size_t size = destination_size -1;
    write = destination;
    char ch = '.';
      // Serial.printf("index=%d\n\r", index);
  // DEBUG_LINE_HERE;
    while ((ch != '\0') && (ch != '|')) {

  // DEBUG_LINE_HERE;
      // (addr) (*(const uint8_t *)(addr))

      ch = *read; //get vlaue from pointer
      read++; // move pointer forward

      // Serial.print(ch);

      // ch = pgm_read_byte(read++);  //pads

      if (size && (ch != '|'))  {
  // DEBUG_LINE_HERE;
        *write++ = ch;
        size--;
      }
    }
    if (0 == ch) {
  // DEBUG_LINE_HERE;
      if (index) {
  // DEBUG_LINE_HERE;
        write = destination;
      }
      break;
    }
  }
  *write = '\0';
  return destination;
}


int16_t DeviceNameList::GetDeviceIDbyName(const char* name_tofind, int16_t unique_module_id)
{
  char* buffer = name_buffer.ptr;  
  char substring_buffer[100] = {0};

  uint16_t substring_count = CountCharInCtr(buffer, '|');
  substring_count += 1;

  ALOG_DBM(PSTR("substring_count=%d"),substring_count);

  for(uint16_t name_i=0;name_i<substring_count;name_i++)
  {

    GetTextIndexed(substring_buffer, sizeof(substring_buffer), name_i, buffer);
    ALOG_DBM(PSTR("GetTextIndexed=%s"),substring_buffer);

    // String Compare without Case Sensitivity
    if(strcasecmp(name_tofind, substring_buffer)==0)
    {
      ALOG_DBM(PSTR("MATCH at name_i=%d"),name_i);
      if(number_buffer.unique_group_ids[name_i] == unique_module_id)
      {
        ALOG_DBM(PSTR("MATCH unique_module_id=%d name_i=%d"), number_buffer.unique_group_ids[name_i], name_i);
        return number_buffer.index_ids[name_i];
      }
    }

  }

  return -1;

}

// #elif defined(ENABLE_DEVFEATURE_GETDEVICEIDBYNAME_V2)

// /**
//  * @brief 
//  * 
//  * @note Searches the buffer for the matched name, counting each delimeter when checked, returning the correct delim position when found (also only counts when class should match)
//  * 
//  * @param name_tofind 
//  * @param unique_module_id Should be unique value (ie 4*1000+list or 4002 for relay) and *NOT* the EM_LIST array value
//  * @return int16_t 
//  */
// int16_t DeviceNameList::GetDeviceIDbyName(const char* name_tofind, int16_t unique_module_id)
// {
  
//   //}, int8_t device_id){
// //   return GetDeviceIDbyName(name_tofind, name_buffer.ptr, &device_id, &unique_module_id);
// // }

// // if(unique_module_id > ) //if larger than max of int16_t stop TODO

// /**
//  * @brief device_id is not needed here, and should be removed
//  * 
//  */

// ALOG_INF( PSTR("const char* name_tofind, int16_t unique_module_id => %s %d"), name_tofind, unique_module_id);

// // // if unique_module_id is nullptr (default value), then ignore matching class first and return first instance of device name
// // // if unique_module_id is passed, then limit matching results to include that class
// // int16_t mSettings::GetDeviceIDbyName(const char* name_tofind, const char* haystack, int8_t* device_id, int8_t* unique_module_id)
// // {
//   const char* haystack = name_buffer.ptr;
//   const char* read = haystack;
//   int16_t position = -1;

//   char name_tofind_with_delimeter[50];
//   snprintf(name_tofind_with_delimeter,sizeof(name_tofind_with_delimeter),"%s|",name_tofind);

//   ALOG_DBM( PSTR("Name_tofind_with_delimeter = %s"),name_tofind_with_delimeter);

//   // Search for substring
//   char *p_start_of_found = strstr(haystack,name_tofind_with_delimeter);     //////////// This might be wrong, it is finding the first one and not the right one

//   if(p_start_of_found == NULL){
//     ALOG_ERR( PSTR("p_start_of_found == NOT FOUND") );
//     return -1;
//   }

//   bool limit_result_to_unique_module_ids = true;
//   // if((unique_module_id == nullptr)||
//   if(unique_module_id == -1){
//     limit_result_to_unique_module_ids = false;
//   }
//     #ifdef ENABLE_LOG_LEVEL_INFO
  
//   ALOG_INF(PSTR("limit_result_to_unique_module_ids = %s"),limit_result_to_unique_module_ids?"YES":"NO");

//     #endif// ENABLE_LOG_LEVEL_INFO

//   uint8_t delimeter_count = 0;
//   uint8_t delimeter_within_class_count = 0;
//   uint16_t haystack_index = 0;
//   uint16_t haystack_length = strlen(haystack);

//   ALOG_DBM( PSTR("haystack_length=%d"), haystack_length );

//   //search for delimeters between found index and start ie indexed list places
//   // while((read != p_start_of_found) && (haystack_index++ < haystack_length)){

//   while(haystack_index++ < haystack_length){
//     // Count delimeters
//     if(*read == '|')
//     {
//       if(limit_result_to_unique_module_ids){

        
//         // Search for substring
//         // Need to research under each module group
//         char *p_start_of_found_sub = strstr(read, name_tofind_with_delimeter);     //////////// This might be wrong, it is finding the first one and not the right one

//         if(p_start_of_found_sub == NULL){
//           ALOG_ERR( PSTR("p_start_of_found_sub == NOT FOUND") );
//           return -1;
//         }
  

//         // ALOG_INF( PSTR("unique_group_ids[delimeter_count] == unique_module_id Unique(s%d|%d) (d%d|cc%d)"), number_buffer.unique_group_ids[delimeter_count], unique_module_id, delimeter_count,delimeter_within_class_count );

//         if(number_buffer.unique_group_ids[delimeter_count] == unique_module_id){
//           delimeter_within_class_count++;
//           #ifdef ENABLE_LOG_LEVEL_INFO
//           // AddLog(LOG_LEVEL_DEV_TEST,PSTR("\n\r%s\n\r found wclass_count\n\r %s\n\r %d %d\n\r\n\r"),haystack,read,delimeter_within_class_count,number_buffer.unique_group_ids[delimeter_count]);
//           ALOG_INF( 
//             PSTR(
//               "Searching \"%s in Module %d\"\n\r"
//               "Found \"%s\" @ index %d\n\r"
//               "del count/index pos %d\n\r"
//             ),         
//             name_tofind, number_buffer.unique_group_ids[delimeter_count],
//             p_start_of_found_sub,
//             p_start_of_found_sub - read, // pointer distance
//             delimeter_within_class_count
//           );
//           #endif // ENABLE_LOG_LEVEL_INFO

           
//           if(read == p_start_of_found_sub) // nested if, so already checked if this is valid for module_id
//           {
//             ALOG_INF(PSTR("DO care about matching modedule ID"));
//             break;
//           }


//         }
//         else{
//           ALOG_INF( PSTR("Unwanted Unique ID=%d"), number_buffer.unique_group_ids[delimeter_count] );
//         }
        
//         delimeter_count++;

//       }else{
//         delimeter_within_class_count++;  
//         delimeter_count++;      
//         if(read == p_start_of_found)
//         {
//           ALOG_INF(PSTR("Dont care about matching modedule ID"));
//           break;
//         }



//       }
//       #ifdef ENABLE_LOG_LEVEL_INFO
//       // AddLog(LOG_LEVEL_INFO,PSTR("%s found %s %d"),haystack,read,delimeter_count);
//       #endif // ENABLE_LOG_LEVEL_INFO

//     }
//     read++; //move pointer along
//   }

//   // #ifdef ENABLE_LOG_LEVEL_INFO
//   // ALOG_INF(
//   //   PSTR(
//   //     "\n\rSearching \n\r===(\"%s\")===\n\r"
//   //     "Found \"%s\" @ index %d\n\r"
//   //     "delimeter_count %d\n\r"
//   //     "delimeter_within_class_count %d\n\r"
//   //   ),         
//   //   name_tofind,
//   //   p_start_of_found,
//   //   p_start_of_found - haystack, // pointer distance
//   //   delimeter_count,
//   //   delimeter_within_class_count
//   // );
//   // #endif // ENABLE_LOG_LEVEL_INFO

//   return delimeter_within_class_count;
// }

// #else


// /**
//  * @brief 
//  * 
//  * @note Searches the buffer for the matched name, counting each delimeter when checked, returning the correct delim position when found (also only counts when class should match)
//  * 
//  * @param name_tofind 
//  * @param unique_module_id Should be unique value (ie 4*1000+list or 4002 for relay) and *NOT* the EM_LIST array value
//  * @return int16_t 
//  */
// int16_t DeviceNameList::GetDeviceIDbyName(const char* name_tofind, int16_t unique_module_id)
// {//}, int8_t device_id){
// //   return GetDeviceIDbyName(name_tofind, name_buffer.ptr, &device_id, &unique_module_id);
// // }

// // if(unique_module_id > ) //if larger than max of int16_t stop TODO

// /**
//  * @brief device_id is not needed here, and should be removed
//  * 
//  */

// ALOG_DBM( PSTR("const char* name_tofind, int16_t unique_module_id => %s %d"), name_tofind, unique_module_id);

// // // if unique_module_id is nullptr (default value), then ignore matching class first and return first instance of device name
// // // if unique_module_id is passed, then limit matching results to include that class
// // int16_t mSettings::GetDeviceIDbyName(const char* name_tofind, const char* haystack, int8_t* device_id, int8_t* unique_module_id)
// // {
//   const char* haystack = name_buffer.ptr;
//   const char* read = haystack;
//   int16_t position = -1;

//   char name_tofind_with_delimeter[50];
//   snprintf(name_tofind_with_delimeter,sizeof(name_tofind_with_delimeter),"%s|",name_tofind);

//   ALOG_DBM( PSTR("Name_tofind_with_delimeter = %s"),name_tofind_with_delimeter);

//   // Search for substring
//   char *p_start_of_found = strstr(haystack,name_tofind_with_delimeter);

//   if(p_start_of_found == NULL){
//     ALOG_ERR( PSTR("p_start_of_found == NOT FOUND") );
//     return -1;
//   }

//   bool limit_result_to_unique_module_ids = true;
//   // if((unique_module_id == nullptr)||
//   if(unique_module_id == -1){
//     limit_result_to_unique_module_ids = false;
//   }
//     #ifdef ENABLE_LOG_LEVEL_INFO
  
//   ALOG_DBM( PSTR("limit_result_to_unique_module_ids = %s"),limit_result_to_unique_module_ids?"YES":"NO");

//     #endif// ENABLE_LOG_LEVEL_INFO

//   uint8_t delimeter_count = 0;
//   uint8_t delimeter_within_class_count = 0;
//   uint16_t haystack_index = 0;
//   uint16_t haystack_length = strlen(haystack);

//   ALOG_DBM( PSTR("haystack_length=%d"), haystack_length );

//   //search for delimeters between found index and start ie indexed list places
//   while((read != p_start_of_found) && (haystack_index++ < haystack_length)){
//     // Count delimeters
//     if(*read == '|'){
//       if(limit_result_to_unique_module_ids){

//         ALOG_DBM( PSTR("unique_group_ids[delimeter_count] == unique_module_id %d,%d,%d"), number_buffer.unique_group_ids[delimeter_count],delimeter_count, unique_module_id );

//         if(number_buffer.unique_group_ids[delimeter_count] == unique_module_id){
//           delimeter_within_class_count++;
//           #ifdef ENABLE_LOG_LEVEL_INFO
//           // AddLog(LOG_LEVEL_DEV_TEST,PSTR("\n\r%s\n\r found wclass_count\n\r %s\n\r %d %d\n\r\n\r"),haystack,read,delimeter_within_class_count,number_buffer.unique_group_ids[delimeter_count]);
//           ALOG_DBM( 
//             PSTR(
//               "Searching \"%s\"\n\r"
//               "Found \"%s\" @ index %d\n\r"
//               "del count/index pos %d\n\r"
//             ),         
//             name_tofind,
//             p_start_of_found,
//             p_start_of_found - read, // pointer distance
//             delimeter_within_class_count
//           );
//           #endif // ENABLE_LOG_LEVEL_INFO
//         }
//       }else{
//         delimeter_within_class_count++;        
//       }
//       delimeter_count++;
//     #ifdef ENABLE_LOG_LEVEL_INFO
//       // AddLog(LOG_LEVEL_INFO,PSTR("%s found %s %d"),haystack,read,delimeter_count);
//     #endif // ENABLE_LOG_LEVEL_INFO
//     }
//     read++; //move pointer along
//   }

//   #ifdef ENABLE_LOG_LEVEL_INFO
//           // AddLog(LOG_LEVEL_DEV_TEST,PSTR("\n\r%s\n\r found wclass_count\n\r %s\n\r %d %d\n\r\n\r"),haystack,read,delimeter_within_class_count,number_buffer.unique_group_ids[delimeter_count]);
//           // AddLog(LOG_LEVEL_DEV_TEST,
//           //   PSTR(
//           //     "\n\rSearching \"%s\"\n\r"
//           //     "Found \"%s\" @ index %d\n\r"
//           //     "delimeter_count %d\n\r"
//           //     "delimeter_within_class_count %d\n\r"
//           //   ),         
//           //   name_tofind,
//           //   p_start_of_found,
//           //   p_start_of_found - haystack, // pointer distance
//           //   delimeter_count,
//           //   delimeter_within_class_count
//           // );
//           #endif // ENABLE_LOG_LEVEL_INFO

//   return delimeter_within_class_count;
// }




// #endif // ENABLE_DEVFEATURE_GETDEVICEIDBYNAME_V2




int16_t DeviceNameList::GetIndexOfNthCharPosition(const char* tosearch, char tofind, uint8_t occurance_count){
  uint16_t count = 0;
  for(int16_t i=0;i<strlen(tosearch);i++){
    if(tosearch[i]==tofind) count++;
    if(count == occurance_count) return i;
  }
  return -1;
}

// If no unique_module_id provided, return total driver count, not just within that class
int8_t DeviceNameList::GetDeviceNameCount(int16_t unique_module_id){
  int8_t count = 0;
  for(int i=0;i<DEVICENAMEBUFFER_NAME_INDEX_LENGTH;i++){
    if(number_buffer.unique_group_ids[i]==unique_module_id){
      count++;
      break;
    }
  }
  return count;
}

uint16_t DeviceNameList::CountCharInCtr(const char* tosearch, char tofind)
{
  uint16_t count = 0;
  for(uint16_t i=0;i<strlen(tosearch);i++){
    if(tosearch[i]==tofind){ count++; }
  }
  return count;
}




// #endif