#include "mWebCam.h"

#ifdef USE_MODULE_DRIVERS__CAMERA_TASMOTA

/*********************************************************************************************
 * @SECTION: Interface
**********************************************************************************************/

ESP8266WebServer *Webserver;


int8_t mWebCamera::Tasker(uint8_t function, JsonParserObject obj)
{

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;

    
    // case TASK_WEB_ADD_HANDLER:
    //   WcPicSetup();
    //   break;
    // case TASK_WEB_ADD_MAIN_BUTTON:
    //   WcShowStream();
    //  break;
    // case TASK_EVERY_SECOND:
    //   WcUpdateStats();
    // case TASK_WEB_SENSOR:
    //   WcStatsShow();
    //   break;
    // case TASK_COMMAND:
    //   result = DecodeCommand(kWCCommands, WCCommand);
    //   break;



  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP:
      EveryLoop();
    break;  
    case TASK_EVERY_SECOND:
      EverySecond();
    break;    
    /************
     * WIFI SECTION * 
    *******************/
    case TASK_WIFI_CONNECTED:
    
    break;
    case TASK_WIFI_DISCONNECTED:
    
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
    break; 
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
    break;
    #endif // USE_MODULE_NETWORK_MQTT
  } // end switch
} // END function

void mWebCamera::Pre_Init()
{
  settings.fEnableSensor = false;
}

void mWebCamera::Init()
{
  
  if (!tkr_set->Settings.webcam_config.data) {
    tkr_set->Settings.webcam_config.stream = 1;
    tkr_set->Settings.webcam_config.resolution = FRAMESIZE_QVGA;
    WcSetDefaults(0);
  }
  // previous webcam driver had only a small subset of possible config vars
  // in this case we have to only set the new variables to default values
  if(!tkr_set->Settings.webcam_config2.upgraded) {
    ALOG_DBG(PSTR("CAM: Upgrade settings"));
    WcSetDefaults(1);
    tkr_set->Settings.webcam_config2.upgraded = 1;
  }

  settings.fEnableSensor = true;
}

void mWebCamera::EveryLoop()
{
  
  if (4 == Wc.stream_active) { return; }

  if (Wc.CamServer) {
    Wc.CamServer->handleClient();
    if (Wc.stream_active) { HandleWebcamMjpegTask(); }
  }
  if (wc_motion.motion_detect) { WcDetectMotion(); }

#ifdef ENABLE_RTSPSERVER
    if (tkr_set->Settings.webcam_config.rtsp && !tkr_set->global_state.wifi_down && Wc.up) {
      if (!Wc.rtsp_start) {
        Wc.rtspp = new WiFiServer(8554);
        Wc.rtspp->begin();
        Wc.rtsp_start = 1;
        ALOG_INF(PSTR("CAM: RTSP init"));
        Wc.rtsp_lastframe_time = millis();
      }

      // If we have an active client connection, just service that until gone
      if (Wc.rtsp_session) {
        Wc.rtsp_session->handleRequests(0); // we don't use a timeout here,
        // instead we send only if we have new enough frames

        uint32_t now = millis();
        if ((now-Wc.rtsp_lastframe_time) > RTSP_FRAME_TIME) {
            Wc.rtsp_session->broadcastCurrentFrame(now);
            Wc.rtsp_lastframe_time = now;
          //  ALOG_INF(PSTR("CAM: RTSP session frame"));
        }

        if (Wc.rtsp_session->m_stopped) {
            delete Wc.rtsp_session;
            delete Wc.rtsp_streamer;
            Wc.rtsp_session = NULL;
            Wc.rtsp_streamer = NULL;
            ALOG_INF(PSTR("CAM: RTSP stopped"));
        }
      }
      else {
        Wc.rtsp_client = Wc.rtspp->accept();
        if (Wc.rtsp_client) {
            Wc.rtsp_streamer = new OV2640Streamer(&Wc.rtsp_client, Wc.cam);        // our streamer for UDP/TCP based RTP transport
            Wc.rtsp_session = new CRtspSession(&Wc.rtsp_client, Wc.rtsp_streamer); // our threads RTSP session and state
            ALOG_INF(PSTR("CAM: RTSP stream created"));
        }
      }
    }
#endif // ENABLE_RTSPSERVER
}

void mWebCamera::EverySecond()
{
  
}






/*********************************************************************************************/

void mWebCamera::WcInterrupt(uint32_t state) {
  // Stop camera ISR if active to fix TG1WDT_SYS_RESET
  if (!Wc.up) { return; }

  if (state) {
    // Re-enable interrupts
    cam_start();
  } else {
    // Stop interrupts
    cam_stop();
  }
}

bool WcPinUsed(void) {
  bool pin_used = true;
  for (uint32_t i = 0; i < MAX_WEBCAM_DATA; i++) {
    if (!pCONT_pins->PinUsed(GPIO_WEBCAM_DATA1_ID, i)) {
      pin_used = false;
    }
//    if (i < MAX_WEBCAM_HSD) {
//      if (!pCONT_pins->(GPIO_WEBCAM_HSD, i)) {
//        pin_used = false;
//      }
//    }
  }
  if (!pCONT_pins->(GPIO_WEBCAM_XCLK_ID) || !pCONT_pins->(GPIO_WEBCAM_PCLK_ID) ||
      !pCONT_pins->(GPIO_WEBCAM_VSYNC_ID) || !pCONT_pins->(GPIO_WEBCAM_HREF_ID) ||
      ((!pCONT_pins->(GPIO_WEBCAM_SIOD_ID) || !pCONT_pins->(GPIO_WEBCAM_SIOC_ID)) && !tkr_set->i2c_enabled_2)    // preferred option is to reuse and share I2Cbus 2
      ) {
        pin_used = false;
  }
  return pin_used;
}

void mWebCamera::WcFeature(int32_t value) {
  sensor_t * wc_s = esp_camera_sensor_get();
  if (!wc_s) { return; }

  if (value != 1) {
      // CLKRC: Set Clock Divider to 0 = fullspeed
      wc_s->set_reg(wc_s, 0x111, 0x3f, 0x00);
      vTaskDelay(200 / portTICK_PERIOD_MS);
  }
  if (value != 2) {
      // Stop Nightmode
      wc_s->set_reg(wc_s, 0x103, 0xff, 0x0a);   // COM1: Reset dummy frames
      wc_s->set_reg(wc_s, 0x10f, 0xff, 0x43);   // Reserved Reg
      wc_s->set_reg(wc_s, 0x10f, 0xff, 0x4b);   // Reserved Reg
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      wc_s->set_reg(wc_s, 0x10f, 0xff, 0x43);   // Reserved Reg
  }

  switch(value) {
    case 1:
      // Reduce FPS
      // CLKRC: Set Clock Divider to 2
      wc_s->set_reg(wc_s, 0x111, 0x3f, 0x02);
      break;
    case 2:
      // Start Nightmode
      wc_s->set_reg(wc_s, 0x10f, 0xff, 0x4b);   // Reserved Reg
      wc_s->set_reg(wc_s, 0x103, 0xff, 0xcf);   // COM1: Allow 7 dummy frames
      break;
  }
  ALOG_DBG(PSTR("CAM: Feature: %d"), value);
}

