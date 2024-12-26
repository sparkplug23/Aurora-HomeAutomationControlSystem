

#ifndef _MDEVICENAMELIST_H
#define _MDEVICENAMELIST_H

#include <stdint.h>
#include <Arduino.h>

#include "2_CoreSystem/08_Logging/mLogging.h"

/**
 * @brief Memory is still held in settings, but internal pointers are used to write and read from
 * */
class DeviceNameList{
    
  private:
    /* Prevent others from being created */
    DeviceNameList(DeviceNameList const& other) = delete;
    DeviceNameList(DeviceNameList&& other) = delete;
    /* Private constructor to prevent instancing. */
    DeviceNameList(){};
    
    struct NAMEBUFFER{
      char* ptr;
      uint16_t length = 0;
    }name_buffer;

    struct INDEXES{
      /**
       * @note also known as class or module ids
       * */
      uint16_t* unique_group_ids;
      /**
       * @note index also known as device_id, ie index within the group
       * */
      uint8_t*   index_ids;
      /**
       * shared element count for both
       * */
      uint8_t   length = 0;
      uint8_t   currently_used_indexed = 0;
    }number_buffer;

  public:
    // External function to get instance
    static DeviceNameList* GetInstance();
    /* Here will be the instance stored. */
    static DeviceNameList* instance;

    
    char* GetTextIndexed(char* destination, size_t destination_size, uint16_t index, const char* haystack);

    uint8_t GetLengthIndexMax(){ return  number_buffer.length; }
    uint8_t GetLengthIndexUsed(){ return  number_buffer.currently_used_indexed; }

    void Init_NameBuffer(char* buffer, uint16_t size)
    {
      name_buffer.ptr = buffer;
      name_buffer.length = size;
    }

    void Init_NummberBuffer(uint16_t* group_ids, uint8_t* index_ids, uint8_t length)
    {
      number_buffer.unique_group_ids = group_ids;
      number_buffer.index_ids = index_ids;
      number_buffer.length = length;
    }

    void ClearBuffers()
    {
      memset(name_buffer.ptr,                 0, name_buffer.length); //char size
      memset(number_buffer.unique_group_ids, D_MAX_UINT16, number_buffer.length*sizeof(uint16_t));
      memset(number_buffer.index_ids,        D_MAX_UINT8, number_buffer.length*sizeof(uint8_t));
      number_buffer.currently_used_indexed = 0;
    }

    /**
     * @brief init the internal buffer pointers
     * @note test
     * @param name_buffer_ptr
     * @param name_buffer_length 
     * @param number_buffer_unique_group_ids 
     * @param number_buffer_index_ids 
     * @param number_buffer_length 
     * */
    void Init(char* name_buffer_ptr, uint16_t name_buffer_length, uint16_t* number_buffer_unique_group_ids, uint8_t* number_buffer_index_ids, uint8_t number_buffer_length)
    {
      Init_NameBuffer(name_buffer_ptr, name_buffer_length);
      Init_NummberBuffer(number_buffer_unique_group_ids, number_buffer_index_ids, number_buffer_length);
      ClearBuffers();
    }

    uint16_t GetNameBuffer_Length()
    {
      return strlen(name_buffer.ptr);
    }

    int8_t AddDeviceName(const char* name_ctr, int16_t class_id, int8_t device_id);
    int8_t RemoveDeviceName(const char* name_ctr, int16_t class_id, int8_t device_id);
    // const char* GetDeviceNameWithEnumNumber(int16_t module_id, int8_t device_id, char* buffer, uint16_t buffer_size, bool flag_respond_nomatch_if_not_found = false);
    
    
    bool GetModuleAndSensorIDs(const char* module_name, const char* sensor_name, uint16_t* out_module_id, uint8_t* out_sensor_id);
    bool GetModuleAndSensorIDs(uint16_t module_id, const char* sensor_name, uint16_t* out_module_id, uint8_t* out_sensor_id);
    
    // const char* GetDeviceNameWithEnumNumber_NoBuffer(int16_t module_id, int8_t device_id);

    const char* GetDeviceName_WithModuleUniqueID(int16_t unique_module_id, int8_t device_id, char* buffer, uint16_t buffer_size, bool flag_respond_nomatch_if_not_found = false);

