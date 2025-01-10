/**
 * @file mDisplaysInterface.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "mDisplaysInterface.h"

#ifdef USE_MODULE_DISPLAYS_INTERFACE

int8_t mDisplaysInterface::Tasker(uint8_t function, JsonParserObject obj){
  
  int8_t function_result = 0;

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
      EveryLoop();
    break;  
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
    /************
     * OTHER SECTION * 
    *******************/
    case TASK_SET_POWER:
      // DisplaySetPower();
    break;
    case TASK_EVERY_SECOND:
      #ifdef USE_GRAPH
        DisplayCheckGraph();
      #endif
      #ifdef USE_DT_VARS
        get_dt_mqtt();
        draw_dt_vars();
      #endif // USE_DT_VARS
      SetPower();
    break;
  }
  
  return function_result;

} // END function


void mDisplaysInterface::Pre_Init(void)
{
  module_state.mode = ModuleStatus::Initialising;
  
  pCONT->Tasker_Interface(TASK_DISPLAY_INIT_DRIVER);

  #ifdef USE_MULTI_DISPLAY
    Set_display(0);
  #endif // USE_MULTI_DISPLAY


  if (renderer) {
    
    renderer->setTextFont(tkr_set->Settings.display.font);
    renderer->setTextSize(tkr_set->Settings.display.size);
    
    // force opaque mode
    renderer->setDrawMode(0);

    for (uint32_t cnt = 0; cnt < (MAX_INDEXCOLORS - PREDEF_INDEXCOLORS); cnt++) {
      index_colors[cnt] = 0;
    }
  }
  
  #ifdef USE_DT_VARS
    free_dt_vars();
  #endif

  #ifdef USE_UFILESYS
    Display_Text_From_File("/display.ini");
  #endif

  #ifdef USE_GRAPH
    for (uint8_t count = 0; count < NUM_GRAPHS; count++) { graph[count] = 0; }
  #endif

  ALOG_DBG(PSTR(D_LOG_DEBUG "Display model %d"), tkr_set->Settings.display.model);

  tkr_set->Settings.display.mode = EM_DISPLAY_MODE_UTC_TIME_ID;

  if (tkr_set->Settings.display.model) {
    // tkr_set->runtime.devices_present++;
    // if (!pCONT_pins->PinUsed(GPIO_BACKLIGHT_ID)) {
    //   if (tkr_set->runtime.light_type && (4 == tkr_set->Settings.display.model)) {
    //     tkr_set->runtime.devices_present--;  // Assume PWM channel is used for backlight
    //   }
    // }
    disp_device = 1;//tkr_set->runtime.devices_present;


    #ifdef ENABLE_FEATURE_DISPLAY_LOG_BUFFER
      LogBuffer_Init();
    #endif

  
  }

  if(module_state.devices)
  {
    module_state.mode = ModuleStatus::Running;
  }

  // tkr_set->Settings.display.mode = EM_DISPLAY_MODE_BASIC_BUFFER_TEXT_DRAW;
}

void mDisplaysInterface::Init(void)
{

}

void mDisplaysInterface::Init(uint8_t mode) // this is not my normal init, move to new name
{
  if (renderer)  {
    renderer->DisplayInit(mode, tkr_set->Settings.display.size, tkr_set->Settings.display.rotate, tkr_set->Settings.display.font);
  }
  else {
    dsp_init = mode;
    pCONT->Tasker_Interface(TASK_DISPLAY_INIT);
  }
}







void mDisplaysInterface::EveryLoop()
{

}




void mDisplaysInterface::Clear(void)
{
  pCONT->Tasker_Interface(TASK_DISPLAY_CLEAR);
}

void mDisplaysInterface::DrawStringAt(uint16_t x, uint16_t y, char *str, uint16_t color, uint8_t flag)
{
  dsp_x = x;
  dsp_y = y;
  dsp_str = str;
  dsp_color = color;
  dsp_flag = flag;
  pCONT->Tasker_Interface(TASK_DISPLAY_DRAW_STRING);
}

void mDisplaysInterface::DisplayOnOff(uint8_t on)
{
//   ExecuteCommandPower(disp_device, on, SRC_DISPLAY);
}

/*-------------------------------------------------------------------------------------------*/

// get asci float number
uint8_t mDisplaysInterface::fatoiv(char *cp,float *res) {
  uint8_t index=0;
  *res= pCONT_sup->CharToFloat(cp);
  while (*cp) {
    if ((*cp>='0' && *cp<='9') || (*cp=='-') || (*cp=='.')) {
      cp++;
      index++;
    } else {
      break;
    }
  }
  return index;
}

// get asci number until delimiter and return asci number lenght and value
uint8_t mDisplaysInterface::atoiv(char *cp, int16_t *res)
{
  uint8_t index = 0;
  *res = atoi(cp);
  while (*cp) {
    if ((*cp>='0' && *cp<='9') || (*cp=='-')) {
      cp++;
      index++;
    } else {
      break;
    }
  }
  return index;
}

// get asci number until delimiter and return asci number lenght and value
uint8_t mDisplaysInterface::atoiV(char *cp, uint16_t *res)
{
  uint8_t index = 0;
  *res = atoi(cp);
  while (*cp) {
    if (*cp>='0' && *cp<='9') {
      cp++;
      index++;
    } else {
      break;
    }
  }
  return index;
}

// right align string
void mDisplaysInterface::alignright(char *string) {
  uint16_t slen=strlen(string);
  uint16_t len=slen;
  while (len) {
    // count spaces to the right
    if (string[len-1]!=' ') {
      break;
    }
    len--;
  }
  uint16_t diff=slen-len;
  if (diff>0) {
    // move string
    memmove(&string[diff],string,len);
    memset(string,' ',diff);
  }
}

char *mDisplaysInterface::get_string(char *buff,uint8_t len,char *cp) {
uint8_t index=0;
  while (*cp!=':') {
    buff[index]=*cp++;
    index++;
    if (index>=len) break;
  }
  buff[index]=0;
  cp++;
  return cp;
}

#define ESCAPE_CHAR '~'

// decode text escapes, 1 hexbyte assumed
uint32_t mDisplaysInterface::decode_te(char *line) {
  uint32_t skip = 0;
  char sbuf[3],*cp;
  while (*line) {
    if (*line==ESCAPE_CHAR) {
      cp=line+1;
      if (*cp!=0 && *cp==ESCAPE_CHAR) {
        // escape escape, discard one
        memmove(cp,cp+1,strlen(cp));
        skip++;
      } else {
        // escape HH
        if (strlen(cp)<2) {
          // illegal lenght, ignore
          return skip;
        }
        // take 2 hex chars
        sbuf[0]=*(cp);
        sbuf[1]=*(cp+1);
        sbuf[2]=0;
        *line=strtol(sbuf,0,16);
        // must shift string 2 bytes shift zero also
        memmove(cp,cp+2,strlen(cp)-1);
        skip += 2;
      }
    }
    line++;
  }
  return skip;
}

// /*-------------------------------------------------------------------------------------------*/


// uint16_t GetColorFromIndex(uint32_t index) {
//   if (index >= MAX_INDEXCOLORS) index = 0;

//   if (index < PREDEF_INDEXCOLORS) {
//     return renderer->GetColorFromIndex(index);
//   } else {
//     return index_colors[index - PREDEF_INDEXCOLORS];
//   }
// }

/*********************************************************************************************\
 * Public
\*********************************************************************************************/


void mDisplaysInterface::SetPower(void)
{
  disp_power = 1;//bitRead(XdrvMailbox.index, disp_device -1);

  //ALOG_DBG(PSTR("DSP: Power %d"), disp_power);

  if (tkr_set->Settings.display.model) {
    if (!renderer) {
      pCONT->Tasker_Interface(TASK_DISPLAY_POWER);
    } else {
      renderer->DisplayOnff(disp_power);
    }
  }
  
}


// void DisplayReInitDriver(void) {
//   pCONT->Tasker_Interface(TASK_DISPLAY_INIT_DRIVER);
// #ifdef USE_MULTI_DISPLAY
//   Set_display(0);
// #endif // USE_MULTI_DISPLAY
//   ResponseCmndDone();
// }



/**
 * @brief Indirect function layer to allow debugging
 * @note command will contain checks for viability
 * */