void mWebCamera::WcApplySettings() {
  sensor_t * wc_s = esp_camera_sensor_get();
  if (!wc_s) { return; }

  wc_s->set_vflip(wc_s, tkr_set->Settings.webcam_config.flip);
  wc_s->set_hmirror(wc_s, tkr_set->Settings.webcam_config.mirror);

  wc_s->set_brightness(wc_s, tkr_set->Settings.webcam_config.brightness - 2);
  wc_s->set_saturation(wc_s, tkr_set->Settings.webcam_config.saturation - 2);
  wc_s->set_contrast(wc_s, tkr_set->Settings.webcam_config.contrast - 2);

  wc_s->set_special_effect(wc_s, tkr_set->Settings.webcam_config2.special_effect);

  wc_s->set_whitebal(wc_s, tkr_set->Settings.webcam_config.awb);
  wc_s->set_wb_mode(wc_s, tkr_set->Settings.webcam_config2.wb_mode);
  wc_s->set_awb_gain(wc_s, tkr_set->Settings.webcam_config.awb_gain);

  wc_s->set_exposure_ctrl(wc_s, tkr_set->Settings.webcam_config.aec);
  wc_s->set_aec_value(wc_s, tkr_set->Settings.webcam_config2.aec_value - 2);
  wc_s->set_ae_level(wc_s, tkr_set->Settings.webcam_config2.ae_level);
  wc_s->set_aec2(wc_s, tkr_set->Settings.webcam_config.aec2);

  wc_s->set_gain_ctrl(wc_s, tkr_set->Settings.webcam_config.agc);
  wc_s->set_agc_gain(wc_s, tkr_set->Settings.webcam_config2.agc_gain);
  wc_s->set_gainceiling(wc_s, (gainceiling_t)tkr_set->Settings.webcam_config2.gainceiling);

  wc_s->set_raw_gma(wc_s, tkr_set->Settings.webcam_config.raw_gma);
  wc_s->set_lenc(wc_s, tkr_set->Settings.webcam_config.lenc);
  wc_s->set_wpc(wc_s, tkr_set->Settings.webcam_config.wpc);
  wc_s->set_dcw(wc_s, tkr_set->Settings.webcam_config.dcw);
  wc_s->set_bpc(wc_s, tkr_set->Settings.webcam_config.bpc);

  WcFeature(tkr_set->Settings.webcam_config.feature);

  ALOG_DBG(PSTR("CAM: Settings updated"));
}

void mWebCamera::WcSetDefaults(uint32_t upgrade) {
  if (!upgrade) {
    tkr_set->Settings.webcam_config.flip = 0;
    tkr_set->Settings.webcam_config.mirror = 0;

    tkr_set->Settings.webcam_config.saturation = 2; // = 0
    tkr_set->Settings.webcam_config.brightness = 2; // = 0
    tkr_set->Settings.webcam_config.contrast = 2;   // = 0
  }

  tkr_set->Settings.webcam_config2.special_effect = 0;
  tkr_set->Settings.webcam_config.colorbar = 0;

  tkr_set->Settings.webcam_config.awb = 1;        // white balance
  tkr_set->Settings.webcam_config2.wb_mode = 0;   // white balance mode
  tkr_set->Settings.webcam_config.awb_gain = 1;   // white blance gain

  tkr_set->Settings.webcam_config.aec = 1;          // autoexposure (sensor)
  tkr_set->Settings.webcam_config.aec2 = 1;         // autoexposure (dsp)
  tkr_set->Settings.webcam_config2.ae_level = 2;    // autoexposure level (-2 - +2, default 0)
  tkr_set->Settings.webcam_config2.aec_value = 204; // manual exposure value

  tkr_set->Settings.webcam_config.agc = 1;          // auto gain control
  tkr_set->Settings.webcam_config2.agc_gain = 5;    // manual gain control
  tkr_set->Settings.webcam_config2.gainceiling = 0; // auto gain ceiling

  tkr_set->Settings.webcam_config.raw_gma = 1;      // gamma correct
  tkr_set->Settings.webcam_config.lenc = 1;         // lens correction
  tkr_set->Settings.webcam_config.wpc = 1;          // white pixel correct
  tkr_set->Settings.webcam_config.dcw = 1;          // downsize en
  tkr_set->Settings.webcam_config.bpc = 0;          // black pixel correct?

  tkr_set->Settings.webcam_config.feature = 0;

  ALOG_DBG(PSTR("CAM: Defaults set"));

  if (Wc.up) { WcApplySettings(); }
}