    int8_t GetDeviceIDbyName(int16_t* class_id, int8_t* device_id, char* name_tofind);
    int16_t GetDeviceIDbyName(const char* name_tofind, const char* haystack, int8_t* device_id, int16_t* class_id = nullptr);
    // int16_t GetDeviceIDbyName(const char* name_tofind, int8_t device_id = -1, int8_t class_id = -1);
    int16_t GetDeviceIDbyName(const char* name_tofind, int16_t class_id = -1);

    int16_t GetIndexOfNthCharPosition(const char* tosearch, char tofind, uint8_t occurance_count);
    int8_t GetDeviceNameCount(int16_t class_id);
    uint16_t CountCharInCtr(const char* tosearch, char tofind);
        

};

#define DeviceNameListI DeviceNameList::GetInstance()
#define DLI DeviceNameListI


#endif // _MDEVICENAMELIST_H



// #ifndef ENABLE_DEVFEATURE_DEVICENAME_TEXTLIST_BY_UNIQUE_ID


// // Make devicename list into its own cpp, and use #ifdef to enable swapping old and new method until working
// // revamp the functions completely to use the unique name, that way I can always tell what it is using
// // This will need some back and forth for getting vector index

// // /*********************struct DATA_BUFFER2 data_buffer;***********************************************************************/
// //
// // CONSIDER combining driver/sensor into one module "AddSensor"

// void mSettings::ClearAllDeviceName(void){

//   data_buffer.payload.ctr[0] = 0;

//   memset(&Settings.device_name_buffer.name_buffer,0,sizeof(Settings.device_name_buffer.name_buffer));
//   memset(&Settings.device_name_buffer.class_id,DEVICENAME_EMPTY_ID,sizeof(Settings.device_name_buffer.class_id));
//   memset(&Settings.device_name_buffer.device_id,DEVICENAME_EMPTY_ID,sizeof(Settings.device_name_buffer.device_id));

// }

// //returns length
// // Other variations needed:
// //  - Overwrite existing devicename
// //  - Remove by name OR by ID
// int8_t mSettings::AddDeviceName(const char* name_ctr, int16_t class_id, int8_t device_id){

//   char* buffer = Settings.device_name_buffer.name_buffer;
//   uint16_t buffer_length = strlen(buffer);
//     #ifdef ENABLE_LOG_LEVEL_INFO
//   //AddLog(LOG_LEVEL_INFO,PSTR("mSettings::AddDeviceName len=%d"),buffer_length);
//     #endif// ENABLE_LOG_LEVEL_INFO
//   buffer_length = buffer_length > DEVICENAMEBUFFER_NAME_BUFFER_LENGTH ? DEVICENAMEBUFFER_NAME_BUFFER_LENGTH : buffer_length;
  
//   #ifdef USE_LOG
//   //AddLog(LOG_LEVEL_INFO,PSTR("name_bufferB=%s"), buffer);
//   #endif
//   // Check indexing
//   uint8_t index = 0;
//   if(buffer_length){ //if anything in buffer
//     index = CountCharInCtr(buffer,'|'); // first | indicates index from 0
//   }
  
//   // Check not above "id" limits
//   if(index >= (DEVICENAMEBUFFER_NAME_INDEX_LENGTH-1)){
//     return 0; // too many devices in list
//   }

//   uint16_t new_buffer_length = buffer_length+strlen(name_ctr);

//     #ifdef ENABLE_LOG_LEVEL_INFO
//   // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("AddDeviceName(%s,%d,%d) %d index=%d"), 
//   //   name_ctr, 
//   //   class_id, 
//   //   device_id, 
//   //   buffer_length, 
//   //   index
//   // );
//     #endif// ENABLE_LOG_LEVEL_INFO
//   //AddLog(LOG_LEVEL_INFO,PSTR("name_bufferB=%s"), buffer);

//   // Write name to next slot
//   if(new_buffer_length<DEVICENAMEBUFFER_NAME_BUFFER_LENGTH){
//     buffer_length+=sprintf(buffer+buffer_length, "%s|", name_ctr); 
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("AddDeviceName ADDED + \"%s\""),name_ctr); 
//     #endif // ENABLE_LOG_LEVEL_INFO
//   }