void mDisplaysInterface::SetCursor(uint8_t x, uint8_t y)
{
  ALOG_TST(PSTR("SetCursor(%d,%d)"),x,y);
  pCONT_iDisp->renderer->setCursor(x,y);
}

/**
 * @brief Indirect function layer to allow debugging
 * @note command will contain checks for viability
 * */
void mDisplaysInterface::SetTextSize(uint8_t font_size)
{
  ALOG_TST(PSTR("SetFontSize(%d)"),font_size);
  pCONT_iDisp->renderer->setTextSize(font_size);
}

/**
 * @brief Indirect function layer to allow debugging
 * @note command will contain checks for viability
 * */
void mDisplaysInterface::SetDisplayMode(uint8_t mode)
{
  ALOG_TST(PSTR("SetDisplayMode(%d)"),mode);
  tkr_set->Settings.display.mode = mode;
}



/********************************************************************************************
 * Screen Buffer
 * *******************************************************************************************/

/**
 * @brief Fills the remaining row buffer with 0x20 (Space ASCII) char, by calculating X row length and its maximum size, filling the rest. The last byte is null terminated
 * */
void mDisplaysInterface::ScreenBuffer_SetUnusedRowCharsToSpaceChar(uint32_t line)
{
  uint32_t unused_char_spaces = screen_buffer.cols - strlen(screen_buffer.ptr[line]);
  if (unused_char_spaces) {
    // Fill the remaining chars in line with 0x20
    memset(screen_buffer.ptr[line] + strlen(screen_buffer.ptr[line]), 0x20, unused_char_spaces);
    // Null terminate
    screen_buffer.ptr[line][screen_buffer.cols -1] = 0;
  }
}


void mDisplaysInterface::ScreenBuffer_Clear(void)
{
  if (screen_buffer.cols) {
    for (uint32_t i = 0; i < screen_buffer.rows; i++) {
      memset(screen_buffer.ptr[i], 0, screen_buffer.cols);
    }
  }
}

void mDisplaysInterface::ScreenBuffer_Free(void)
{
  if (screen_buffer.ptr != nullptr) {
    for (uint32_t i = 0; i < screen_buffer.rows; i++) {
      if (screen_buffer.ptr[i] != nullptr) { free(screen_buffer.ptr[i]); }
    }
    free(screen_buffer.ptr);
    screen_buffer.cols = 0;
    screen_buffer.rows = 0;
  }
}

void mDisplaysInterface::ScreenBuffer_Alloc(void)
{
  if (!screen_buffer.cols) {
    screen_buffer.rows = tkr_set->Settings.display.rows;
    screen_buffer.ptr = (char**)malloc(sizeof(*screen_buffer.ptr) * screen_buffer.rows);
    if (screen_buffer.ptr != nullptr) {
      for (uint32_t i = 0; i < screen_buffer.rows; i++) {
        screen_buffer.ptr[i] = (char*)malloc(sizeof(*screen_buffer.ptr[i]) * (tkr_set->Settings.display.cols[0] +1));
        if (screen_buffer.ptr[i] == nullptr) {
          ScreenBuffer_Free();
          break;
        }
      }
    }
    if (screen_buffer.ptr != nullptr) {
      screen_buffer.cols = tkr_set->Settings.display.cols[0] +1;
      ScreenBuffer_Clear();
    }
  }
}

void mDisplaysInterface::ScreenBuffer_ReAlloc(void)
{
  ScreenBuffer_Free();
  ScreenBuffer_Alloc();
}

/*-------------------------------------------------------------------------------------------*/

#ifdef ENABLE_FEATURE_DISPLAY_LOG_BUFFER

void mDisplaysInterface::LogBuffer_Init(void)
{
    
  log_buffer.index = 0;
  log_buffer.ptr_index = 0;
  disp_refresh = tkr_set->Settings.display.refresh;

  LogBuffer_ReAlloc();
 
}

void mDisplaysInterface::LogBuffer_Clear(void)
{
  // If columns were succesful
  if (log_buffer.cols) {
    // Using the row pointer, clear the row (column width) by how many there is
    for (uint32_t i = 0; i < DISPLAY_LOG_ROWS; i++) {
      memset(log_buffer.ptr[i], 0, log_buffer.cols); // note, cols also contains the terminating character
    }
  }
}

void mDisplaysInterface::LogBuffer_Free(void)
{
  // Only free if the pointer has already been set once
  if (log_buffer.ptr != nullptr) {
    // Clear each row, using the pointer to that row
    for (uint32_t i = 0; i < DISPLAY_LOG_ROWS; i++) {
      if (log_buffer.ptr[i] != nullptr) { free(log_buffer.ptr[i]); }
    }
    free(log_buffer.ptr);
    log_buffer.cols = 0;
  }
}

void mDisplaysInterface::LogBuffer_Alloc(void)
{
  if (!log_buffer.cols) {

    // Create a list of pointers to each row
    log_buffer.ptr = (char**)malloc(sizeof(*log_buffer.ptr) * DISPLAY_LOG_ROWS);
    
    // Check if malloc was succesful
    if (log_buffer.ptr != nullptr) {
      
      // Allocate each row pointer, to have enough space for the row + termination
      for (uint32_t i = 0; i < DISPLAY_LOG_ROWS; i++) {
        log_buffer.ptr[i] = (char*)malloc(sizeof(*log_buffer.ptr[i]) * (tkr_set->Settings.display.cols[0] +1));
        // If not succesful, reset buffer
        if (log_buffer.ptr[i] == nullptr) {
          // Clear buffer again
          LogBuffer_Free();
          break;
        }
      }
    }
    // If all of the above worked, then get the number of coloumn available
    if (log_buffer.ptr != nullptr) {
      log_buffer.cols = tkr_set->Settings.display.cols[0] +1;
      // Init the data as empty
      LogBuffer_Clear();
    }
  }
}

void mDisplaysInterface::LogBuffer_ReAlloc(void)
{
  LogBuffer_Free();
  LogBuffer_Alloc();
}

void mDisplaysInterface::LogBuffer_Add(char* txt)
{
  // Cols is just to check if things are initialised
  if (log_buffer.cols) {
    // Copy new text into current line
    strlcpy(log_buffer.ptr[log_buffer.index], txt, log_buffer.cols);  // This preserves the % sign where printf won't
    
    // Add that a new row exists, this will allow "LogBuffer_GetRowPointer" to know an update is required
    log_buffer.index++;
    if (DISPLAY_LOG_ROWS == log_buffer.index) { log_buffer.index = 0; }
  }
}

void mDisplaysInterface::LogBuffer_AddRow(char* txt, uint8_t row_index)
{
  // Cols is just to check if things are initialised
  if (log_buffer.cols) {
    // Copy new text into current line
    if (row_index < DISPLAY_LOG_ROWS)
    {
      strlcpy(log_buffer.ptr[row_index], txt, log_buffer.cols);  // This preserves the % sign where printf won't
    }
  }
}

char* mDisplaysInterface::LogBuffer_GetRowPointer(char temp_code)
{
  char* result = nullptr;
  if (log_buffer.cols) {
    // ALOG_DBG(PSTR("log_buffer.cols=%d"),log_buffer.cols);
    // If currently shown line, is not the latest line, we must update the screen 
    if (log_buffer.index != log_buffer.ptr_index) {
    ALOG_DBG(PSTR("log_buffer.index=%d %d"),log_buffer.index,log_buffer.ptr_index);
      result = log_buffer.ptr[log_buffer.ptr_index];
    ALOG_DBG(PSTR("result=%s"),result);
      log_buffer.ptr_index++;

      // if reached maximum rows, wrap around
      if (DISPLAY_LOG_ROWS == log_buffer.ptr_index) { log_buffer.ptr_index = 0; }

      // Swaps '~' for degree symbol for temperature ( i dont need this on test method)
      char *pch = strchr(result, '~');  // = 0x7E (~) Replace degrees character (276 octal)
      if (pch != nullptr) { result[pch - result] = temp_code; }
    }
  }
  return result;
}

char* mDisplaysInterface::LogBuffer_GetRowPointerByRowIndex(uint8_t row_index)
{
  if(log_buffer.cols)
  {
    if(row_index < DISPLAY_LOG_ROWS)
    {
      return log_buffer.ptr[row_index];
    }
  }
  return nullptr; // invalid row
}