uint32_t WcSetup(int32_t fsiz) {
  if (fsiz >= FRAMESIZE_FHD) { fsiz = FRAMESIZE_FHD - 1; }

  Wc.stream_active = 0;

  if (fsiz < 0) {
    esp_camera_deinit();
    Wc.up = 0;
    return 0;
  }

  if (Wc.up) {
    esp_camera_deinit();
    ALOG_DBG(PSTR("CAM: Deinit"));
    //return Wc.up;
  }
  Wc.up = 0;

//esp_log_level_set("*", ESP_LOG_VERBOSE);

  camera_config_t config;

  if (WcPinUsed()) {
    config.pin_d0 = Pin(GPIO_WEBCAM_DATA1_ID);        // Y2_GPIO_NUM;
    config.pin_d1 = Pin(GPIO_WEBCAM_DATA1_ID, 1);     // Y3_GPIO_NUM;
    config.pin_d2 = Pin(GPIO_WEBCAM_DATA1_ID, 2);     // Y4_GPIO_NUM;
    config.pin_d3 = Pin(GPIO_WEBCAM_DATA1_ID, 3);     // Y5_GPIO_NUM;
    config.pin_d4 = Pin(GPIO_WEBCAM_DATA1_ID, 4);     // Y6_GPIO_NUM;
    config.pin_d5 = Pin(GPIO_WEBCAM_DATA1_ID, 5);     // Y7_GPIO_NUM;
    config.pin_d6 = Pin(GPIO_WEBCAM_DATA1_ID, 6);     // Y8_GPIO_NUM;
    config.pin_d7 = Pin(GPIO_WEBCAM_DATA1_ID, 7);     // Y9_GPIO_NUM;
    config.pin_xclk = Pin(GPIO_WEBCAM_XCLK_ID);      // XCLK_GPIO_NUM;
    config.pin_pclk = Pin(GPIO_WEBCAM_PCLK_ID);      // PCLK_GPIO_NUM;
    config.pin_vsync = Pin(GPIO_WEBCAM_VSYNC_ID);    // VSYNC_GPIO_NUM;
    config.pin_href = Pin(GPIO_WEBCAM_HREF_ID);      // HREF_GPIO_NUM;
    config.pin_sccb_sda = Pin(GPIO_WEBCAM_SIOD_ID);  // SIOD_GPIO_NUM; - unset to use shared I2C bus 2
    config.pin_sccb_scl = Pin(GPIO_WEBCAM_SIOC_ID);  // SIOC_GPIO_NUM;
    // My fix, just use port 1 for now
    // if(tkr_set->i2c_enabled_2){              // configure SIOD and SIOC as SDA,2 and SCL,2
    //   config.sccb_i2c_port = 1;                   // reuse initialized bus 2, can be shared now
    //   if(config.pin_sccb_sda < 0){                // GPIO_WEBCAM_SIOD must not be set to really make it happen
    //     ALOG_INF(PSTR("CAM: use I2C bus 2"));
    //   }
    // }
    config.pin_pwdn = Pin(GPIO_WEBCAM_PWDN_ID);       // PWDN_GPIO_NUM;
    config.pin_reset = Pin(GPIO_WEBCAM_RESET_ID);    // RESET_GPIO_NUM;
    ALOG_DBG(PSTR("CAM: Template pin config"));
  } else if (Y2_GPIO_NUM != -1) {
    // Modell is set in camera_pins.h
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    ALOG_DBG(PSTR("CAM: Compile flag pin config"));
  } else {
    // no valid config found -> abort
    ALOG_DBG(PSTR("CAM: No pin config"));
    return 0;
}

  int32_t ledc_channel = analogAttach(config.pin_xclk);
  if (ledc_channel < 0) {
    AddLog(LOG_LEVEL_ERROR, "CAM: cannot allocated ledc channel, remove a PWM GPIO");
  }
  config.ledc_channel = (ledc_channel_t) ledc_channel;
  AddLog(LOG_LEVEL_DEBUG_MORE, "CAM: XCLK on GPIO %i using ledc channel %i", config.pin_xclk, config.ledc_channel);
  config.ledc_timer = LEDC_TIMER_0;
//  config.xclk_freq_hz = 20000000;
  config.xclk_freq_hz = tkr_set->Settings.webcam_clk * 1000000;
  config.pixel_format = PIXFORMAT_JPEG;

  //esp_log_level_set("*", ESP_LOG_INFO);

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.

  bool psram = true; // ASSUME TRUE FOR NOW //UsePSRAM(); //added in "tas/support_esp" to be merged later
  if (psram) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
    ALOG_DBG(PSTR("CAM: PSRAM found"));
  } else {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
    config.fb_location = CAMERA_FB_IN_DRAM;
    ALOG_DBG(PSTR("CAM: PSRAM not found"));
  }

  esp_err_t err = esp_camera_init(&config);

  if (err != ESP_OK) {
    ALOG_INF(PSTR("CAM: Init failed with error 0x%x"), err);
    return 0;
  }

//  ALOG_INF(PSTR("CAM: heap check 2: %d"),ESP_getFreeHeap());

  sensor_t * wc_s = esp_camera_sensor_get();
  // drop down frame size for higher initial frame rate
  wc_s->set_framesize(wc_s, (framesize_t)fsiz);

  camera_fb_t *wc_fb = esp_camera_fb_get();
  if (!wc_fb) {
    ALOG_INF(PSTR("CAM: Init failed to get the frame on time"));
    return 0;
  }
  Wc.width = wc_fb->width;
  Wc.height = wc_fb->height;
  esp_camera_fb_return(wc_fb);

  WcApplySettings();

  camera_sensor_info_t *info = esp_camera_sensor_get_info(&wc_s->id);

  ALOG_INF(PSTR("CAM: %s Initialized"), info->name);


  Wc.up = 1;
  if (psram) { Wc.up = 2; }

  return Wc.up;
}

/*********************************************************************************************/

int32_t WcSetOptions(uint32_t sel, int32_t value) {
  int32_t res = 0;
  sensor_t *s = esp_camera_sensor_get();
  if (!s) { return -99; }

  switch (sel) {
    case 0:
      if (value >= 0) { s->set_framesize(s, (framesize_t)value); }
      res = s->status.framesize;

      // WcFeature is lost on resolution change
      WcApplySettings();
      break;
    case 1:
      if (value >= 0) { s->set_special_effect(s, value); }
      res = s->status.special_effect;
      break;
    case 2:
      if (value >= 0) { s->set_vflip(s, value); }
      res = s->status.vflip;
      break;
    case 3:
      if (value >= 0) { s->set_hmirror(s, value); }
      res = s->status.hmirror;
      break;
    case 4:
      if (value >= -4) { s->set_contrast(s, value); }
      res = s->status.contrast;
      break;
    case 5:
      if (value >= -4) { s->set_brightness(s, value); }
      res = s->status.brightness;
      break;
    case 6:
      if (value >= -4) { s->set_saturation(s,value); }
      res = s->status.saturation;
      break;
    case 7:
      if (value >= 0) { s->set_whitebal(s, value); }
      res = s->status.awb;
      break;
    case 8:
      if (value >= 0) { s->set_wb_mode(s, value); }
      res = s->status.wb_mode;
      break;
    case 9:
      if (value >= 0) { s->set_awb_gain(s, value); }
      res = s->status.awb_gain;
      break;
    case 10:
      if (value >= 0) { s->set_exposure_ctrl(s, value); }
      res = s->status.aec;
      break;
    case 11:
      if (value >= 0) { s->set_aec_value(s, value); }
      res = s->status.aec_value;
      break;
    case 12:
      if (value >= 0) { s->set_ae_level(s, value); }
      res = s->status.ae_level;
      break;
    case 13:
      if (value >= 0) { s->set_aec2(s, value); }
      res = s->status.aec2;
      break;
    case 14:
      if (value >= 0) { s->set_gain_ctrl(s, value); }
      res = s->status.agc;
      break;
    case 15:
      if (value >= 0) { s->set_agc_gain(s, value); }
      res = s->status.agc_gain;
      break;
    case 16:
      if (value >= 0) { s->set_gainceiling(s, (gainceiling_t)value); }
      res = s->status.gainceiling;
      break;
    case 17:
      if (value >= 0) { s->set_raw_gma(s, value); }
      res = s->status.raw_gma;
      break;
    case 18:
      if (value >= 0) { s->set_lenc(s, value); }
      res = s->status.lenc;
      break;
    case 19:
      if (value >= 0) { s->set_wpc(s, value); }
      res = s->status.wpc;
      break;
    case 20:
      if (value >= 0) { s->set_dcw(s, value); }
      res = s->status.dcw;
      break;
    case 21:
      // blackpixelcontrol
      if (value >= 0) { s->set_bpc(s, value); }
      res = s->status.bpc;
      break;
    case 22:
      if (value >= 0) { s->set_colorbar(s, value); }
      res = s->status.colorbar;
      break;
    case 23:
      if (value >= 0) { WcFeature(value); }
      break;
  }

  return res;
}