//   // Add index to next slot
//   Settings.device_name_buffer.class_id[index] = class_id;
//   Settings.device_name_buffer.device_id[index] = device_id;

//     #ifdef ENABLE_LOG_LEVEL_INFO
//   // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("name_bufferE=%s"), buffer);
//   // AddLog_Array(LOG_LEVEL_DEBUG_MORE,PSTR("class_id"),Settings.device_name_buffer.class_id,(int16_t)DEVICENAMEBUFFER_NAME_INDEX_LENGTH);
//   // AddLog_Array(LOG_LEVEL_DEBUG_MORE,PSTR("device_id"),Settings.device_name_buffer.device_id,(int8_t)DEVICENAMEBUFFER_NAME_INDEX_LENGTH);
//     #endif// ENABLE_LOG_LEVEL_INFO

// }

// int8_t mSettings::RemoveDeviceName(const char* name_ctr, int16_t class_id, int8_t device_id){
//   // Find name, shift results to remove found name
//   // Not needed until I want to remove values
// }

// const char* mSettings::GetDeviceNameWithEnumNumber(int16_t module_id, int8_t device_id, char* buffer, uint16_t buffer_size){

//   int8_t found_index = -1;
//   // Check if class & id match
//   for(int i=0;i<DEVICENAMEBUFFER_NAME_INDEX_LENGTH;i++){
//     if((Settings.device_name_buffer.class_id[i]==module_id)&&(Settings.device_name_buffer.device_id[i]==device_id)){
//       found_index = i;
//     #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("mSettings::GetDeviceNameWithEnumNumber found_index %d"),i);
//     #endif // ENABLE_LOG_LEVEL_INFO
//       break;
//     }
//   }
//   //future, if none found, have a list of the prefered defaults, relay%d, sensor%d etc

//   if(found_index == -1){
//     memcpy(buffer,PM_SEARCH_NOMATCH,sizeof(PM_SEARCH_NOMATCH));
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     ALOG_DBM( PSTR("F::%s >> %s"),__FUNCTION__,PM_SEARCH_NOMATCH);
//     #endif // ENABLE_LOG_LEVEL_INFO
//     return buffer;
//   }

//   char* name_buffer = Settings.device_name_buffer.name_buffer;
//     #ifdef ENABLE_LOG_LEVEL_INFO
//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("GetDeviceNameWithEnumNumber len=%d"),strlen(buffer));
//     #endif // ENABLE_LOG_LEVEL_INFO
//   // gets first index from the array, where we start at the position the desired name is the next name
//   pCONT_sup->GetTextIndexed(buffer, buffer_size, found_index, name_buffer);
//     #ifdef ENABLE_LOG_LEVEL_INFO
//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("GetDeviceNameWithEnumNumber=%s"),buffer);
//     #endif // ENABLE_LOG_LEVEL_INFO
//   // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("GetDeviceNameWithEnumNumber &name_buffer[index]=%s"),&name_buffer[index]);

//   if(buffer == nullptr){
//     memcpy(buffer,PM_SEARCH_NOMATCH,sizeof(PM_SEARCH_NOMATCH));
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_ERROR, PSTR("F::%s ERROR >> %s"),__FUNCTION__,PM_SEARCH_NOMATCH);
//     #endif // ENABLE_LOG_LEVEL_INFO
//   }

//   return buffer;
  
// }

// // search for name, return its index position using pointers
// // search for name within devicebuffer, return its class/device id's
// // int8_t mSettings::GetDeviceIDbyName(int8_t* class_id, int8_t* device_id, char* name_tofind){

// //   // pCONT_sup->GetText


// // uint8_t GetDevice_


// //   return *device_id;
// // }


// // if class_id is nullptr (default value), then ignore matching class first and return first instance of device name
// // if class_id is passed, then limit matching results to include that class
// int16_t mSettings::GetDeviceIDbyName(const char* name_tofind, int8_t device_id, int8_t class_id){
// //   return GetDeviceIDbyName(name_tofind, Settings.device_name_buffer.name_buffer, &device_id, &class_id);
// // }


