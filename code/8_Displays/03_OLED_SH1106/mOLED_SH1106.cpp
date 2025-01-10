/*
  xdsp_02_ssd1306.ino - Display Oled SH1106 support for Tasmota

  Copyright (C) 2021  Theo Arends and Adafruit

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
#include "mOLED_SH1106.h"

#ifdef USE_MODULE_DISPLAYS_OLED_SH1106

int8_t mOLED_SH1106::Tasker(uint8_t function, JsonParserObject obj)
{
  
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
    case TASK_DISPLAY_INIT_DRIVER:
      InitDriver();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }
  if(!pCONT_iDisp->renderer) { return FUNCTION_RESULT_ERROR_POINTER_INVALID_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:
      EverySecond();
    break;
    case TASK_DISPLAY_REFRESH_SHOW_ID:
      RefreshDisplay();
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
  }
  
  return function_result;

} // END function


void mOLED_SH1106::Pre_Init(void)
{
  if (pCONT_i2c->I2cEnabled(XI2C_04))
  { 
    settings.fEnableSensor = true;
  }
}


void mOLED_SH1106::Init(void)
{
  
}


void mOLED_SH1106::EverySecond(void)
{
  /**
   * RefreshDisplay
   * */
  RefreshDisplay();
}

// void mOLED_SH1106::EveryLoop()
// {
//   if(mTime::TimeReached(&tSaved))
// RefreshDisplay();

// }



void mOLED_SH1106::RefreshDisplay()
{

  switch (tkr_set->Settings.display.mode) {
    default:
    case EM_DISPLAY_MODE_LOG_SCROLLING_ID:
      ShowScrollingLog();
    break;
    case EM_DISPLAY_MODE_LOG_STATIC_ID:
      ShowStaticLog();
    break;
    case EM_DISPLAY_MODE_USER_TEXT_SERIALISED_ID: 
    case EM_DISPLAY_MODE_USER_TEXT_ADVANCED_ID:
      // Refresh not needed, drawn directly on execution
    break;
    case EM_DISPLAY_MODE_UTC_TIME_ID:
      ShowUTCTime();
    break;
  }


}


void mOLED_SH1106::InitDriver(void)
{
  if (!tkr_set->runtime.i2c_enabled) 
  {
    return; 
  }

  if (!tkr_set->Settings.display.model)
  {
    if (pCONT_i2c->I2cSetDevice(OLED_ADDRESS1))
    {
      tkr_set->Settings.display.address[0] = OLED_ADDRESS1;
      tkr_set->Settings.display.model = D_GROUP_MODULE_DISPLAYS_OLED_SH1106_ID;
    }
    else if (pCONT_i2c->I2cSetDevice(OLED_ADDRESS2))
    {
      tkr_set->Settings.display.address[0] = OLED_ADDRESS2;
      tkr_set->Settings.display.model = D_GROUP_MODULE_DISPLAYS_OLED_SH1106_ID;
    }
  }

  ALOG_INF(PSTR("DSP: SD1306 address[0] %d"),tkr_set->Settings.display.address[0]);
  
  if(tkr_set->Settings.display.model == D_GROUP_MODULE_DISPLAYS_OLED_SH1106_ID)
  {
    pCONT_i2c->I2cSetActiveFound(tkr_set->Settings.display.address[0], "SH1106");

    if(
      (tkr_set->Settings.display.width != 64) && 
      (tkr_set->Settings.display.width != 96) && 
      (tkr_set->Settings.display.width != 128))
    {
      tkr_set->Settings.display.width = 128;
    }
    if(
      (tkr_set->Settings.display.height != 16) && 
      (tkr_set->Settings.display.height != 32) && 
      (tkr_set->Settings.display.height != 48) && 
      (tkr_set->Settings.display.height != 64)
    ){
      tkr_set->Settings.display.height = 64;
    }

    oled1106 = new Adafruit_SH1106(tkr_set->Settings.display.width, tkr_set->Settings.display.height, pCONT_i2c->wire);
    oled1106->begin(SH1106_SWITCHCAPVCC, tkr_set->Settings.display.address[0], pCONT_pins->Pin(GPIO_OLED_RESET_ID) >= 0);
    pCONT_iDisp->renderer = oled1106;
    pCONT_iDisp->renderer->DisplayInit(pCONT_iDisp->DISPLAY_INIT_MODE, tkr_set->Settings.display.size, tkr_set->Settings.display.rotate, tkr_set->Settings.display.font);
    pCONT_iDisp->renderer->setTextColor(1,0);

    #ifdef SHOW_SPLASH
    pCONT_iDisp->renderer->setTextFont(0);
    pCONT_iDisp->renderer->setTextSize(2);
    pCONT_iDisp->renderer->setCursor(20,20);
    pCONT_iDisp->renderer->println(F("SH1106"));
    pCONT_iDisp->renderer->Updateframe();
    pCONT_iDisp->renderer->DisplayOnff(true);
    #endif

    ALOG_INF(PSTR("DSP: SD1306"));
  }

}