uint32_t WcGetWidth(void) {
  camera_fb_t *wc_fb = esp_camera_fb_get();
  if (!wc_fb) { return 0; }
  Wc.width = wc_fb->width;
  esp_camera_fb_return(wc_fb);
  return Wc.width;
}

uint32_t WcGetHeight(void) {
  camera_fb_t *wc_fb = esp_camera_fb_get();
  if (!wc_fb) { return 0; }
  Wc.height = wc_fb->height;
  esp_camera_fb_return(wc_fb);
  return Wc.height;
}

/*********************************************************************************************/

struct WC_Motion {
uint16_t motion_detect;
uint32_t motion_ltime;
uint32_t motion_trigger;
uint32_t motion_brightness;
uint8_t *last_motion_buffer;
} wc_motion;


uint32_t WcSetMotionDetect(int32_t value) {
  if (value >= 0) { wc_motion.motion_detect = value; }
  if (-1 == value) {
    return wc_motion.motion_trigger;
  } else  {
    return wc_motion.motion_brightness;
  }
}

// optional motion detector
void mWebCamera::WcDetectMotion(void) {
  camera_fb_t *wc_fb;
  uint8_t *out_buf = 0;

  if ((millis()-wc_motion.motion_ltime) > wc_motion.motion_detect) {
    wc_motion.motion_ltime = millis();
    wc_fb = esp_camera_fb_get();
    if (!wc_fb) { return; }

    if (!wc_motion.last_motion_buffer) {
      wc_motion.last_motion_buffer = (uint8_t *)heap_caps_malloc((wc_fb->width*wc_fb->height) + 4, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    }
    if (wc_motion.last_motion_buffer) {
      if (PIXFORMAT_JPEG == wc_fb->format) {
        out_buf = (uint8_t *)heap_caps_malloc((wc_fb->width*wc_fb->height*3)+4, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        if (out_buf) {
          fmt2rgb888(wc_fb->buf, wc_fb->len, wc_fb->format, out_buf);
          uint32_t x, y;
          uint8_t *pxi = out_buf;
          uint8_t *pxr = wc_motion.last_motion_buffer;
          // convert to bw
          uint64_t accu = 0;
          uint64_t bright = 0;
          for (y = 0; y < wc_fb->height; y++) {
            for (x = 0; x < wc_fb->width; x++) {
              int32_t gray = (pxi[0] + pxi[1] + pxi[2]) / 3;
              int32_t lgray = pxr[0];
              pxr[0] = gray;
              pxi += 3;
              pxr++;
              accu += abs(gray - lgray);
              bright += gray;
            }
          }
          wc_motion.motion_trigger = accu / ((wc_fb->height * wc_fb->width) / 100);
          wc_motion.motion_brightness = bright / ((wc_fb->height * wc_fb->width) / 100);
          free(out_buf);
        }
      }
    }
    esp_camera_fb_return(wc_fb);
  }
}

#ifdef COPYFRAME
struct PICSTORE tmp_picstore;
#endif

uint32_t WcGetPicstore(int32_t num, uint8_t **buff) {
  if (num<0) { return MAX_PICSTORE; }
  *buff = Wc.picstore[num].buff;
  return Wc.picstore[num].len;
}

uint32_t WcGetFrame(int32_t bnum) {
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  camera_fb_t *wc_fb = 0;
  bool jpeg_converted = false;

  if (bnum < 0) {
    if (bnum < -MAX_PICSTORE) { bnum=-1; }
    bnum = -bnum;
    bnum--;
    if (Wc.picstore[bnum].buff) { free(Wc.picstore[bnum].buff); }
    Wc.picstore[bnum].len = 0;
    return 0;
  }

#ifdef COPYFRAME
  if (bnum & 0x10) {
    bnum &= 0xf;
    _jpg_buf = tmp_picstore.buff;
    _jpg_buf_len = tmp_picstore.len;
    if (!_jpg_buf_len) { return 0; }
    goto pcopy;
  }
#endif

  wc_fb = esp_camera_fb_get();
  if (!wc_fb) {
    ALOG_DBG(PSTR("CAM: Can't get frame"));
    return 0;
  }
  if (!bnum) {
    Wc.width = wc_fb->width;
    Wc.height = wc_fb->height;
    esp_camera_fb_return(wc_fb);
    return 0;
  }

  if (wc_fb->format != PIXFORMAT_JPEG) {
    jpeg_converted = frame2jpg(wc_fb, 80, &_jpg_buf, &_jpg_buf_len);
    if (!jpeg_converted){
      //Serial.println("JPEG compression failed");
      _jpg_buf_len = wc_fb->len;
      _jpg_buf = wc_fb->buf;
    }
  } else {
    _jpg_buf_len = wc_fb->len;
    _jpg_buf = wc_fb->buf;
  }

pcopy:
  if ((bnum < 1) || (bnum > MAX_PICSTORE)) { bnum = 1; }
  bnum--;
  if (Wc.picstore[bnum].buff) { free(Wc.picstore[bnum].buff); }
  Wc.picstore[bnum].buff = (uint8_t *)heap_caps_malloc(_jpg_buf_len+4, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  if (Wc.picstore[bnum].buff) {
    memcpy(Wc.picstore[bnum].buff, _jpg_buf, _jpg_buf_len);
    Wc.picstore[bnum].len = _jpg_buf_len;
  } else {
    ALOG_DBG(PSTR("CAM: Can't allocate picstore"));
    Wc.picstore[bnum].len = 0;
  }
  if (wc_fb) { esp_camera_fb_return(wc_fb); }
  if (jpeg_converted) { free(_jpg_buf); }
  if (!Wc.picstore[bnum].buff) { return 0; }

  return  _jpg_buf_len;
}

//////////////// Handle authentication /////////////////


bool WebcamAuthenticate(void)
{
  if (strlen(SettingsText(SET_WEBPWD)) && (HTTP_MANAGER_RESET_ONLY != Web.state)) {
    return Wc.CamServer->authenticate(WEB_USERNAME, SettingsText(SET_WEBPWD));
  } else {
    return true;
  }
}

bool WebcamCheckPriviledgedAccess(bool autorequestauth = true)
{

  if(tkr_set->Settings.webcam_config2.auth == 0){
    return true;
  }

  if (autorequestauth && !WebcamAuthenticate()) {
    Wc.CamServer->requestAuthentication();
    return false;
  }
  return true;
}

///////////////////////////////////////////////////

void mWebCamera::HandleImage(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  uint32_t bnum = Webserver->arg(F("p")).toInt();
  if ((bnum < 0) || (bnum > MAX_PICSTORE)) { bnum= 1; }
  WiFiClient client = Webserver->client();
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-disposition: inline; filename=cap.jpg\r\n";
  response += "Content-type: image/jpeg\r\n\r\n";
  Webserver->sendContent(response);

  if (!bnum) {
    size_t _jpg_buf_len = 0;
    uint8_t * _jpg_buf = NULL;
    camera_fb_t *wc_fb = 0;
    wc_fb = esp_camera_fb_get();
    if (!wc_fb) { return; }
    if (Wc.stream_active < 2) {
      // fetch some more frames
      esp_camera_fb_return(wc_fb);
      wc_fb = esp_camera_fb_get();
      esp_camera_fb_return(wc_fb);
      wc_fb = esp_camera_fb_get();
    }
    if (wc_fb->format != PIXFORMAT_JPEG) {
      bool jpeg_converted = frame2jpg(wc_fb, 80, &_jpg_buf, &_jpg_buf_len);
      if (!jpeg_converted) {
        _jpg_buf_len = wc_fb->len;
        _jpg_buf = wc_fb->buf;
      }
    } else {
      _jpg_buf_len = wc_fb->len;
      _jpg_buf = wc_fb->buf;
    }
    if (_jpg_buf_len) {
      client.write((char *)_jpg_buf, _jpg_buf_len);
    }
    if (wc_fb) { esp_camera_fb_return(wc_fb); }
  } else {
    bnum--;
    if (!Wc.picstore[bnum].len) {
      ALOG_DBG(PSTR("CAM: No image #: %d"), bnum);
      return;
    }
    client.write((char *)Wc.picstore[bnum].buff, Wc.picstore[bnum].len);
  }
  client.stop();

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("CAM: Sending image #: %d"), bnum+1);
}

void mWebCamera::HandleImageBasic(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP "Capture image"));

  if (tkr_set->Settings.webcam_config.stream) {
    if (!Wc.CamServer) {
      WcInterruptControl();
    }
  }

  camera_fb_t *wc_fb;
  wc_fb = esp_camera_fb_get();  // Acquire frame
  if (!wc_fb) {
    ALOG_DBG(PSTR("CAM: Frame buffer could not be acquired"));
    return;
  }

  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  if (wc_fb->format != PIXFORMAT_JPEG) {
    bool jpeg_converted = frame2jpg(wc_fb, 80, &_jpg_buf, &_jpg_buf_len);
    if (!jpeg_converted) {
      _jpg_buf_len = wc_fb->len;
      _jpg_buf = wc_fb->buf;
    }
  } else {
    _jpg_buf_len = wc_fb->len;
    _jpg_buf = wc_fb->buf;
  }

  if (_jpg_buf_len) {
    Webserver->client().flush();
    WSHeaderSend();
    Webserver->sendHeader(F("Content-disposition"), F("inline; filename=snapshot.jpg"));
    Webserver->send_P(200, "image/jpeg", (char *)_jpg_buf, _jpg_buf_len);
    Webserver->client().stop();
  }

  esp_camera_fb_return(wc_fb);  // Free frame buffer

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("CAM: Image sent"));
}