// // // if class_id is nullptr (default value), then ignore matching class first and return first instance of device name
// // // if class_id is passed, then limit matching results to include that class
// // int16_t mSettings::GetDeviceIDbyName(const char* name_tofind, const char* haystack, int8_t* device_id, int8_t* class_id)
// // {
//   const char* haystack = Settings.device_name_buffer.name_buffer;
//   const char* read = haystack;
//   int16_t position = -1;

//   char name_tofind_with_delimeter[50];
//   snprintf(name_tofind_with_delimeter,sizeof(name_tofind_with_delimeter),"%s|",name_tofind);


//     // AddLog(LOG_LEVEL_INFO,PSTR("Name_tofind_with_delimeter = %s"),name_tofind_with_delimeter);

//   // Search for substring
//   char *p_start_of_found = strstr(haystack,name_tofind_with_delimeter);

//   if(p_start_of_found == NULL){
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("p_start_of_found == NOT FOUND"));
//     #endif// ENABLE_LOG_LEVEL_INFO
//     return -1;
//   }

//   bool limit_result_to_class_ids = true;
//   // if((class_id == nullptr)||
//   if(class_id == -1){
//     limit_result_to_class_ids = false;
//   }
//     #ifdef ENABLE_LOG_LEVEL_INFO
  
//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("limit_result_to_class_ids = %s"),limit_result_to_class_ids?"YES":"NO");

//     #endif// ENABLE_LOG_LEVEL_INFO

//   uint8_t delimeter_count = 0;
//   uint8_t delimeter_within_class_count = 0;
//   uint16_t haystack_index = 0;
//   uint16_t haystack_length = strlen(haystack);
//   //search for delimeters between found index and start ie indexed list places
//   while((read != p_start_of_found) && (haystack_index++ < haystack_length)){
//     // Count delimeters
//     if(*read == '|'){
//       if(limit_result_to_class_ids){
//         if(pCONT_set->Settings.device_name_buffer.class_id[delimeter_count] == class_id){
//           delimeter_within_class_count++;
//           // #ifdef ENABLE_LOG_LEVEL_INFO
//           // // AddLog(LOG_LEVEL_DEV_TEST,PSTR("\n\r%s\n\r found wclass_count\n\r %s\n\r %d %d\n\r\n\r"),haystack,read,delimeter_within_class_count,Settings.device_name_buffer.class_id[delimeter_count]);
//           // AddLog(LOG_LEVEL_DEV_TEST,
//           //   PSTR(
//           //     "Searching \"%s\"\n\r"
//           //     "Found \"%s\" @ index %d\n\r"
//           //     "del count/index pos %d\n\r"
//           //   ),         
//           //   name_tofind,
//           //   p_start_of_found,
//           //   p_start_of_found - read, // pointer distance
//           //   delimeter_within_class_count
//           // );
//           // #endif // ENABLE_LOG_LEVEL_INFO
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
//           // AddLog(LOG_LEVEL_DEV_TEST,PSTR("\n\r%s\n\r found wclass_count\n\r %s\n\r %d %d\n\r\n\r"),haystack,read,delimeter_within_class_count,Settings.device_name_buffer.class_id[delimeter_count]);
//           AddLog(LOG_LEVEL_DEBUG_MORE,
//             PSTR(
//               "\n\rSearching \"%s\"\n\r"
//               "Found \"%s\" @ index %d\n\r"
//               "del count/index pos %d\n\r"
//             ),         
//             name_tofind,
//             p_start_of_found,
//             p_start_of_found - haystack, // pointer distance
//             delimeter_within_class_count
//           );
//           #endif // ENABLE_LOG_LEVEL_INFO

//   return delimeter_within_class_count;
// }




// int16_t mSettings::GetIndexOfNthCharPosition(const char* tosearch, char tofind, uint8_t occurance_count){
//   uint16_t count = 0;
//   for(int16_t i=0;i<strlen(tosearch);i++){
//     if(tosearch[i]==tofind) count++;
//     if(count == occurance_count) return i;
//   }
//   return -1;
// }