#endif // ENABLE_FEATURE_DISPLAY_LOG_BUFFER



void mDisplaysInterface::parse_JSONCommand(JsonParserObject obj){

  char buffer[50];
  JsonParserToken jtok = 0; 
  JsonParserToken jtok_level = 0; 
  JsonParserObject obj_sub = 0;
  int8_t tmp_id = 0;
  

  bool flag_drivers_needs_reinitialised = false;

  /**
   * @brief New method
   * How to check for a json level that is repeated 
   */
  if(obj_sub = obj[PM_DISPLAY])
  {

    if(jtok = obj_sub[PM_MODEL])
    {
      tkr_set->Settings.display.model = jtok.getInt();
      ALOG_COM( PM_COMMAND_PM_SVALUE_SVALUE_NVALUE, PM_DISPLAY, PM_MODEL, tkr_set->Settings.display.model );
    }

    if(jtok = obj_sub[PM_MODE])
    {
      tkr_set->Settings.display.mode = jtok.getInt();
      ALOG_COM( PM_COMMAND_PM_SVALUE_SVALUE_NVALUE, PM_DISPLAY, PM_MODE, tkr_set->Settings.display.mode );
    }

    // if(jtok = obj["DisplayRefresh"]){
    //   tkr_set->Settings.display.refresh = jtok.getInt();
    // }
    if(jtok = obj_sub["DisplayRows"]){
      tkr_set->Settings.display.rows = jtok.getInt();
      ALOG_INF(PSTR("DisplayRows=%d"), tkr_set->Settings.display.rows);
      // ALOG_COM( PM_COMMAND_PM_SVALUE_NVALUE, PM_DISPLAY, PM_MODEL, tkr_set->Settings.display.model );
    }
    if(jtok = obj_sub["DisplayCols"])
    {
      JsonParserArray arr_pos = jtok;
      if(arr_pos.size() == 2)
      {
        tkr_set->Settings.display.cols[0] = arr_pos[0].getInt();
        tkr_set->Settings.display.cols[1] = arr_pos[1].getInt();
      }
      flag_drivers_needs_reinitialised = true;
    }
    // if(jtok = obj["DisplayDimmer"]){
    //   tkr_set->Settings.display.dimmer = jtok.getInt();
    // }
    if(jtok = obj_sub["DisplaySize"]){
      tkr_set->Settings.display.size = jtok.getInt();
    }
    // if(jtok = obj["DisplayFont"]){
    //   tkr_set->Settings.display.font = jtok.getInt();
    // }
    // if(jtok = obj["DisplayRotate"]){
    //   tkr_set->Settings.display.rotate = jtok.getInt();
    // }


    if(jtok = obj["DisplayText"]){
      CmndDisplayText(jtok.getStr());
    //   if(jtok.isStr()){
    //     if((tmp_id=mPaletteI->GetPaletteIDbyName(jtok.getStr()))>=0){
    //       CommandSet_PaletteID(tmp_id);
    //       data_buffer.isserviced++;
    //     }
    //   }else
    //   if(jtok.isNum()){
    //     CommandSet_PaletteID(jtok.getInt());
    //     data_buffer.isserviced++;
    //   }
    //   #ifdef ENABLE_LOG_LEVEL_DEBUG
      ALOG_DBG(PSTR(D_LOG_LIGHT "DisplayText"));//D_COMMAND_SVALUE_K(D_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette_id, buffer, sizeof(buffer)));
    //   #endif // ENABLE_LOG_LEVEL_DEBUG
    }

    /**
     * @brief Display:AddLog
     * */
    if(jtok = obj_sub[PM_ADDLOG])
    {
      if(jtok.isStr()){
        CommandSet_DisplayAddLog(jtok.getStr());
      }
      ALOG_COM(PM_COMMAND_PM_SVALUE_SVALUE_SVALUE, PM_DISPLAY, PM_ADDLOG, jtok.getStr());
    }

    /**
     * @brief Display:ClearLog
     * */
    if(jtok = obj_sub[PM_CLEARLOG])
    {
      if(jtok.isInt()){
        CommandSet_DisplayClearLog(jtok.getInt());
      }
      ALOG_COM(PM_COMMAND_PM_SVALUE_SVALUE_NVALUE, PM_DISPLAY, PM_CLEARLOG, jtok.getInt());
    }

    if(jtok = obj_sub[PM_BRIGHTNESS])
    {
      // if(jtok.isInt()){
      //   CommandSet_DisplayClearLog(jtok.getInt());
      // }

// I need to introduce how tasmota has a received/mailbox so I can store the brightness to then call sub displays to set them by hardware

      ALOG_COM(PM_COMMAND_PM_SVALUE_SVALUE_NVALUE, PM_DISPLAY, PM_CLEARLOG, jtok.getInt());
    }


    
//     if(!obj["brightness"].isNull()){ 
//       ALOG_INF(PSTR(D_LOG_NEXTION D_PARSING_MATCHED "brightness"));    
//       uint8_t brightness = obj["brightness"];
//       //nextionSetAttr("dim", String(brightness));
//       //sprintf(command_ctr,"dims=%d",brightness);
//       nextionSendCmd("dims=dim");
//     }else



  }






  /**
   * @brief DisplayAddLog
   * */
  if(jtok = obj["DisplayAddStaticLog"]){
    // if(jtok.isStr()){
    //   CommandSet_DisplayAddLog(jtok.getStr());
    // }

    tkr_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;
    LogBuffer_Add((char*)jtok.getStr());

    #ifdef ENABLE_LOG_LEVEL_DEBUG
    ALOG_DBG(PSTR(D_LOG_LIGHT "DisplayAddLog %s"),jtok.getStr());//D_COMMAND_SVALUE_K(D_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette_id, buffer, sizeof(buffer)));
    #endif
  }



  /**
   * @brief DisplayStaticLog with direct index
   * */
  if(jtok = obj["DisplayStaticLog"].getObject()["Row"]){
    uint8_t row_number = jtok.getInt();
    if(jtok = obj["DisplayStaticLog"].getObject()["Text"]){
      tkr_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;
      LogBuffer_AddRow((char*)jtok.getStr(), row_number);
    }
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    ALOG_DBG(PSTR(D_LOG_LIGHT "DisplayAddLog %s"),jtok.getStr());
    #endif
  }

  /**
   * @brief DisplayStaticLog with arrays per line
   * */
  if(jtok = obj["DisplayStaticLog"])
  {
    if(jtok.isArray())
    {
      tkr_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;
      JsonParserArray array = jtok;
      uint8_t index = 0;
      LogBuffer_Clear();
      for(auto& object:array)
      {
        LogBuffer_AddRow((char*)object.getStr(), index++);
      }
    }
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    ALOG_DBG(PSTR(D_LOG_LIGHT "DisplayAddLog %s"),jtok.getStr());
    #endif
  }




  if(jtok = obj["ClearDisplay"]){

    // create time out options where the screen can auto clear

    pCONT_iDisp->renderer->clearDisplay();

      // CmndDisplayText(jtok.getStr());


  }

  if(jtok = obj["DrawText"]){
    CommandSet_DisplayText_Advanced_JSON(obj);

    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    ALOG_DBG(PSTR(D_LOG_LIGHT "DrawText" ));//D_COMMAND_SVALUE_K(D_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette_id, buffer, sizeof(buffer)));
    // #endif // ENABLE_LOG_LEVEL_DEBUG


  }


  if(jtok = obj[PM_DISPLAY_MODE]){
    SetDisplayMode(jtok.getInt());
  }


  if(flag_drivers_needs_reinitialised)
  {
    //restart driver
    Pre_Init();

  }


  // mqtthandler_debug_teleperiod.flags.SendNow = true;

}










#ifdef ENABLE_DISPLAY_MODE_USER_TEXT_SERIALISED