void mWebCamera::HandleWebcamMjpeg(void) {
  if(!WebcamCheckPriviledgedAccess()){
    Wc.CamServer->send(403,"","");
    return;
  }
  ALOG_DBG(PSTR("CAM: Handle camserver"));
//  if (!Wc.stream_active) {
// always restart stream
    Wc.stream_active = 1;
    Wc.client = Wc.CamServer->client();
    ALOG_DBG(PSTR("CAM: Create client"));
//  }
}

void mWebCamera::HandleWebcamMjpegTask(void) {
  camera_fb_t *wc_fb;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;

  //WiFiClient client = CamServer->client();
  uint32_t tlen;
  bool jpeg_converted = false;

  if (!Wc.client.connected()) {
    ALOG_DBG(PSTR("CAM: Client fail"));
    Wc.stream_active = 0;
    WcStats.clientfail++;
  }
  if (1 == Wc.stream_active) {
    Wc.client.flush();
    Wc.client.setTimeout(3);
    ALOG_DBG(PSTR("CAM: Start stream"));
    Wc.client.print("HTTP/1.1 200 OK\r\n"
      "Content-Type: multipart/x-mixed-replace;boundary=" BOUNDARY "\r\n"
      "\r\n");
    Wc.stream_active = 2;
  }
  if (2 == Wc.stream_active) {
    wc_fb = esp_camera_fb_get();
    if (!wc_fb) {
      ALOG_DBG(PSTR("CAM: Frame fail"));
      Wc.stream_active = 0;
      WcStats.camfail++;
    }
    WcStats.camcnt++;
  }
  if (2 == Wc.stream_active) {
    if (wc_fb->format != PIXFORMAT_JPEG) {
      jpeg_converted = frame2jpg(wc_fb, 80, &_jpg_buf, &_jpg_buf_len);
      if (!jpeg_converted){
        ALOG_DBG(PSTR("CAM: JPEG compression failed"));
        _jpg_buf_len = wc_fb->len;
        _jpg_buf = wc_fb->buf;
        WcStats.jpegfail++;
      }
    } else {
      _jpg_buf_len = wc_fb->len;
      _jpg_buf = wc_fb->buf;
    }

    Wc.client.print("--" BOUNDARY "\r\n");
    Wc.client.printf("Content-Type: image/jpeg\r\n"
      "Content-Length: %d\r\n"
      "\r\n", static_cast<int>(_jpg_buf_len));
    tlen = Wc.client.write(_jpg_buf, _jpg_buf_len);
    /*
    if (tlen!=_jpg_buf_len) {
      esp_camera_fb_return(wc_fb);
      Wc.stream_active=0;
      ALOG_DBG(PSTR("CAM: Send fail"));
    }*/
//    Wc.client.print("\r\n--" BOUNDARY "\r\n");
    Wc.client.print("\r\n");

#ifdef COPYFRAME
    if (tmp_picstore.buff) { free(tmp_picstore.buff); }
    tmp_picstore.buff = (uint8_t *)heap_caps_malloc(_jpg_buf_len+4, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (tmp_picstore.buff) {
      memcpy(tmp_picstore.buff, _jpg_buf, _jpg_buf_len);
      tmp_picstore.len = _jpg_buf_len;
    } else {
      tmp_picstore.len = 0;
    }
#endif

    if (jpeg_converted) { free(_jpg_buf); }
    esp_camera_fb_return(wc_fb);
    //ALOG_DBG(PSTR("CAM: send frame"));
  }
  if (0 == Wc.stream_active) {
    ALOG_DBG(PSTR("CAM: Stream exit"));
    Wc.client.flush();
    Wc.client.stop();
  }
}

void mWebCamera::HandleWebcamRoot(void) {
  #ifdef ENABLE_DEVFEATURE_CAMERA_TASMOTA_INCLUDE_WEBSERVER
  if(!WebcamCheckPriviledgedAccess()){
    Wc.CamServer->send(403,"","");
    return;
  }
  //CamServer->redirect("http://" + String(ip) + ":81/cam.mjpeg");
  Wc.CamServer->sendHeader("Location", "/cam.mjpeg");
  Wc.CamServer->send(302, "", "");
  #endif // ENABLE_DEVFEATURE_CAMERA_TASMOTA_INCLUDE_WEBSERVER
  ALOG_DBG(PSTR("CAM: Root called"));
}

/*********************************************************************************************/

uint32_t WcSetStreamserver(uint32_t flag) {
  if (tkr_set->global_state.network_down) { return 0; }

  Wc.stream_active = 0;

  #ifdef ENABLE_DEVFEATURE_CAMERA_TASMOTA_INCLUDE_WEBSERVER
  if (flag) {
    if (!Wc.CamServer) {
      Wc.CamServer = new ESP8266WebServer(81);
      Wc.CamServer->on("/", HandleWebcamRoot);
      Wc.CamServer->on("/cam.mjpeg", HandleWebcamMjpeg);
      Wc.CamServer->on("/cam.jpg", HandleWebcamMjpeg);
      Wc.CamServer->on("/stream", HandleWebcamMjpeg);
      ALOG_DBG(PSTR("CAM: Stream init"));
      Wc.CamServer->begin();
    }
  } else {
    if (Wc.CamServer) {
      Wc.CamServer->stop();
      delete Wc.CamServer;
      Wc.CamServer = NULL;
      ALOG_DBG(PSTR("CAM: Stream exit"));
    }
  }
  
  #endif // ENABLE_DEVFEATURE_CAMERA_TASMOTA_INCLUDE_WEBSERVER

  return 0;
}

void mWebCamera::WcInterruptControl() {
  WcSetStreamserver(tkr_set->Settings.webcam_config.stream);
  WcSetup(tkr_set->Settings.webcam_config.resolution);
}

/*********************************************************************************************/


void mWebCamera::WcPicSetup(void) {
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  WebServer_on(PSTR("/wc.jpg"), HandleImage);
  WebServer_on(PSTR("/wc.mjpeg"), HandleImage);
  WebServer_on(PSTR("/snapshot.jpg"), HandleImage);
  #endif 
}

void mWebCamera::WcShowStream(void) {
  
  #ifdef ENABLE_DEVFEATURE_CAMERA_TASMOTA_INCLUDE_WEBSERVER
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  if (tkr_set->Settings.webcam_config.stream) {
//    if (!Wc.CamServer || !Wc.up) {
    if (!Wc.CamServer) {
      WcInterruptControl();
      delay(50);   // Give the webcam webserver some time to prepare the stream
    }
    if (Wc.CamServer && Wc.up) {
      WSContentSend_P(PSTR("<p></p><center><img src='http://%_I:81/stream' alt='Webcam stream' style='width:99%%;'></center><p></p>"),
        (uint32_t)WiFi.localIP());
    }
  }
  
  #endif // USE_MODULE_NETWORK_WEBSERVER
  #endif // ENABLE_DEVFEATURE_CAMERA_TASMOTA_INCLUDE_WEBSERVER

}


/*********************************************************************************************\
 * Commands
\*********************************************************************************************/


void mWebCamera::CmndWebcam(void) {
//   Response_P(PSTR("{\"" D_PRFX_WEBCAM "\":{\"" D_CMND_WC_STREAM "\":%d,\"" D_CMND_WC_RESOLUTION "\":%d,\"" D_CMND_WC_MIRROR "\":%d,\""
//     D_CMND_WC_FLIP "\":%d,\""
//     D_CMND_WC_SATURATION "\":%d,\"" D_CMND_WC_BRIGHTNESS "\":%d,\"" D_CMND_WC_CONTRAST "\":%d,\""
//     D_CMND_WC_SPECIALEFFECT "\":%d,\"" D_CMND_WC_AWB  "\":%d,\"" D_CMND_WC_WB_MODE "\":%d,\""
//     D_CMND_WC_AWB_GAIN "\":%d,\"" D_CMND_WC_AEC "\":%d,\"" D_CMND_WC_AEC_VALUE "\":%d,\""
//     D_CMND_WC_AE_LEVEL "\":%d,\"" D_CMND_WC_AEC2 "\":%d,\"" D_CMND_WC_AGC "\":%d,\""
//     D_CMND_WC_AGC_GAIN "\":%d,\"" D_CMND_WC_GAINCEILING "\":%d,\"" D_CMND_WC_RAW_GMA "\":%d,\""
//     D_CMND_WC_LENC "\":%d,\"" D_CMND_WC_WPC "\":%d,\"" D_CMND_WC_DCW "\":%d,\"" D_CMND_WC_BPC "\":%d,\""
//     D_CMND_WC_COLORBAR "\":%d,\"" D_CMND_WC_FEATURE "\":%d,\"" D_CMND_WC_AUTH "\":%d"
// #ifdef ENABLE_RTSPSERVER
//   ",\"" D_CMND_RTSP "\":%d"
// #endif // ENABLE_RTSPSERVER
//   "}}"),
//     tkr_set->Settings.webcam_config.stream, tkr_set->Settings.webcam_config.resolution, tkr_set->Settings.webcam_config.mirror,
//     tkr_set->Settings.webcam_config.flip,
//     tkr_set->Settings.webcam_config.saturation -2, tkr_set->Settings.webcam_config.brightness -2, tkr_set->Settings.webcam_config.contrast -2,
//     tkr_set->Settings.webcam_config2.special_effect, tkr_set->Settings.webcam_config.awb, tkr_set->Settings.webcam_config2.wb_mode,
//     tkr_set->Settings.webcam_config.awb_gain, tkr_set->Settings.webcam_config.aec, tkr_set->Settings.webcam_config2.aec_value,
//     tkr_set->Settings.webcam_config2.ae_level -2, tkr_set->Settings.webcam_config.aec2, tkr_set->Settings.webcam_config.agc,
//     tkr_set->Settings.webcam_config2.agc_gain, tkr_set->Settings.webcam_config2.gainceiling, tkr_set->Settings.webcam_config.raw_gma,
//     tkr_set->Settings.webcam_config.lenc, tkr_set->Settings.webcam_config.wpc, tkr_set->Settings.webcam_config.dcw, tkr_set->Settings.webcam_config.bpc,
//     tkr_set->Settings.webcam_config.colorbar, tkr_set->Settings.webcam_config.feature, tkr_set->Settings.webcam_config2.auth
// #ifdef ENABLE_RTSPSERVER
//   , tkr_set->Settings.webcam_config.rtsp
// #endif // ENABLE_RTSPSERVER
//   );
}

void mWebCamera::CmndWebcamStream(void) {
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
  //   tkr_set->Settings.webcam_config.stream = XdrvMailbox.payload;
  //   if (!tkr_set->Settings.webcam_config.stream) { WcInterruptControl(); }  // Stop stream
  // }
  // ResponseCmndStateText(tkr_set->Settings.webcam_config.stream);
}

void mWebCamera::CmndWebcamResolution(void) {
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < FRAMESIZE_FHD)) {
  //   tkr_set->Settings.webcam_config.resolution = XdrvMailbox.payload;
  //   WcSetOptions(0, tkr_set->Settings.webcam_config.resolution);
  // }
  // ResponseCmndNumber(tkr_set->Settings.webcam_config.resolution);
}