// // If no class_id provided, return total driver count, not just within that class
// int8_t mSettings::GetDeviceNameCount(int16_t class_id){
//   int8_t count = 0;
//   for(int i=0;i<DEVICENAMEBUFFER_NAME_INDEX_LENGTH;i++){
//     if(Settings.device_name_buffer.class_id[i]==class_id){
//       count++;
//       break;
//     }
//   }
//   return count;
// }

// uint16_t mSettings::CountCharInCtr(const char* tosearch, char tofind){
//   uint16_t count = 0;
//   for(uint16_t i=0;i<strlen(tosearch);i++){
//     if(tosearch[i]==tofind){ count++; }
//   }
//   return count;
// }

// #else

// //delete as not needed

// // Make devicename list into its own cpp, and use #ifdef to enable swapping old and new method until working
// // revamp the functions completely to use the unique name, that way I can always tell what it is using
// // This will need some back and forth for getting vector index

// // /*********************struct DATA_BUFFER2 data_buffer;***********************************************************************/
// //
// // CONSIDER combining driver/sensor into one module "AddSensor"

// // void mSettings::ClearAllDeviceName(void){

// //   data_buffer.payload.ctr[0] = 0;

// //   memset(&Settings.device_name_buffer.name_buffer,0,sizeof(Settings.device_name_buffer.name_buffer));
// //   memset(&Settings.device_name_buffer.class_id,DEVICENAME_EMPTY_ID,sizeof(Settings.device_name_buffer.class_id));
// //   memset(&Settings.device_name_buffer.device_id,DEVICENAME_EMPTY_ID,sizeof(Settings.device_name_buffer.device_id));

// // }

// //returns length
// // Other variations needed:
// //  - Overwrite existing devicename
// //  - Remove by name OR by ID
// int8_t mSettings::AddDeviceName(const char* name_ctr, int16_t class_id, int8_t device_id){

//   char* buffer = Settings.device_name_buffer.name_buffer;
//   uint16_t buffer_length = strlen(buffer);
//     #ifdef ENABLE_LOG_LEVEL_INFO
//   //AddLog(LOG_LEVEL_INFO,PSTR("mSettings::AddDeviceName len=%d"),buffer_length);
//     #endif// ENABLE_LOG_LEVEL_INFO
//   buffer_length = buffer_length > DEVICENAMEBUFFER_NAME_BUFFER_LENGTH ? DEVICENAMEBUFFER_NAME_BUFFER_LENGTH : buffer_length;
  
//   #ifdef USE_LOG
//   //AddLog(LOG_LEVEL_INFO,PSTR("name_bufferB=%s"), buffer);
//   #endif
//   // Check indexing
//   uint8_t index = 0;
//   if(buffer_length){ //if anything in buffer
//     index = CountCharInCtr(buffer,'|'); // first | indicates index from 0
//   }
  
//   // Check not above "id" limits
//   if(index >= (DEVICENAMEBUFFER_NAME_INDEX_LENGTH-1)){
//     return 0; // too many devices in list
//   }

//   uint16_t new_buffer_length = buffer_length+strlen(name_ctr);

//     #ifdef ENABLE_LOG_LEVEL_INFO
//   // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("AddDeviceName(%s,%d,%d) %d index=%d"), 
//   //   name_ctr, 
//   //   class_id, 
//   //   device_id, 
//   //   buffer_length, 
//   //   index
//   // );
//     #endif// ENABLE_LOG_LEVEL_INFO
//   //AddLog(LOG_LEVEL_INFO,PSTR("name_bufferB=%s"), buffer);

//   // Write name to next slot
//   if(new_buffer_length<DEVICENAMEBUFFER_NAME_BUFFER_LENGTH){
//     buffer_length+=sprintf(buffer+buffer_length, "%s|", name_ctr); 
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("AddDeviceName ADDED + \"%s\""),name_ctr); 
//     #endif // ENABLE_LOG_LEVEL_INFO
//   }

//   // Add index to next slot
//   Settings.device_name_buffer.class_id[index] = class_id;
//   Settings.device_name_buffer.device_id[index] = device_id;