void mDisplaysInterface::DisplayText(const char* buffer)
{
  uint8_t lpos;
  uint8_t escape = 0;
  uint8_t var;
  int16_t lin = 0;
  int16_t col = 0;
  int16_t fill = 0;
  int16_t temp;
  int16_t temp1;
  float ftemp;

  // {"DisplayText":"[Michael]"}

  char linebuf[DISPLAY_BUFFER_COLS];
  char *dp = linebuf;
  char *cp = (char*)buffer;

  ALOG_TST(PSTR("DisplayText %s"),cp);

  memset(linebuf, ' ', sizeof(linebuf));
  linebuf[sizeof(linebuf)-1] = 0;
  *dp = 0;

  while (*cp) {
    if (!escape) {
      // check for escape
      if (*cp == '[') {
        escape = 1;
        cp++;
        // if string in buffer print it
        if ((uint32_t)dp - (uint32_t)linebuf) {
          if (!fill) { *dp = 0; }
          if (col > 0 && lin > 0) {
            // use col and lin
            if (!renderer) DisplayDrawStringAt(col, lin, linebuf, fg_color, 1);
            else renderer->DrawStringAt(col, lin, linebuf, fg_color, 1);
          } else {
            // use disp_xpos, disp_ypos
            if (!renderer) DisplayDrawStringAt(disp_xpos, disp_ypos, linebuf, fg_color, 0);
            else renderer->DrawStringAt(disp_xpos, disp_ypos, linebuf, fg_color, 0);
          }
          memset(linebuf, ' ', sizeof(linebuf));
          linebuf[sizeof(linebuf)-1] = 0;
          dp = linebuf;
        }
      } else {
        // copy chars
        if (dp < (linebuf + DISPLAY_BUFFER_COLS)) { *dp++ = *cp++; }
      }
    } else {
      // check escapes
      if (*cp == ']') {
        escape = 0;
        cp++;
      } else {
        // analyze escapes
        switch (*cp++) {
          case 'z':
            // clear display
            if (!renderer) DisplayClear();
            else renderer->fillScreen(bg_color);
            disp_xpos = 0;
            disp_ypos = 0;
            col = 0;
            lin = 0;
            break;
          case 'i':
            // init display with partial update
            DisplayInit(DISPLAY_INIT_PARTIAL);
            break;
          case 'I':
            // init display with full refresh
            DisplayInit(DISPLAY_INIT_FULL);
            break;
          case 'o':
            DisplayOnOff(0);
            break;
          case 'O':
            DisplayOnOff(1);
            break;
          case 'x':
            // set disp_xpos
            var = atoiv(cp, &disp_xpos);
            cp += var;
            break;
          case 'y':
            // set disp_ypos
            var = atoiv(cp, &disp_ypos);
            cp += var;
            break;
          case 'l':
            // text line lxx
            var = atoiv(cp, &lin);
            cp += var;
            //display.setCursor(display.getCursorX(),(lin-1)*font_y*txtsize);
            break;
          case 'c':
            // text column cxx
            var = atoiv(cp, &col);
            cp += var;
            //display.setCursor((col-1)*font_x*txtsize,display.getCursorY());
            break;
          // case 'C':
          //   // text color cxx
          //   if (*cp=='i') {
          //     // color index 0-18
          //     cp++;
          //     var = atoiv(cp, &temp);
          //     if (renderer) ftemp = GetColorFromIndex(temp);
          //   } else {
          //     // float because it must handle unsigned 16 bit
          //     var = fatoiv(cp,&ftemp);
          //   }
          //   fg_color=ftemp;
          //   cp += var;
          //   if (renderer) renderer->setTextColor(fg_color,bg_color);
          //   break;
          // case 'B':
          //   // bg color Bxx
          //   if (*cp=='i') {
          //     // color index 0-18
          //     cp++;
          //     var = atoiv(cp, &temp);
          //     if (renderer) ftemp = GetColorFromIndex(temp);
          //   } else {
          //     var = fatoiv(cp,&ftemp);
          //   }
          //   bg_color=ftemp;
          //   cp += var;
          //   if (renderer) renderer->setTextColor(fg_color,bg_color);
          //   break;
          case 'p':
            // pad field with spaces fxx
            var = atoiv(cp, &fill);
            cp += var;
            linebuf[fill] = 0;
            break;
// #ifdef USE_UFILESYS
//           case 'P':
//             { char *ep=strchr(cp,':');
//              if (ep) {
//                *ep=0;
//                ep++;
//                Draw_RGB_Bitmap(cp,disp_xpos,disp_ypos, false);
//                cp=ep;
//              }
//             }
//             break;
// #ifdef USE_MULTI_DISPLAY
//           case 'S':
//             {
//               var = atoiv(cp, &temp);
//               cp += var;
//               if (temp < 1 || temp > 3) {
//                 temp = 1;
//               }
//               temp--;
//               if (*cp == ':') {
//                 cp++;
//                 if (displays[temp].display) {
//                   Set_display(cur_display);
//                   Get_display(temp);
//                 }
//               } else {
//                 char *ep=strchr(cp,':');
//                 if (ep) {
//                   *ep=0;
//                   ep++;
//                   File fp;
//                   if (ffsp) {
//                     ALOG_INF(PSTR("DSP: File: %s"),cp);
//                     fp = ffsp->open(cp, "r");
//                     if (fp > 0) {
//                       uint32_t size = fp.size();
//                       char *fdesc = (char *)calloc(size + 4, 1);
//                       if (fdesc) {
//                         fp.read((uint8_t*)fdesc, size);
//                         fp.close();
//                         Get_display(temp);
//                         renderer = Init_uDisplay(fdesc, -1);
//                         Set_display(temp);
//                         ALOG_INF(PSTR("DSP: File descriptor loaded %x"),renderer);
//                       }
//                     }
//                   }
//                 }
//                 cp = ep;
//               }
//             }
//             break;
// #endif // USE_MULTI_DISPLAY
// #endif // USE_UFILESYS
          case 'h':
            // hor line to
            var = atoiv(cp, &temp);
            cp += var;
            if (temp < 0) {
              if (renderer) renderer->writeFastHLine(disp_xpos + temp, disp_ypos, -temp, fg_color);
              //else DisplayDrawHLine(disp_xpos + temp, disp_ypos, -temp, fg_color);
            } else {
              if (renderer) renderer->writeFastHLine(disp_xpos, disp_ypos, temp, fg_color);
              //else DisplayDrawHLine(disp_xpos, disp_ypos, temp, fg_color);
            }
            disp_xpos += temp;
            break;
          case 'v':
            // vert line to
            var = atoiv(cp, &temp);
            cp += var;
            if (temp < 0) {
              if (renderer) renderer->writeFastVLine(disp_xpos, disp_ypos + temp, -temp, fg_color);
              //else DisplayDrawVLine(disp_xpos, disp_ypos + temp, -temp, fg_color);
            } else {
              if (renderer) renderer->writeFastVLine(disp_xpos, disp_ypos, temp, fg_color);
              //else DisplayDrawVLine(disp_xpos, disp_ypos, temp, fg_color);
            }
            disp_ypos += temp;
            break;
          case 'L':
            // any line to
            var = atoiv(cp, &temp);
            cp += var;
            cp++;
            var = atoiv(cp, &temp1);
            cp += var;
            if (renderer) renderer->writeLine(disp_xpos, disp_ypos, temp, temp1, fg_color);
            //else DisplayDrawLine(disp_xpos, disp_ypos, temp, temp1, fg_color);
            disp_xpos += temp;
            disp_ypos += temp1;
            break;
          case 'k':
            // circle
            var = atoiv(cp, &temp);
            cp += var;
            if (renderer) renderer->drawCircle(disp_xpos, disp_ypos, temp, fg_color);
            //else DisplayDrawCircle(disp_xpos, disp_ypos, temp, fg_color);
            break;
          case 'K':
            // filled circle
            var = atoiv(cp, &temp);
            cp += var;
            if (renderer) renderer->fillCircle(disp_xpos, disp_ypos, temp, fg_color);
            //else DisplayDrawFilledCircle(disp_xpos, disp_ypos, temp, fg_color);
            break;
          case 'r':
            // rectangle
            var = atoiv(cp, &temp);
            cp += var;
            cp++;
            var = atoiv(cp, &temp1);
            cp += var;
            if (renderer) renderer->drawRect(disp_xpos, disp_ypos, temp, temp1, fg_color);
            //else DisplayDrawRectangle(disp_xpos, disp_ypos, temp, temp1, fg_color);
            break;
          case 'R':
            // filled rectangle
            var = atoiv(cp, &temp);
            cp += var;
            cp++;
            var = atoiv(cp, &temp1);
            cp += var;
            if (renderer) renderer->fillRect(disp_xpos, disp_ypos, temp, temp1, fg_color);
            //else DisplayDrawFilledRectangle(disp_xpos, disp_ypos, temp, temp1, fg_color);
            break;
          case 'u':
            // rounded rectangle
            { int16_t rad;
            var = atoiv(cp, &temp);
            cp += var;
            cp++;
            var = atoiv(cp, &temp1);
            cp += var;
            cp++;
            var = atoiv(cp, &rad);
            cp += var;
            if (renderer) renderer->drawRoundRect(disp_xpos, disp_ypos, temp, temp1, rad, fg_color);
              //else DisplayDrawFilledRectangle(disp_xpos, disp_ypos, temp, temp1, fg_color);
            }
            break;
          case 'U':
            // rounded rectangle
            { int16_t rad;
            var = atoiv(cp, &temp);
            cp += var;
            cp++;
            var = atoiv(cp, &temp1);
            cp += var;
            cp++;
            var = atoiv(cp, &rad);
            cp += var;
            if (renderer) renderer->fillRoundRect(disp_xpos, disp_ypos, temp, temp1, rad, fg_color);
                  //else DisplayDrawFilledRectangle(disp_xpos, disp_ypos, temp, temp1, fg_color);
            }
            break;

          // case 't':
          //   if (*cp=='S') {
          //     cp++;
          //     if (dp < (linebuf + DISPLAY_BUFFER_COLS) -8) {
          //       snprintf_P(dp, 9, PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute, RtcTime.second);
          //       dp += 8;
          //     }
          //   } else {
          //     if (dp < (linebuf + DISPLAY_BUFFER_COLS) -5) {
          //       snprintf_P(dp, 6, PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute);
          //       dp += 5;
          //     }
          //   }
          //   break;
          // case 'T': {
          //   uint8_t param1 = RtcTime.day_of_month;
          //   uint8_t param2 = RtcTime.month;
          //   if (*cp=='U') {
          //     cp++;
          //     param1 = RtcTime.month;
          //     param2 = RtcTime.day_of_month;
          //   }
          //   if (dp < (linebuf + DISPLAY_BUFFER_COLS) -8) {
          //     snprintf_P(dp, 9, PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%02d"), param1, param2, RtcTime.year%2000);
          //     dp += 8;
          //   }
          //   break; }
//           case 'd':
//             if (*cp == 'c') {
//               cp++;
//               // define index colo
//               var = atoiv(cp, &temp);
//               cp += var;
//               cp++;
//               var = fatoiv(cp, &ftemp);
//               cp += var;
//               if (temp >= MAX_INDEXCOLORS) temp = PREDEF_INDEXCOLORS;
//               if (temp < PREDEF_INDEXCOLORS) temp = PREDEF_INDEXCOLORS;
//               index_colors[temp - PREDEF_INDEXCOLORS] = ftemp;
//               break;
//             }
// #ifdef USE_DT_VARS
//           if (*cp == 'v') {
//             cp++;
//             { int16_t num, gxp, gyp, textbcol, textfcol, font, textsize, txlen, dp, time;
//               var=atoiv(cp,&num);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&gxp);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&gyp);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&textbcol);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&textfcol);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&font);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&textsize);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&txlen);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&dp);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&time);
//               cp+=var;
//               cp++;
//               // text itself
//               char bbuff[32];
//               cp = get_string(bbuff, sizeof(bbuff), cp);
//               char unit[4];
//               cp = get_string(unit, sizeof(unit), cp);
//               define_dt_var(num, gxp, gyp, textbcol, textfcol, font, textsize, txlen, time, dp, bbuff, unit);
//             }
//           }
// #endif // USE_DT_VARS
//             // force draw grafics buffer
//             if (renderer) renderer->Updateframe();
//             //else DisplayDrawFrame();
//             break;
          case 'D':
            // set auto draw mode
            auto_draw=*cp&3;
            if (renderer) renderer->setDrawMode(auto_draw>>1);
            cp += 1;
            break;
          case 's':
            // size sx
            if (renderer) renderer->setTextSize(*cp&7);
            //else DisplaySetSize(*cp&3);
            cp += 1;
            break;
          case 'f':
            // font sx
            { uint8_t font = *cp&7;
              if (renderer) renderer->setTextFont(font);
              //else DisplaySetFont(font);
              if (font) {
                // for backward compatibility set size to 1 on non GFX fonts
                if (renderer) renderer->setTextSize(1);
                //else DisplaySetSize(1);
              }
              cp += 1;
            }
            break;
// #ifdef USE_UFILESYS
// #ifdef USE_RAMFONT
// extern FS *ffsp;
//           case 'F':
//             { char *ep = strchr(cp,':');
//               if (ep) {
//                 static uint8_t *ram_font;
//                 char fname[32];
//                 *ep = 0;
//                 ep++;
//                 if (*cp != '/') {
//                   fname[0] = '/';
//                   fname[1] = 0;
//                 } else {
//                   fname[0] = 0;
//                 }
//                 strlcat(fname, cp, sizeof(fname));
//                 if (!strstr(cp, ".fnt")) {
//                   strlcat(fname, ".fnt", sizeof(fname));
//                 }
//                 if (ffsp) {
//                   File fp;
//                   fp = ffsp->open(fname, "r");
//                   if (fp > 0) {
//                     uint32_t size = fp.size();
//                     if (ram_font) free (ram_font);
//                     ram_font = (uint8_t*)special_malloc(size + 4);
//                     fp.read((uint8_t*)ram_font, size);
//                     fp.close();
//                     if (renderer) renderer->SetRamfont(ram_font);
//                     //Serial.printf("Font loaded: %s\n",fname );
//                   }
//                 }
//                 cp = ep;
//               }
//             }
//             break;
// #endif // USE_RAMFONT
// #endif // USE_UFILESYS
          case 'a':
            // rotation angle
            if (renderer) renderer->setRotation(*cp&3);
            //else DisplaySetRotation(*cp&3);
            cp+=1;
            break;

// #ifdef USE_GRAPH
//           case 'G':
//             // define graph
//             if (*cp=='d') {
//               cp++;
//               var=atoiv(cp,&temp);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&temp1);
//               cp+=var;
//               RedrawGraph(temp,temp1);
//               break;
//             }
// #if (defined(USE_SCRIPT_FATFS) && defined(USE_SCRIPT)) || defined(USE_UFILESYS)
//             if (*cp=='s') {
//               cp++;
//               var=atoiv(cp,&temp);
//               cp+=var;
//               cp++;
//               // path
//               char bbuff[128];
//               cp=get_string(bbuff,sizeof(bbuff),cp);
//               Save_graph(temp,bbuff);
//               break;
//             }
//             if (*cp=='r') {
//               cp++;
//               var=atoiv(cp,&temp);
//               cp+=var;
//               cp++;
//               // path
//               char bbuff[128];
//               cp=get_string(bbuff,sizeof(bbuff),cp);
//               Restore_graph(temp,bbuff);
//               break;
//             }
// #endif // USE_SCRIPT_FATFS
//             { int16_t num,gxp,gyp,gxs,gys,dec,icol;
//               float ymin,ymax;
//               var=atoiv(cp,&num);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&gxp);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&gyp);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&gxs);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&gys);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&dec);
//               cp+=var;
//               cp++;
//               var=fatoiv(cp,&ymin);
//               cp+=var;
//               cp++;
//               var=fatoiv(cp,&ymax);
//               cp+=var;
//               if (color_type==COLOR_COLOR) {
//                 // color graph requires channel color
//                 cp++;
//                 var=atoiv(cp,&icol);
//                 cp+=var;
//               } else {
//                 icol=0;
//               }
//               DefineGraph(num,gxp,gyp,gxs,gys,dec,ymin,ymax,icol);
//             }
//             break;
//           case 'g':
//               { float temp;
//                 int16_t num;
//                 var=atoiv(cp,&num);
//                 cp+=var;
//                 cp++;
//                 var=fatoiv(cp,&temp);
//                 cp+=var;
//                 AddValue(num,temp);
//               }
//             break;
// #endif // USE_GRAPH

// #ifdef USE_AWATCH
//           case 'w':
//               var = atoiv(cp, &temp);
//               cp += var;
//               DrawAClock(temp);
//               break;
// #endif // USE_AWATCH

// #ifdef USE_TOUCH_BUTTONS
//           case 'b':
//           { int16_t num, gxp, gyp, gxs, gys, outline, fill, textcolor, textsize; uint8_t dflg = 1, sbt = 0;
//             if (*cp == 'e' || *cp == 'd') {
//               // enable disable
//               uint8_t dis = 0;
//               if (*cp == 'd') dis = 1;
//               cp++;
//               var = atoiv(cp, &num);
//               num = num % MAX_TOUCH_BUTTONS;
//               cp += var;
//               if (buttons[num]) {
//                 buttons[num]->vpower.disable = dis;
//                 if (!dis) {
//                   if (buttons[num]->vpower.is_virtual) buttons[num]->xdrawButton(buttons[num]->vpower.on_off);
//                   else buttons[num]->xdrawButton(bitRead(tkr_set->runtime_var.power,num));
//                 }
//               }
//               break;
//             }
//             if (*cp == '-') {
//               cp++;
//               dflg = 0;
//             }
//             if (*cp == 's') {
//               cp++;
//               sbt = 1;
//             }
//             var=atoiv(cp,&num);
//             cp+=var;
//             uint8_t bflags=num>>8;
//             num=num%MAX_TOUCH_BUTTONS;
//             if (*cp == 's') {
//               cp++;
//               var=atoiv(cp,&gxp);
//               if (buttons[num]) {
//                 // set slider or button
//                 if (buttons[num]->vpower.slider) {
//                   buttons[num]->UpdateSlider(-gxp, -gxp);
//                 } else {
//                   buttons[num]->vpower.on_off = gxp;
//                   buttons[num]->xdrawButton(buttons[num]->vpower.on_off);
//                 }
//               }
//               break;
//             }
//             cp++;
//             var=atoiv(cp,&gxp);
//             cp+=var;
//             cp++;
//             var=atoiv(cp,&gyp);
//             cp+=var;
//             cp++;
//             var=atoiv(cp,&gxs);
//             cp+=var;
//             cp++;
//             var=atoiv(cp,&gys);
//             cp+=var;
//             cp++;
//             var=atoiv(cp,&outline);
//             cp+=var;
//             cp++;
//             var=atoiv(cp,&fill);
//             cp+=var;
//             cp++;
//             var=atoiv(cp,&textcolor);
//             cp+=var;
//             cp++;
//             var=atoiv(cp,&textsize);
//             cp+=var;
//             cp++;
//             // text itself
//             char bbuff[32];
//             if (!sbt) {
//               // text itself
//               cp = get_string(bbuff, sizeof(bbuff), cp);
//             }
//             if (buttons[num]) {
//               delete buttons[num];
//             }
//             if (renderer) {
//               buttons[num]= new VButton();
//               if (buttons[num]) {
//                 if (!sbt) {
//                   buttons[num]->vpower.slider = 0;
//                   buttons[num]->initButtonUL(renderer, gxp, gyp, gxs, gys, GetColorFromIndex(outline),\
//                     GetColorFromIndex(fill), GetColorFromIndex(textcolor), bbuff, textsize);
//                   if (!bflags) {
//                     // power button
//                     if (dflg) buttons[num]->xdrawButton(bitRead(tkr_set->runtime_var.power, num));
//                     buttons[num]->vpower.is_virtual = 0;
//                   } else {
//                     // virtual button
//                     buttons[num]->vpower.is_virtual = 1;
//                     if (bflags==2) {
//                       // push
//                       buttons[num]->vpower.is_pushbutton = 1;
//                     } else {
//                       // toggle
//                       buttons[num]->vpower.is_pushbutton = 0;
//                     }
//                     if (dflg) buttons[num]->xdrawButton(buttons[num]->vpower.on_off);
//                     buttons[num]->vpower.disable = !dflg;
//                   }
//                 } else {
//                   // slider
//                   buttons[num]->vpower.slider = 1;
//                   buttons[num]->SliderInit(renderer, gxp, gyp, gxs, gys, outline, GetColorFromIndex(fill),\
//                     GetColorFromIndex(textcolor), GetColorFromIndex(textsize));
//                 }
//               }
//             }
//           }
//           break;
// #endif // USE_TOUCH_BUTTONS
          default:
            // unknown escape
            // Response_P(PSTR("Unknown Escape"));
            goto exit;
            break;
        }
      }
    }
  }
  exit:
  // now draw buffer
    dp -= decode_te(linebuf);
    if ((uint32_t)dp - (uint32_t)linebuf) {
      if (!fill) {
        *dp = 0;
      } else {
        linebuf[abs(int(fill))] = 0;
      }
      if (fill<0) {
        // right align
        alignright(linebuf);
      }
      if (col > 0 && lin > 0) {
        // use col and lin
        if (!renderer) DisplayDrawStringAt(col, lin, linebuf, fg_color, 1);
        else renderer->DrawStringAt(col, lin, linebuf, fg_color, 1);
      } else {
        // use disp_xpos, disp_ypos
        if (!renderer) DisplayDrawStringAt(disp_xpos, disp_ypos, linebuf, fg_color, 0);
        else renderer->DrawStringAt(disp_xpos, disp_ypos, linebuf, fg_color, 0);
      }
    }
    // draw buffer
    if (auto_draw&1) {
      if (renderer) renderer->Updateframe();
      //else DisplayDrawFrame();
    }
} //end function