void mWebCamera::CmndWebcamMirror(void) {
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
  //   tkr_set->Settings.webcam_config.mirror = XdrvMailbox.payload;
  //   WcSetOptions(3, tkr_set->Settings.webcam_config.mirror);
  // }
  // ResponseCmndStateText(tkr_set->Settings.webcam_config.mirror);
}

void mWebCamera::CmndWebcamFlip(void) {
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
  //   tkr_set->Settings.webcam_config.flip = XdrvMailbox.payload;
  //   WcSetOptions(2, tkr_set->Settings.webcam_config.flip);
  // }
  // ResponseCmndStateText(tkr_set->Settings.webcam_config.flip);
}

void mWebCamera::CmndWebcamSaturation(void) {
  // if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
  //   tkr_set->Settings.webcam_config.saturation = XdrvMailbox.payload +2;
  //   WcSetOptions(6, tkr_set->Settings.webcam_config.saturation -2);
  // }
  // ResponseCmndNumber(tkr_set->Settings.webcam_config.saturation -2);
}

void mWebCamera::CmndWebcamBrightness(void) {
  // if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
  //   tkr_set->Settings.webcam_config.brightness = XdrvMailbox.payload +2;
  //   WcSetOptions(5, tkr_set->Settings.webcam_config.brightness -2);
  // }
  // ResponseCmndNumber(tkr_set->Settings.webcam_config.brightness -2);
}