/*********************************************************************************************/
#ifdef USE_DISPLAY_MODES1TO5

/**
 * @brief Will take from log_buffer and fill screen_buffer, shifting the rows up
 * */
void mOLED_SH1106::ShowScrollingLog(void)
{
  
  pCONT_iDisp->disp_refresh--;
  if (!pCONT_iDisp->disp_refresh) {
    pCONT_iDisp->disp_refresh = tkr_set->Settings.display.refresh;

    // If no columns have been init, then first allocate memory
    if (!pCONT_iDisp->screen_buffer.cols) { pCONT_iDisp->ScreenBuffer_Alloc(); }

    // Get pointer to row
    char* txt = pCONT_iDisp->LogBuffer_GetRowPointer('\370');
    if (txt != NULL) {
      // Last row is row_size - 1 for indexing
      uint8_t last_row = tkr_set->Settings.display.rows -1;

      // Start by clearing the display
      pCONT_iDisp->renderer->clearDisplay();
      pCONT_iDisp->renderer->setTextSize(tkr_set->Settings.display.size);
      pCONT_iDisp->renderer->setCursor(0,0);

      // Shift the logs by moving the rows from next into current and display this
      for (byte i = 0; i < last_row; i++) {
        strlcpy(pCONT_iDisp->screen_buffer.ptr[i], pCONT_iDisp->screen_buffer.ptr[i +1], pCONT_iDisp->screen_buffer.cols);
        pCONT_iDisp->renderer->println(pCONT_iDisp->screen_buffer.ptr[i]);
      }
      // Add new row
      strlcpy(pCONT_iDisp->screen_buffer.ptr[last_row], txt, pCONT_iDisp->screen_buffer.cols);
      // This is only done here, as the copied rows will have it done at this line when first commited to the screen_buffer
      pCONT_iDisp->ScreenBuffer_SetUnusedRowCharsToSpaceChar(last_row); 

      ALOG_DBG(PSTR(D_LOG_DEBUG "[%s]"), pCONT_iDisp->screen_buffer.ptr[last_row]);

      // Print last row
      pCONT_iDisp->renderer->println(pCONT_iDisp->screen_buffer.ptr[last_row]);

      // Commit display refresh
      pCONT_iDisp->renderer->Updateframe();
    }
  }
}


/**
 * @brief Unlike AppendingLog method, this will only commit all of log_buffer into screen_buffer and display
 * */
void mOLED_SH1106::ShowStaticLog(void)
{
  
  // If no columns have been init, then first allocate memory
  if (!pCONT_iDisp->screen_buffer.cols) { pCONT_iDisp->ScreenBuffer_Alloc(); }

  // Start by clearing the display
  pCONT_iDisp->renderer->clearDisplay();
  pCONT_iDisp->renderer->setTextSize(tkr_set->Settings.display.size);
  pCONT_iDisp->renderer->setCursor(0,0);

  // Copy log_buffer contents into screen_buffer
  for(int row_index=0; row_index<tkr_set->Settings.display.rows; row_index++)
  {
    // Get log_buffer by row
    char* row_ptr = pCONT_iDisp->LogBuffer_GetRowPointerByRowIndex(row_index);
    // Move to screen_buffer
    strlcpy(pCONT_iDisp->screen_buffer.ptr[row_index], row_ptr, pCONT_iDisp->screen_buffer.cols);
    // Fill remaining spaces
    pCONT_iDisp->ScreenBuffer_SetUnusedRowCharsToSpaceChar(row_index); 
    // Write row to display
    pCONT_iDisp->renderer->println(pCONT_iDisp->screen_buffer.ptr[row_index]);
  }

  // Commit display refresh
  pCONT_iDisp->renderer->Updateframe();

}


void mOLED_SH1106::ShowUTCTime(void)
{

  char line[12];
  pCONT_iDisp->renderer->clearDisplay();
  pCONT_iDisp->renderer->setTextSize(tkr_set->Settings.display.size);
  pCONT_iDisp->renderer->setTextFont(tkr_set->Settings.display.font);
  pCONT_iDisp->renderer->setCursor(0, 0);
  // snprintf_P(line, sizeof(line), PSTR(" %02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), tkr_time->RtcTime.hour,  tkr_time->RtcTime.minute,  tkr_time->RtcTime.second);  // [ 12:34:56 ]
  pCONT_iDisp->renderer->println( tkr_time->GetTime().c_str() );
  pCONT_iDisp->renderer->println(tkr_time->GetUptime());
  // snprintf_P(line, sizeof(line), PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%04d"),  tkr_time->RtcTime.Mday,  tkr_time->RtcTime.month,  tkr_time->RtcTime.year);   // [01-02-2018]
  // pCONT_iDisp->renderer->println(line);
  pCONT_iDisp->renderer->Updateframe();

}


#endif  // USE_DISPLAY_MODES1TO5


#endif // USE_MODULE_DISPLAYS_OLED_SH1106