#endif // ENABLE_DISPLAY_MODE_USER_TEXT_SERIALISED





















// /*********************************************************************************************\
//  * Commands
// \*********************************************************************************************/


/**
 * @brief CommandSet_DisplayAddLog
 * */
void mDisplaysInterface::CommandSet_DisplayAddLog(const char* c)
{  
  SetDisplayMode(EM_DISPLAY_MODE_LOCAL1_ID);
  LogBuffer_Add((char*)c);

  // Change to flag method later, so this function will not be called for every command
  pCONT->Tasker_Interface(TASK_DISPLAY_REFRESH_SHOW_ID);

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  ALOG_DBG(PSTR(D_LOG_LIGHT "DisplayAddLog %s"),c);//D_COMMAND_SVALUE_K(D_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette_id, buffer, sizeof(buffer)));
  #endif
}

/**
 * @brief CommandSet_DisplayClearLog
 * */
void mDisplaysInterface::CommandSet_DisplayClearLog(bool d)
{  
  LogBuffer_Clear(); 
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  ALOG_DBG(PSTR(D_LOG_LIGHT "DisplayClearLog"));//D_COMMAND_SVALUE_K(D_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette_id, buffer, sizeof(buffer)));
  #endif
}


/**
 * @brief CommandSet_DisplayAddLog
 * */
