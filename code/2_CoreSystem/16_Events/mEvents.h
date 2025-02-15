
/**
 * Basic class that holds event type only
 * The event size will be changed to dynamic later, allowing more complex rules to be save as data
 * */
#ifndef _MEVENT_H
#define _MEVENT_H

#define D_UNIQUE_MODULE_CORE_EVENTS_ID 2016 // [(Folder_Number*100)+ID_File]

#include <stdint.h>


class mEvent
{

  public:
  
    mEvent(){};

    /***
     * 
     * Future change:
     * Events should become their own class, to hold things that happen.
     * Rules can then only be turned on when needed in the primary loop, equally, I should add a check to only go through rules if any are active with a flag
     * 
     * 
     * */

    #define RULE_ENCODED_DATA_MAX_BYTES 10

    struct EVENT_PART{
      /**
       * Buttons, Relays
       * */
      uint16_t module_id = 0; //temporary rename until I can verify all module_ids use Unique and not ENUM codes
      /**
       * InputChange, Timer
       */
      uint16_t function_id = 0;
      /**
       * ?? or buffer[0]
       * */
      uint8_t device_id = 0;
      /**
       * Values
       * */
      struct VALUES{
        uint8_t encoding = 0;
        // dynamic way of handling this in the future, ie the instance of this class creates the size?
        uint8_t data[RULE_ENCODED_DATA_MAX_BYTES]; // should this change to a vector? where would the vector save, and expand to?
        uint8_t length = 0;
      }value;
      /**
       * jsoncommands stored in a dlist
       * */
      uint8_t json_commands_dlist_id = 0; // 0 = none, 1+ is the count
    };

};


// #endif

#endif //ifdef USE_MODULE_CORE_RULES