void mWebCamera::CmndWebcamContrast(void) {
  // if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
  //   tkr_set->Settings.webcam_config.contrast = XdrvMailbox.payload +2;
  //   WcSetOptions(4, tkr_set->Settings.webcam_config.contrast -2);
  // }
  // ResponseCmndNumber(tkr_set->Settings.webcam_config.contrast -2);
}

void mWebCamera::CmndWebcamSpecialEffect(void) {
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 6)) {
  //   tkr_set->Settings.webcam_config2.special_effect = XdrvMailbox.payload;
  //   WcSetOptions(1, tkr_set->Settings.webcam_config2.special_effect);
  // }
  // ResponseCmndNumber(tkr_set->Settings.webcam_config2.special_effect);
}

void mWebCamera::CmndWebcamAWB(void) {
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
  //   tkr_set->Settings.webcam_config.awb = XdrvMailbox.payload;
  //   WcSetOptions(7, tkr_set->Settings.webcam_config.awb);
  // }
  // ResponseCmndStateText(tkr_set->Settings.webcam_config.awb);
}

void mWebCamera::CmndWebcamWBMode(void) {
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 4)) {
  //   tkr_set->Settings.webcam_config2.wb_mode = XdrvMailbox.payload;
  //   WcSetOptions(8, tkr_set->Settings.webcam_config2.wb_mode);
  // }
  // ResponseCmndNumber(tkr_set->Settings.webcam_config2.wb_mode);
}

void mWebCamera::CmndWebcamAWBGain(void) {
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
  //   tkr_set->Settings.webcam_config.awb_gain = XdrvMailbox.payload;
  //   WcSetOptions(9, tkr_set->Settings.webcam_config.awb_gain);
  // }
  // ResponseCmndStateText(tkr_set->Settings.webcam_config.awb_gain);
}

void mWebCamera::CmndWebcamAEC(void) {
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
  //   tkr_set->Settings.webcam_config.aec = XdrvMailbox.payload;
  //   WcSetOptions(10, tkr_set->Settings.webcam_config.aec);
  // }
  // ResponseCmndStateText(tkr_set->Settings.webcam_config.aec);
}

void mWebCamera::CmndWebcamAECValue(void) {
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1200)) {
  //   tkr_set->Settings.webcam_config2.aec_value = XdrvMailbox.payload;
  //   WcSetOptions(11, tkr_set->Settings.webcam_config2.aec_value);
  // }
  // ResponseCmndNumber(tkr_set->Settings.webcam_config2.aec_value);
}

void mWebCamera::CmndWebcamAELevel(void) {
  // if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
  //   tkr_set->Settings.webcam_config2.ae_level = XdrvMailbox.payload + 2;
  //   WcSetOptions(12, tkr_set->Settings.webcam_config2.ae_level - 2);
  // }
  // ResponseCmndNumber(tkr_set->Settings.webcam_config2.ae_level - 2);
}

void mWebCamera::CmndWebcamAEC2(void) {
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
  //   tkr_set->Settings.webcam_config.aec2 = XdrvMailbox.payload;
  //   WcSetOptions(13, tkr_set->Settings.webcam_config.aec2);
  // }
  // ResponseCmndStateText(tkr_set->Settings.webcam_config.aec2);
}