// void mDisplaysInterface::CommandSet_DisplayAddLog(const char* c)
// {  
//   SetDisplayMode(EM_DISPLAY_MODE_LOG_STATIC_ID);
//   LogBuffer_Add((char*)c);
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   ALOG_DBG(PSTR(D_LOG_LIGHT "DisplayAddLog %s"),c);//D_COMMAND_SVALUE_K(D_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette_id, buffer, sizeof(buffer)));
//   #endif
// }





// void CmndDisplay(void) {
//   Response_P(PSTR("{\"" D_PRFX_DISPLAY "\":{\"" D_CMND_DISP_MODEL "\":%d,\"" D_CMND_DISP_TYPE "\":%d,\"" D_CMND_DISP_WIDTH "\":%d,\"" D_CMND_DISP_HEIGHT "\":%d,\""
//     D_CMND_DISP_MODE "\":%d,\"" D_CMND_DISP_DIMMER "\":%d,\"" D_CMND_DISP_SIZE "\":%d,\"" D_CMND_DISP_FONT "\":%d,\""
//     D_CMND_DISP_ROTATE "\":%d,\"" D_CMND_DISP_INVERT "\":%d,\"" D_CMND_DISP_REFRESH "\":%d,\"" D_CMND_DISP_COLS "\":[%d,%d],\"" D_CMND_DISP_ROWS "\":%d}}"),
//     tkr_set->Settings.display.model, tkr_set->Settings.display.options.type, tkr_set->Settings.display.width, tkr_set->Settings.display.height,
//     tkr_set->Settings.display.mode, changeUIntScale(tkr_set->Settings.display.dimmer, 0, 15, 0, 100), tkr_set->Settings.display.size, tkr_set->Settings.display.font,
//     tkr_set->Settings.display.rotate, tkr_set->Settings.display.options.invert, tkr_set->Settings.display.refresh, tkr_set->Settings.display.cols[0], tkr_set->Settings.display.cols[1], tkr_set->Settings.display.rows);
// }