//     #ifdef ENABLE_LOG_LEVEL_INFO
//   // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("name_bufferE=%s"), buffer);
//   // AddLog_Array(LOG_LEVEL_DEBUG_MORE,PSTR("class_id"),Settings.device_name_buffer.class_id,(int16_t)DEVICENAMEBUFFER_NAME_INDEX_LENGTH);
//   // AddLog_Array(LOG_LEVEL_DEBUG_MORE,PSTR("device_id"),Settings.device_name_buffer.device_id,(int8_t)DEVICENAMEBUFFER_NAME_INDEX_LENGTH);
//     #endif// ENABLE_LOG_LEVEL_INFO

// }

// int8_t mSettings::RemoveDeviceName(const char* name_ctr, int16_t class_id, int8_t device_id){
//   // Find name, shift results to remove found name
//   // Not needed until I want to remove values
// }

// const char* mSettings::GetDeviceNameWithEnumNumber(int16_t module_id, int8_t device_id, char* buffer, uint16_t buffer_size){

//   int8_t found_index = -1;
//   // Check if class & id match
//   for(int i=0;i<DEVICENAMEBUFFER_NAME_INDEX_LENGTH;i++){
//     if((Settings.device_name_buffer.class_id[i]==module_id)&&(Settings.device_name_buffer.device_id[i]==device_id)){
//       found_index = i;
//     #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("mSettings::GetDeviceNameWithEnumNumber found_index %d"),i);
//     #endif // ENABLE_LOG_LEVEL_INFO
//       break;
//     }
//   }
//   //future, if none found, have a list of the prefered defaults, relay%d, sensor%d etc

//   if(found_index == -1){
//     memcpy(buffer,PM_SEARCH_NOMATCH,sizeof(PM_SEARCH_NOMATCH));
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     ALOG_DBM( PSTR("F::%s >> %s"),__FUNCTION__,PM_SEARCH_NOMATCH);
//     #endif // ENABLE_LOG_LEVEL_INFO
//     return buffer;
//   }

//   char* name_buffer = Settings.device_name_buffer.name_buffer;
//     #ifdef ENABLE_LOG_LEVEL_INFO
//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("GetDeviceNameWithEnumNumber len=%d"),strlen(buffer));
//     #endif // ENABLE_LOG_LEVEL_INFO
//   // gets first index from the array, where we start at the position the desired name is the next name
//   pCONT_sup->GetTextIndexed(buffer, buffer_size, found_index, name_buffer);
//     #ifdef ENABLE_LOG_LEVEL_INFO
//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("GetDeviceNameWithEnumNumber=%s"),buffer);
//     #endif // ENABLE_LOG_LEVEL_INFO
//   // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("GetDeviceNameWithEnumNumber &name_buffer[index]=%s"),&name_buffer[index]);

//   if(buffer == nullptr){
//     memcpy(buffer,PM_SEARCH_NOMATCH,sizeof(PM_SEARCH_NOMATCH));
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_ERROR, PSTR("F::%s ERROR >> %s"),__FUNCTION__,PM_SEARCH_NOMATCH);
//     #endif // ENABLE_LOG_LEVEL_INFO
//   }

//   return buffer;
  
// }

// // search for name, return its index position using pointers
// // search for name within devicebuffer, return its class/device id's
// // int8_t mSettings::GetDeviceIDbyName(int8_t* class_id, int8_t* device_id, char* name_tofind){

// //   // pCONT_sup->GetText


// // uint8_t GetDevice_


// //   return *device_id;
// // }


// // if class_id is nullptr (default value), then ignore matching class first and return first instance of device name
// // if class_id is passed, then limit matching results to include that class
// int16_t mSettings::GetDeviceIDbyName(const char* name_tofind, int8_t device_id, int8_t class_id){
// //   return GetDeviceIDbyName(name_tofind, Settings.device_name_buffer.name_buffer, &device_id, &class_id);
// // }


// // // if class_id is nullptr (default value), then ignore matching class first and return first instance of device name
// // // if class_id is passed, then limit matching results to include that class
// // int16_t mSettings::GetDeviceIDbyName(const char* name_tofind, const char* haystack, int8_t* device_id, int8_t* class_id)
// // {
//   const char* haystack = Settings.device_name_buffer.name_buffer;
//   const char* read = haystack;
//   int16_t position = -1;