void mWebCamera::CmndWebcamAGC(void) {
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
  //   tkr_set->Settings.webcam_config.agc = XdrvMailbox.payload;
  //   WcSetOptions(14, tkr_set->Settings.webcam_config.agc);
  // }
  // ResponseCmndStateText(tkr_set->Settings.webcam_config.agc);
}

void mWebCamera::CmndWebcamAGCGain(void) {
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 30)) {
  //   tkr_set->Settings.webcam_config2.agc_gain = XdrvMailbox.payload;
  //   WcSetOptions(15, tkr_set->Settings.webcam_config2.agc_gain);
  // }
  // ResponseCmndNumber(tkr_set->Settings.webcam_config2.agc_gain);
}

void mWebCamera::CmndWebcamGainCeiling(void) {
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 6)) {
  //   tkr_set->Settings.webcam_config2.gainceiling = XdrvMailbox.payload;
  //   WcSetOptions(16, tkr_set->Settings.webcam_config2.gainceiling);
  // }
  // ResponseCmndNumber(tkr_set->Settings.webcam_config2.gainceiling);
}

void mWebCamera::CmndWebcamGammaCorrect(void) {
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
  //   tkr_set->Settings.webcam_config.raw_gma = XdrvMailbox.payload;
  //   WcSetOptions(17, tkr_set->Settings.webcam_config.raw_gma);
  // }
  // ResponseCmndStateText(tkr_set->Settings.webcam_config.raw_gma);
}

void mWebCamera::CmndWebcamLensCorrect(void) {
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
  //   tkr_set->Settings.webcam_config.lenc = XdrvMailbox.payload;
  //   WcSetOptions(18, tkr_set->Settings.webcam_config.lenc);
  // }
  // ResponseCmndStateText(tkr_set->Settings.webcam_config.lenc);
}

void mWebCamera::CmndWebcamWPC(void) {
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
  //   tkr_set->Settings.webcam_config.wpc = XdrvMailbox.payload;
  //   WcSetOptions(19, tkr_set->Settings.webcam_config.wpc);
  // }
  // ResponseCmndStateText(tkr_set->Settings.webcam_config.wpc);
}

void mWebCamera::CmndWebcamDCW(void) {
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
  //   tkr_set->Settings.webcam_config.dcw = XdrvMailbox.payload;
  //   WcSetOptions(20, tkr_set->Settings.webcam_config.dcw);
  // }
  // ResponseCmndStateText(tkr_set->Settings.webcam_config.dcw);
}

void mWebCamera::CmndWebcamBPC(void) {
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
  //   tkr_set->Settings.webcam_config.bpc = XdrvMailbox.payload;
  //   WcSetOptions(21, tkr_set->Settings.webcam_config.bpc);
  // }
  // ResponseCmndStateText(tkr_set->Settings.webcam_config.bpc);
}

void mWebCamera::CmndWebcamColorbar(void) {
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
  //   tkr_set->Settings.webcam_config.colorbar = XdrvMailbox.payload;
  //   WcSetOptions(22, tkr_set->Settings.webcam_config.colorbar);
  // }
  // ResponseCmndStateText(tkr_set->Settings.webcam_config.colorbar);
}

void mWebCamera::CmndWebcamFeature(void) {
  // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
  //   tkr_set->Settings.webcam_config.feature = XdrvMailbox.payload;
  //   WcSetOptions(23, tkr_set->Settings.webcam_config.feature);
  // }
  // ResponseCmndNumber(tkr_set->Settings.webcam_config.feature);
}

void mWebCamera::CmndWebcamAuth(void){
  // if((XdrvMailbox.payload >=0) && (XdrvMailbox.payload <= 1)){
  //   tkr_set->Settings.webcam_config2.auth = XdrvMailbox.payload;
  // }
  // ResponseCmndNumber(tkr_set->Settings.webcam_config2.auth);
}

void mWebCamera::CmndWebcamClock(void){
//   if((XdrvMailbox.payload >= 10) && (XdrvMailbox.payload <= 200)){
//     tkr_set->Settings.webcam_clk = XdrvMailbox.payload;
//     WcInterruptControl();
//   }
//   ResponseCmndNumber(tkr_set->Settings.webcam_clk);
}

void mWebCamera::CmndWebcamInit(void) {
  WcInterruptControl();
  ResponseCmndDone();
}

void mWebCamera::CmndWebcamSetDefaults(void) {
  WcSetDefaults(0);
  ResponseCmndDone();
}


void mWebCamera::CmndWebcamStats(void) {
  // Response_P(PSTR("{\"" D_PRFX_WEBCAM D_CMND_WC_STATS "\":{\"" D_WEBCAM_STATS_FPS "\":%d,\""
  //   D_WEBCAM_STATS_CAMFAIL "\":%d,\"" D_WEBCAM_STATS_JPEGFAIL "\":%d,\"" D_WEBCAM_STATS_CLIENTFAIL "\":%d"
  // "}}"),
  // WcStats.camfps, WcStats.camfail, WcStats.jpegfail, WcStats.clientfail
  // );
}

#ifdef ENABLE_RTSPSERVER
void mWebCamera::CmndWebRtsp(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    tkr_set->Settings.webcam_config.rtsp = XdrvMailbox.payload;
    tkr_set->restart_flag = 2;
  }
  ResponseCmndStateText(tkr_set->Settings.webcam_config.rtsp);
}
#endif // ENABLE_RTSPSERVER

void mWebCamera::WcUpdateStats(void) {
  WcStats.camfps = WcStats.camcnt;
  WcStats.camcnt = 0;
}

// const char HTTP_WEBCAM_FPS[] PROGMEM = "{s}%s " D_FRAME_RATE "{m}%d " D_UNIT_FPS  "{e}";

void mWebCamera::WcStatsShow(void) {
#ifdef USE_WEBSERVER
  WSContentSend_PD(HTTP_WEBCAM_FPS, WcStats.name, WcStats.camfps);
#endif  // USE_WEBSERVER
}



































/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mWebCamera::parse_JSONCommand(JsonParserObject obj)
{

}

/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mWebCamera::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add("data", 1);
  return JBI->End();

}

uint8_t mWebCamera::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add("data", 1);
  return JBI->End();

}
  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mWebCamera::MQTTHandler_Init()
{

  struct handler<mWebCamera>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mWebCamera::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mWebCamera::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mWebCamera::ConstructJSON_Sensor;
  
} 

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mWebCamera::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mWebCamera::MQTTHandler_Rate()
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
void mWebCamera::MQTTHandler_Sender()
{    
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}
  
#endif // USE_MODULE_NETWORK_MQTT


#endif // USE_MODULE_DRIVERS_CAMERA_TASMOTA