// void CmndDisplayModel(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < DISPLAY_MAX_DRIVERS)) {
//     uint32_t last_display_model = tkr_set->Settings.display.model;
//     tkr_set->Settings.display.model = XdrvMailbox.payload;
//     if (pCONT->Tasker_Interface(TASK_DISPLAY_MODEL)) {
//       tkr_set->runtime_var.restart_flag = 2;  // Restart to re-init interface and add/Remove MQTT subscribe
//     } else {
//       tkr_set->Settings.display.model = last_display_model;
//     }
//   }
//   ResponseCmndNumber(tkr_set->Settings.display.model);
// }

// void CmndDisplayType(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 7)) {
//     tkr_set->Settings.display.options.type = XdrvMailbox.payload;
//     tkr_set->runtime_var.restart_flag = 2;
//   }
//   ResponseCmndNumber(tkr_set->Settings.display.options.type);
// }

// void CmndDisplayWidth(void) {
//   if (XdrvMailbox.payload > 0) {
//     if (XdrvMailbox.payload != tkr_set->Settings.display.width) {
//       tkr_set->Settings.display.width = XdrvMailbox.payload;
//       tkr_set->runtime_var.restart_flag = 2;  // Restart to re-init width
//     }
//   }
//   ResponseCmndNumber(tkr_set->Settings.display.width);
// }

// void CmndDisplayHeight(void) {
//   if (XdrvMailbox.payload > 0) {
//     if (XdrvMailbox.payload != tkr_set->Settings.display.height) {
//       tkr_set->Settings.display.height = XdrvMailbox.payload;
//       tkr_set->runtime_var.restart_flag = 2;  // Restart to re-init height
//     }
//   }
//   ResponseCmndNumber(tkr_set->Settings.display.height);
// }