//   char name_tofind_with_delimeter[50];
//   snprintf(name_tofind_with_delimeter,sizeof(name_tofind_with_delimeter),"%s|",name_tofind);


//     // AddLog(LOG_LEVEL_INFO,PSTR("Name_tofind_with_delimeter = %s"),name_tofind_with_delimeter);

//   // Search for substring
//   char *p_start_of_found = strstr(haystack,name_tofind_with_delimeter);

//   if(p_start_of_found == NULL){
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("p_start_of_found == NOT FOUND"));
//     #endif// ENABLE_LOG_LEVEL_INFO
//     return -1;
//   }

//   bool limit_result_to_class_ids = true;
//   // if((class_id == nullptr)||
//   if(class_id == -1){
//     limit_result_to_class_ids = false;
//   }
//     #ifdef ENABLE_LOG_LEVEL_INFO
  
//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("limit_result_to_class_ids = %s"),limit_result_to_class_ids?"YES":"NO");

//     #endif// ENABLE_LOG_LEVEL_INFO

//   uint8_t delimeter_count = 0;
//   uint8_t delimeter_within_class_count = 0;
//   uint16_t haystack_index = 0;
//   uint16_t haystack_length = strlen(haystack);
//   //search for delimeters between found index and start ie indexed list places
//   while((read != p_start_of_found) && (haystack_index++ < haystack_length)){
//     // Count delimeters
//     if(*read == '|'){
//       if(limit_result_to_class_ids){
//         if(pCONT_set->Settings.device_name_buffer.class_id[delimeter_count] == class_id){
//           delimeter_within_class_count++;
//           // #ifdef ENABLE_LOG_LEVEL_INFO
//           // // AddLog(LOG_LEVEL_DEV_TEST,PSTR("\n\r%s\n\r found wclass_count\n\r %s\n\r %d %d\n\r\n\r"),haystack,read,delimeter_within_class_count,Settings.device_name_buffer.class_id[delimeter_count]);
//           // AddLog(LOG_LEVEL_DEV_TEST,
//           //   PSTR(
//           //     "Searching \"%s\"\n\r"
//           //     "Found \"%s\" @ index %d\n\r"
//           //     "del count/index pos %d\n\r"
//           //   ),         
//           //   name_tofind,
//           //   p_start_of_found,
//           //   p_start_of_found - read, // pointer distance
//           //   delimeter_within_class_count
//           // );
//           // #endif // ENABLE_LOG_LEVEL_INFO
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
//           // AddLog(LOG_LEVEL_DEV_TEST,PSTR("\n\r%s\n\r found wclass_count\n\r %s\n\r %d %d\n\r\n\r"),haystack,read,delimeter_within_class_count,Settings.device_name_buffer.class_id[delimeter_count]);
//           AddLog(LOG_LEVEL_DEBUG_MORE,
//             PSTR(
//               "\n\rSearching \"%s\"\n\r"
//               "Found \"%s\" @ index %d\n\r"
//               "del count/index pos %d\n\r"
//             ),         
//             name_tofind,
//             p_start_of_found,
//             p_start_of_found - haystack, // pointer distance
//             delimeter_within_class_count
//           );
//           #endif // ENABLE_LOG_LEVEL_INFO

//   return delimeter_within_class_count;
// }




// int16_t mSettings::GetIndexOfNthCharPosition(const char* tosearch, char tofind, uint8_t occurance_count){
//   uint16_t count = 0;
//   for(int16_t i=0;i<strlen(tosearch);i++){
//     if(tosearch[i]==tofind) count++;
//     if(count == occurance_count) return i;
//   }
//   return -1;
// }

// // If no class_id provided, return total driver count, not just within that class
// int8_t mSettings::GetDeviceNameCount(int16_t class_id){
//   int8_t count = 0;
//   for(int i=0;i<DEVICENAMEBUFFER_NAME_INDEX_LENGTH;i++){
//     if(Settings.device_name_buffer.class_id[i]==class_id){
//       count++;
//       break;
//     }
//   }
//   return count;
// }