// void CmndDisplayMode(void) {
// #ifdef USE_DISPLAY_MODES1TO5
// /*     Matrix / 7-segment   LCD / Oled                           TFT
//  * 1 = Text up and time     Time
//  * 2 = Date                 Local sensors                        Local sensors
//  * 3 = Day                  Local sensors and time               Local sensors and time
//  * 4 = Mqtt left and time   Mqtt (incl local) sensors            Mqtt (incl local) sensors
//  * 5 = Mqtt up and time     Mqtt (incl local) sensors and time   Mqtt (incl local) sensors and time
// */
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 5)) {
//     uint32_t last_display_mode = tkr_set->Settings.display.mode;
//     tkr_set->Settings.display.mode = XdrvMailbox.payload;

//     if (disp_subscribed != (tkr_set->Settings.display.mode &0x04)) {
//       tkr_set->runtime_var.restart_flag = 2;  // Restart to Add/Remove MQTT subscribe
//     } else {
//       if (last_display_mode && !tkr_set->Settings.display.mode) {  // Switch to mode 0
//         DisplayInit(DISPLAY_INIT_MODE);
//         if (renderer) renderer->fillScreen(bg_color);
//         else DisplayClear();
//       } else {
//         LogBuffer_Init();
//         DisplayInit(DISPLAY_INIT_MODE);
//       }
//     }
//   }
// #endif  // USE_DISPLAY_MODES1TO5
//   ResponseCmndNumber(tkr_set->Settings.display.mode);
// }

// void CmndDisplayDimmer(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
//     tkr_set->Settings.display.dimmer = changeUIntScale(XdrvMailbox.payload, 0, 100, 0, 15);  // Correction for Domoticz (0 - 15)
//     if (tkr_set->Settings.display.dimmer && !(disp_power)) {
//       ExecuteCommandPower(disp_device, POWER_ON, SRC_DISPLAY);
//     }
//     else if (!tkr_set->Settings.display.dimmer && disp_power) {
//       ExecuteCommandPower(disp_device, POWER_OFF, SRC_DISPLAY);
//     }
//     if (renderer) {
//       renderer->dim(tkr_set->Settings.display.dimmer);
//     } else {
//       pCONT->Tasker_Interface(TASK_DISPLAY_DIM);
//     }
//   }
//   ResponseCmndNumber(changeUIntScale(tkr_set->Settings.display.dimmer, 0, 15, 0, 100));
// }

// void CmndDisplaySize(void) {
//   if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= 4)) {
//     tkr_set->Settings.display.size = XdrvMailbox.payload;
//     if (renderer) renderer->setTextSize(tkr_set->Settings.display.size);
//     //else DisplaySetSize(tkr_set->Settings.display.size);
//   }
//   ResponseCmndNumber(tkr_set->Settings.display.size);
// }

// void CmndDisplayFont(void) {
//   if ((XdrvMailbox.payload >=0) && (XdrvMailbox.payload <= 4)) {
//     tkr_set->Settings.display.font = XdrvMailbox.payload;
//     if (renderer) renderer->setTextFont(tkr_set->Settings.display.font);
//     //else DisplaySetFont(tkr_set->Settings.display.font);
//   }
//   ResponseCmndNumber(tkr_set->Settings.display.font);
// }

// void CmndDisplayRotate(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 4)) {
//     if ((tkr_set->Settings.display.rotate) != XdrvMailbox.payload) {
// /*
//       // Needs font info regarding height and width
//       if ((tkr_set->Settings.display.rotate &1) != (XdrvMailbox.payload &1)) {
//         uint8_t temp_rows = tkr_set->Settings.display.rows;
//         tkr_set->Settings.display.rows = tkr_set->Settings.display.cols[0];
//         tkr_set->Settings.display.cols[0] = temp_rows;
// #ifdef USE_DISPLAY_MODES1TO5
//         ScreenBuffer_ReAlloc();
// #endif  // USE_DISPLAY_MODES1TO5
//       }
// */
//       tkr_set->Settings.display.rotate = XdrvMailbox.payload;
//       DisplayInit(DISPLAY_INIT_MODE);
// #ifdef USE_DISPLAY_MODES1TO5
//       LogBuffer_Init();
// #endif  // USE_DISPLAY_MODES1TO5
//     }
//   }
//   ResponseCmndNumber(tkr_set->Settings.display.rotate);
// }

// void CmndDisplayInvert(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     tkr_set->Settings.display.options.invert = XdrvMailbox.payload;
//     if (renderer) renderer->invertDisplay(tkr_set->Settings.display.options.invert);
//   }
//   ResponseCmndNumber(tkr_set->Settings.display.options.invert);
// }

// void CmndDisplayRefresh(void) {
//   if ((XdrvMailbox.payload >= 1) && (XdrvMailbox.payload <= 7)) {
//     tkr_set->Settings.display.refresh = XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(tkr_set->Settings.display.refresh);
// }

// void CmndDisplayColumns(void) {
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 2)) {
//     if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= DISPLAY_MAX_COLS)) {
//       tkr_set->Settings.display.cols[XdrvMailbox.index -1] = XdrvMailbox.payload;
// #ifdef USE_DISPLAY_MODES1TO5
//       if (1 == XdrvMailbox.index) {
//         LogBuffer_Init();
//         ScreenBuffer_ReAlloc();
//       }
// #endif  // USE_DISPLAY_MODES1TO5
//     }
//     ResponseCmndIdxNumber(tkr_set->Settings.display.cols[XdrvMailbox.index -1]);
//   }
// }

// void CmndDisplayRows(void) {
//   if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= DISPLAY_MAX_ROWS)) {
//     tkr_set->Settings.display.rows = XdrvMailbox.payload;
// #ifdef USE_DISPLAY_MODES1TO5
//     LogBuffer_Init();
//     ScreenBuffer_ReAlloc();
// #endif  // USE_DISPLAY_MODES1TO5
//   }
//   ResponseCmndNumber(tkr_set->Settings.display.rows);
// }

// void CmndDisplayAddress(void) {
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 8)) {
//     if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 255)) {
//       tkr_set->Settings.display.address[XdrvMailbox.index -1] = XdrvMailbox.payload;
//     }
//     ResponseCmndIdxNumber(tkr_set->Settings.display.address[XdrvMailbox.index -1]);
//   }
// }

// void CmndDisplayBlinkrate(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
//     if (!renderer) {
//       pCONT->Tasker_Interface(TASK_DISPLAY_BLINKRATE);
//     }
//   }
//   ResponseCmndNumber(XdrvMailbox.payload);
// }

// #ifdef USE_UFILESYS
// void CmndDisplayBatch(void) {
//   if (XdrvMailbox.data_len > 0) {
//     if (!tkr_set->Settings.display.mode) {
//       Display_Text_From_File(XdrvMailbox.data);
//     }
//     ResponseCmndChar(XdrvMailbox.data);
//   }
// }
// #endif

void mDisplaysInterface::CmndDisplayText(const char* buffer) {
  // if (disp_device && XdrvMailbox.data_len > 0) {

// #ifndef USE_DISPLAY_MODES1TO5
//     DisplayText();
// #else
//     if(tkr_set->Settings.display.model == 15) {
//       pCONT->Tasker_Interface(TASK_DISPLAY_SEVENSEG_TEXT);
//     } else if (!tkr_set->Settings.display.mode) {
#ifdef ENABLE_DISPLAY_MODE_USER_TEXT_SERIALISED
      DisplayText(buffer);
#endif // ENABLE_DISPLAY_MODE_USER_TEXT_SERIALISED
    // } else {
    //   LogBuffer_Add(XdrvMailbox.data);
    // }
// #endif  // USE_DISPLAY_MODES1TO5


    // ResponseCmndChar(XdrvMailbox.data);
  // }
}


/**
 * @brief Parsing of direct to display method version 2, using json objects to control effects
 * @param jobj The JsonParserObject from the command
 * */
/**
 * 
{
  "ClearDisplay":1,
  "DrawText":
  [
    {
      "FontSize":1,
      "Position":[0,0],
      "Text":"TextBlock0"
    },
    {
      "FontSize":1,
      "Position":[0,40],
      "Text":"TextBlock1"
    }
  ]
}
 * 
 * */
void mDisplaysInterface::CommandSet_DisplayText_Advanced_JSON(JsonParserObject jobj) {

  JsonParserArray array = jobj["DrawText"];
  JsonParserToken jtok = 0;

  for(auto& object:array)
  {
    JsonParserObject obj1 = object;

    ALOG_DBG(PSTR(D_LOG_LIGHT "obj1=%d" ), obj1.size());
    
    /**
     * @note Change fontsize
     * */
    if(jtok = obj1["TextSize"])
    {
      SetTextSize(jtok.getInt());
    }

    /**
     * @note Position of XY coordinate is always given as array
     * */
    if(jtok = obj1["Position"])
    {
      JsonParserArray arr_pos = obj1["Position"];
      if(arr_pos.size() == 2)
      {
        SetCursor(arr_pos[0].getInt(), arr_pos[1].getInt());
      }
    }

    /**
     * @note This must be last, as previous formatting needs to be applied first
     * */
    if(jtok = obj1["Text"])
    {
      //Draw function
      pCONT_iDisp->renderer->println(jtok.getStr());
    }

    pCONT_iDisp->renderer->Updateframe();

  }



// }else{
//  ALOG_DBG(PSTR(D_LOG_LIGHT "!o2.isArray(" ));

// }






}



// /*********************************************************************************************\
//  * Currently 7-segement specific - should have been handled by (extended) DisplayText command
// \*********************************************************************************************/

// void CmndDisplayClear(void) {
//   if (!renderer)
//     pCONT->Tasker_Interface(TASK_DISPLAY_CLEAR);
//   ResponseCmndChar(XdrvMailbox.data);
// }

// void CmndDisplayNumber(void) {
//   if (!renderer) {
//     pCONT->Tasker_Interface(TASK_DISPLAY_NUMBER);
//   }
//   ResponseCmndChar(XdrvMailbox.data);
// }

// void CmndDisplayFloat(void) {
//   if (!renderer) {
//     pCONT->Tasker_Interface(TASK_DISPLAY_FLOAT);
//   }
//   ResponseCmndChar(XdrvMailbox.data);
// }

// void CmndDisplayNumberNC(void) {
//   if (!renderer) {
//     pCONT->Tasker_Interface(TASK_DISPLAY_NUMBERNC);
//   }
//   ResponseCmndChar(XdrvMailbox.data);
// }

// void CmndDisplayFloatNC(void) {
//   if (!renderer) {
//     pCONT->Tasker_Interface(TASK_DISPLAY_FLOATNC);
//   }
//   ResponseCmndChar(XdrvMailbox.data);
// }

// void CmndDisplayRaw(void) {
//   if (!renderer) {
//     pCONT->Tasker_Interface(TASK_DISPLAY_RAW);
//   }
//   ResponseCmndChar(XdrvMailbox.data);
// }

// void CmndDisplayLevel(void) {
//   bool result = false;
//   if (!renderer) {
//     result = pCONT->Tasker_Interface(TASK_DISPLAY_LEVEL);
//   }
//   if(result) ResponseCmndNumber(XdrvMailbox.payload);
// }

// void CmndDisplaySevensegText(void) {
//   if (!renderer) {
//     pCONT->Tasker_Interface(TASK_DISPLAY_SEVENSEG_TEXT);
//   }
//   ResponseCmndChar(XdrvMailbox.data);
// }

// void CmndDisplayTextNC(void) {
//   if (!renderer) {
//     pCONT->Tasker_Interface(TASK_DISPLAY_SEVENSEG_TEXTNC);
//   }
//   ResponseCmndChar(XdrvMailbox.data);
// }

// void CmndDisplaySevensegTextNC(void) {
//   if (!renderer) {
//     pCONT->Tasker_Interface(TASK_DISPLAY_SEVENSEG_TEXTNC);
//   }
//   ResponseCmndChar(XdrvMailbox.data);
// }

// void CmndDisplayScrollDelay(void) {
//   if (!renderer) {
//     pCONT->Tasker_Interface(TASK_DISPLAY_SCROLLDELAY);
//   }
//   ResponseCmndNumber(XdrvMailbox.payload);
// }

// void CmndDisplayClock(void) {
//   if (!renderer) {
//     pCONT->Tasker_Interface(TASK_DISPLAY_CLOCK);
//   }
//   ResponseCmndNumber(XdrvMailbox.payload);
// }

// void CmndDisplayScrollText(void) {
//   bool result = false;
//   if (!renderer) {
//     result = pCONT->Tasker_Interface(TASK_DISPLAY_SCROLLTEXT);
//   }
//   if(result) ResponseCmndChar(XdrvMailbox.data);
// }






uint8_t mDisplaysInterface::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();

    JBI->Add("model", tkr_set->Settings.display.model);
    JBI->Add("mode", tkr_set->Settings.display.mode);
    JBI->Add("refresh", tkr_set->Settings.display.refresh);
    JBI->Add("rows", tkr_set->Settings.display.rows);
    JBI->Add("cols0", tkr_set->Settings.display.cols[0]);
    JBI->Add("cols1", tkr_set->Settings.display.cols[1]);
    JBI->Add("address0", tkr_set->Settings.display.address[0]);
    JBI->Add("dimmer", tkr_set->Settings.display.dimmer);
    JBI->Add("size", tkr_set->Settings.display.size);
    JBI->Add("font", tkr_set->Settings.display.font);
    JBI->Add("rotate", tkr_set->Settings.display.rotate);

    JBI->Add("width", tkr_set->Settings.display.width);
    JBI->Add("height", tkr_set->Settings.display.height);

  return JBI->End();

}


uint8_t mDisplaysInterface::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_VOLTAGE, 0);
  return JBI->End();
    
}




#ifdef USE_MODULE_NETWORK_MQTT

void mDisplaysInterface::MQTTHandler_Init()
{

  struct handler<mDisplaysInterface>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mDisplaysInterface::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mDisplaysInterface::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mDisplaysInterface::ConstructJSON_Sensor;
  
} 


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mDisplaysInterface::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mDisplaysInterface::MQTTHandler_Rate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
  }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mDisplaysInterface::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT








#endif
