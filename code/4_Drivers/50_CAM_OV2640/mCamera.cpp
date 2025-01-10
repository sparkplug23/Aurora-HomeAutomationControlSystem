

#include "1_TaskerManager/mTaskerManager.h"

// unknown source

#ifdef USE_MODULE_DRIVERS_CAMERA_OV2640


/*

  This is a simple MJPEG streaming webserver implemented for AI-Thinker ESP32-CAM
  and ESP-EYE modules.
  This is tested to work with VLC and Blynk video widget and can support up to 10
  simultaneously connected streaming clients.
  Simultaneous streaming is implemented with FreeRTOS tasks.

  Inspired by and based on this Instructable: $9 RTSP Video Streamer Using the ESP32-CAM Board
  (https://www.instructables.com/id/9-RTSP-Video-Streamer-Using-the-ESP32-CAM-Board/)

  Board: AI-Thinker ESP32-CAM or ESP-EYE
  Compile as:
   ESP32 Dev Module
   CPU Freq: 240
   Flash Freq: 80
   Flash mode: QIO
   Flash Size: 4Mb
   Patrition: Minimal SPIFFS
   PSRAM: Enabled
*/


// ESP32 has two cores: APPlication core and PROcess core (the one that runs ESP32 SDK stack)
#define APP_CPU 1
#define PRO_CPU 0

#include "internal/OV2640.h"
#include <WiFi.h>
// #include <WebServer.h>
#include <WiFiClient.h>

#include "ESP8266WebServer.h"

#include <esp_bt.h>
#include <esp_wifi.h>
#include <esp_sleep.h>
#include <driver/rtc_io.h>

// Select camera model
//#define CAMERA_MODEL_WROVER_KIT
//#define CAMERA_MODEL_ESP_EYE
//#define CAMERA_MODEL_M5STACK_PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE
#define CAMERA_MODEL_AI_THINKER

// #include "camera_pins.h"




/*********************************************************************************************\
 * ESP32 webcam based on example in Arduino-ESP32 library
 *
 * Template as used on ESP32-CAM WiFi + bluetooth Camera Module Development Board ESP32 With Camera Module OV2640 Geekcreit for Arduino
 * {"NAME":"AITHINKER CAM","GPIO":[4992,1,1,1,1,5088,1,1,1,1,1,1,1,1,5089,5090,0,5091,5184,5152,0,5120,5024,5056,0,0,0,0,4928,1,5094,5095,5092,0,0,5093],"FLAG":0,"BASE":1}
 *
 * Supported commands:
 * WcStream     = Control streaming, 0 = stop, 1 = start
 * WcResolution = Set resolution
 0 = FRAMESIZE_96X96,    // 96x96
 1 = FRAMESIZE_QQVGA,    // 160x120
 2 = FRAMESIZE_QCIF,     // 176x144
 3 = FRAMESIZE_HQVGA,    // 240x176
 4 = FRAMESIZE_240X240,  // 240x240
 5 = FRAMESIZE_QVGA,     // 320x240
 6 = FRAMESIZE_CIF,      // 400x296
 7 = FRAMESIZE_HVGA,     // 480x320
 8 = FRAMESIZE_VGA,      // 640x480
 9 = FRAMESIZE_SVGA,     // 800x600
 10 = FRAMESIZE_XGA,      // 1024x768
 11 = FRAMESIZE_HD,       // 1280x720
 12 = FRAMESIZE_SXGA,     // 1280x1024
 13 = FRAMESIZE_UXGA,     // 1600x1200
 // 3MP Sensors above this no yet supported with this driver
 14 = FRAMESIZE_FHD,      // 1920x1080
 15 = FRAMESIZE_P_HD,     //  720x1280
 16 = FRAMESIZE_P_3MP,    //  864x1536
 17 = FRAMESIZE_QXGA,     // 2048x1536
 // 5MP Sensors
 18 = FRAMESIZE_QHD,      // 2560x1440
 19 = FRAMESIZE_WQXGA,    // 2560x1600
 20 = FRAMESIZE_P_FHD,    // 1080x1920
 21 = FRAMESIZE_QSXGA,    // 2560x1920
 22 = FRAMESIZE_INVALID

 * WcMirror     = Mirror picture, 0 = no, 1 = yes
 * WcFlip       = Flip picture, 0 = no, 1 = yes
 * WcSaturation = Set picture Saturation -2 ... +2
 * WcBrightness = Set picture Brightness -2 ... +2
 * WcContrast   = Set picture Contrast -2 ... +2
 * WcInit       = Init Camera Interface
 * WcRtsp       = Control RTSP Server, 0=disable, 1=enable (forces restart) (if defined ENABLE_RTSPSERVER)
 *
 * Only boards with PSRAM should be used. To enable PSRAM board should be se set to esp32cam in common32 of platform_override.ini
 * board                   = esp32cam
 * To speed up cam processing cpu frequency should be better set to 240Mhz in common32 of platform_override.ini
 * board_build.f_cpu       = 240000000L
 * remarks for AI-THINKER
 * GPIO0 zero must be disconnected from any wire after programming because this pin drives the cam clock and does
 * not tolerate any capictive load
 * flash led = gpio 4
 * red led = gpio 33
 * optional rtsp url: rtsp://xxx.xxx.xxx.xxx:8554/mjpeg/1
 */

/*********************************************************************************************/

#define XDRV_81           81

#include "esp_camera.h"
#include "sensor.h"
#include "fb_gfx.h"
#include "fd_forward.h"
#include "fr_forward.h"

bool HttpCheckPriviledgedAccess(bool);
// extern ESP8266WebServer *Webserver;
// extern mWebServer *Webserver;

// #define BOUNDARY "e8b8c539-047d-4777-a985-fbba6edff11e"


//TASMOTA
// CAMERA_MODEL_AI_THINKER default template pins
// #define PWDN_GPIO_NUM     32
// #define RESET_GPIO_NUM    -1
// #define XCLK_GPIO_NUM      0
// #define SIOD_GPIO_NUM     26
// #define SIOC_GPIO_NUM     27

// #define Y9_GPIO_NUM       35
// #define Y8_GPIO_NUM       34
// #define Y7_GPIO_NUM       39
// #define Y6_GPIO_NUM       36
// #define Y5_GPIO_NUM       21
// #define Y4_GPIO_NUM       19
// #define Y3_GPIO_NUM       18
// #define Y2_GPIO_NUM        5
// #define VSYNC_GPIO_NUM    25
// #define HREF_GPIO_NUM     23
// #define PCLK_GPIO_NUM     22

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22


#ifndef MAX_PICSTORE
#define MAX_PICSTORE 4
#endif
struct PICSTORE {
  uint8_t *buff;
  uint32_t len;
};

#ifdef ENABLE_RTSPSERVER
#include <OV2640.h>
#include <SimStreamer.h>
#include <OV2640Streamer.h>
#include <CRtspSession.h>
#ifndef RTSP_FRAME_TIME
#define RTSP_FRAME_TIME 100
#endif // RTSP_FRAME_TIME
#endif // ENABLE_RTSPSERVER

struct {
  uint8_t  up;
  uint16_t width;
  uint16_t height;
  uint8_t  stream_active;
  WiFiClient client;
  ESP8266WebServer *CamServer;
  // WebServer* CamServer;
  struct PICSTORE picstore[MAX_PICSTORE];
#ifdef USE_FACE_DETECT
  uint8_t  faces;
  uint16_t face_detect_time;
  uint32_t face_ltime;
  mtmn_config_t mtmn_config = {0};
#endif // USE_FACE_DETECT
#ifdef ENABLE_RTSPSERVER
  WiFiServer *rtspp;
  CStreamer *rtsp_streamer;
  CRtspSession *rtsp_session;
  WiFiClient rtsp_client;
  uint8_t rtsp_start;
  OV2640 cam;
  uint32_t rtsp_lastframe_time;
#endif // ENABLE_RTSPSERVER
} Wc;



// /*********************************************************************************************/

// bool WcPinUsed(void) {
//   bool pin_used = true;
//   for (uint32_t i = 0; i < MAX_WEBCAM_DATA; i++) {
//     if (!PinUsed(GPIO_WEBCAM_DATA, i)) {
//       pin_used = false;
//     }
// //    if (i < MAX_WEBCAM_HSD) {
// //      if (!PinUsed(GPIO_WEBCAM_HSD, i)) {
// //        pin_used = false;
// //      }
// //    }
//   }
//   if (!PinUsed(GPIO_WEBCAM_XCLK) || !PinUsed(GPIO_WEBCAM_PCLK) ||
//       !PinUsed(GPIO_WEBCAM_VSYNC) || !PinUsed(GPIO_WEBCAM_HREF) ||
//       !PinUsed(GPIO_WEBCAM_SIOD) || !PinUsed(GPIO_WEBCAM_SIOC)) {
//     pin_used = false;
//   }
//   return pin_used;
// }

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
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
//  config.pixel_format = PIXFORMAT_GRAYSCALE;
//  config.pixel_format = PIXFORMAT_RGB565;

  // if (WcPinUsed()) {
  //   config.pin_d0 = Pin(GPIO_WEBCAM_DATA);        // Y2_GPIO_NUM;
  //   config.pin_d1 = Pin(GPIO_WEBCAM_DATA, 1);     // Y3_GPIO_NUM;
  //   config.pin_d2 = Pin(GPIO_WEBCAM_DATA, 2);     // Y4_GPIO_NUM;
  //   config.pin_d3 = Pin(GPIO_WEBCAM_DATA, 3);     // Y5_GPIO_NUM;
  //   config.pin_d4 = Pin(GPIO_WEBCAM_DATA, 4);     // Y6_GPIO_NUM;
  //   config.pin_d5 = Pin(GPIO_WEBCAM_DATA, 5);     // Y7_GPIO_NUM;
  //   config.pin_d6 = Pin(GPIO_WEBCAM_DATA, 6);     // Y8_GPIO_NUM;
  //   config.pin_d7 = Pin(GPIO_WEBCAM_DATA, 7);     // Y9_GPIO_NUM;
  //   config.pin_xclk = Pin(GPIO_WEBCAM_XCLK);      // XCLK_GPIO_NUM;
  //   config.pin_pclk = Pin(GPIO_WEBCAM_PCLK);      // PCLK_GPIO_NUM;
  //   config.pin_vsync = Pin(GPIO_WEBCAM_VSYNC);    // VSYNC_GPIO_NUM;
  //   config.pin_href = Pin(GPIO_WEBCAM_HREF);      // HREF_GPIO_NUM;
  //   config.pin_sscb_sda = Pin(GPIO_WEBCAM_SIOD);  // SIOD_GPIO_NUM;
  //   config.pin_sscb_scl = Pin(GPIO_WEBCAM_SIOC);  // SIOC_GPIO_NUM;
  //   config.pin_pwdn = Pin(GPIO_WEBCAM_PWDN);       // PWDN_GPIO_NUM;
  //   config.pin_reset = Pin(GPIO_WEBCAM_RESET);    // RESET_GPIO_NUM;

  //   ALOG_DBG(PSTR("CAM: User template"));
  // } else {
    // defaults to AI THINKER
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
    ALOG_DBG(PSTR("CAM: Default template"));
  // }

  //ESP.getPsramSize()

  //esp_log_level_set("*", ESP_LOG_INFO);

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.

  bool psram = psramFound();
  if (psram) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
    ALOG_DBG(PSTR("CAM: PSRAM found"));
  } else {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
    ALOG_DBG(PSTR("CAM: PSRAM not found"));
  }

//  ALOG_INF(PSTR("CAM: heap check 1: %d"),ESP_getFreeHeap());

  // stupid workaround camera diver eats up static ram should prefer PSRAM
  // so we steal static ram to force driver to alloc PSRAM
//  uint32_t maxfram = ESP.getMaxAllocHeap();
//  void *x=malloc(maxfram-4096);
  void *x = 0;
  esp_err_t err = esp_camera_init(&config);
  if (x) { free(x); }

  if (err != ESP_OK) {
    ALOG_INF(PSTR("CAM: Init failed with error 0x%x"), err);
    return 0;
  }

//  ALOG_INF(PSTR("CAM: heap check 2: %d"),ESP_getFreeHeap());

  sensor_t * wc_s = esp_camera_sensor_get();

  wc_s->set_vflip(wc_s, tkr_set->Settings.webcam_config.flip);
  wc_s->set_hmirror(wc_s, tkr_set->Settings.webcam_config.mirror);
  wc_s->set_brightness(wc_s, tkr_set->Settings.webcam_config.brightness -2);  // up the brightness just a bit
  wc_s->set_saturation(wc_s, tkr_set->Settings.webcam_config.saturation -2);  // lower the saturation
  wc_s->set_contrast(wc_s, tkr_set->Settings.webcam_config.contrast -2);      // keep contrast

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

#ifdef USE_FACE_DETECT
  fd_init();
#endif

  ALOG_INF(PSTR("CAM: Initialized"));

  Wc.up = 1;
  if (psram) { Wc.up = 2; }

  return Wc.up;
}

// /*********************************************************************************************/

// int32_t WcSetOptions(uint32_t sel, int32_t value) {
//   int32_t res = 0;
//   sensor_t *s = esp_camera_sensor_get();
//   if (!s) { return -99; }

//   switch (sel) {
//     case 0:
//       if (value >= 0) { s->set_framesize(s, (framesize_t)value); }
//       res = s->status.framesize;
//       break;
//     case 1:
//       if (value >= 0) { s->set_special_effect(s, value); }
//       res = s->status.special_effect;
//       break;
//     case 2:
//       if (value >= 0) { s->set_vflip(s, value); }
//       res = s->status.vflip;
//       break;
//     case 3:
//       if (value >= 0) { s->set_hmirror(s, value); }
//       res = s->status.hmirror;
//       break;
//     case 4:
//       if (value >= -4) { s->set_contrast(s, value); }
//       res = s->status.contrast;
//       break;
//     case 5:
//       if (value >= -4) { s->set_brightness(s, value); }
//       res = s->status.brightness;
//       break;
//     case 6:
//       if (value >= -4) { s->set_saturation(s,value); }
//       res = s->status.saturation;
//       break;
//   }

//   return res;
// }

// uint32_t WcGetWidth(void) {
//   camera_fb_t *wc_fb = esp_camera_fb_get();
//   if (!wc_fb) { return 0; }
//   Wc.width = wc_fb->width;
//   esp_camera_fb_return(wc_fb);
//   return Wc.width;
// }

// uint32_t WcGetHeight(void) {
//   camera_fb_t *wc_fb = esp_camera_fb_get();
//   if (!wc_fb) { return 0; }
//   Wc.height = wc_fb->height;
//   esp_camera_fb_return(wc_fb);
//   return Wc.height;
// }

// /*********************************************************************************************/

// struct WC_Motion {
// uint16_t motion_detect;
// uint32_t motion_ltime;
// uint32_t motion_trigger;
// uint32_t motion_brightness;
// uint8_t *last_motion_buffer;
// } wc_motion;


// uint32_t WcSetMotionDetect(int32_t value) {
//   if (value >= 0) { wc_motion.motion_detect = value; }
//   if (-1 == value) {
//     return wc_motion.motion_trigger;
//   } else  {
//     return wc_motion.motion_brightness;
//   }
// }

// // optional motion detector
// void WcDetectMotion(void) {
//   camera_fb_t *wc_fb;
//   uint8_t *out_buf = 0;

//   if ((millis()-wc_motion.motion_ltime) > wc_motion.motion_detect) {
//     wc_motion.motion_ltime = millis();
//     wc_fb = esp_camera_fb_get();
//     if (!wc_fb) { return; }

//     if (!wc_motion.last_motion_buffer) {
//       wc_motion.last_motion_buffer = (uint8_t *)heap_caps_malloc((wc_fb->width*wc_fb->height) + 4, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
//     }
//     if (wc_motion.last_motion_buffer) {
//       if (PIXFORMAT_JPEG == wc_fb->format) {
//         out_buf = (uint8_t *)heap_caps_malloc((wc_fb->width*wc_fb->height*3)+4, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
//         if (out_buf) {
//           fmt2rgb888(wc_fb->buf, wc_fb->len, wc_fb->format, out_buf);
//           uint32_t x, y;
//           uint8_t *pxi = out_buf;
//           uint8_t *pxr = wc_motion.last_motion_buffer;
//           // convert to bw
//           uint64_t accu = 0;
//           uint64_t bright = 0;
//           for (y = 0; y < wc_fb->height; y++) {
//             for (x = 0; x < wc_fb->width; x++) {
//               int32_t gray = (pxi[0] + pxi[1] + pxi[2]) / 3;
//               int32_t lgray = pxr[0];
//               pxr[0] = gray;
//               pxi += 3;
//               pxr++;
//               accu += abs(gray - lgray);
//               bright += gray;
//             }
//           }
//           wc_motion.motion_trigger = accu / ((wc_fb->height * wc_fb->width) / 100);
//           wc_motion.motion_brightness = bright / ((wc_fb->height * wc_fb->width) / 100);
//           free(out_buf);
//         }
//       }
//     }
//     esp_camera_fb_return(wc_fb);
//   }
// }

// /*********************************************************************************************/

// #ifdef USE_FACE_DETECT

// void fd_init(void) {
//   Wc.mtmn_config.type = FAST;
//   Wc.mtmn_config.min_face = 80;
//   Wc.mtmn_config.pyramid = 0.707;
//   Wc.mtmn_config.pyramid_times = 4;
//   Wc.mtmn_config.p_threshold.score = 0.6;
//   Wc.mtmn_config.p_threshold.nms = 0.7;
//   Wc.mtmn_config.p_threshold.candidate_number = 20;
//   Wc.mtmn_config.r_threshold.score = 0.7;
//   Wc.mtmn_config.r_threshold.nms = 0.7;
//   Wc.mtmn_config.r_threshold.candidate_number = 10;
//   Wc.mtmn_config.o_threshold.score = 0.7;
//   Wc.mtmn_config.o_threshold.nms = 0.7;
//   Wc.mtmn_config.o_threshold.candidate_number = 1;
// }

// #define FACE_COLOR_WHITE  0x00FFFFFF
// #define FACE_COLOR_BLACK  0x00000000
// #define FACE_COLOR_RED    0x000000FF
// #define FACE_COLOR_GREEN  0x0000FF00
// #define FACE_COLOR_BLUE   0x00FF0000
// #define FACE_COLOR_YELLOW (FACE_COLOR_RED | FACE_COLOR_GREEN)
// #define FACE_COLOR_CYAN   (FACE_COLOR_BLUE | FACE_COLOR_GREEN)
// #define FACE_COLOR_PURPLE (FACE_COLOR_BLUE | FACE_COLOR_RED)
// void draw_face_boxes(dl_matrix3du_t *image_matrix, box_array_t *boxes, int face_id);

// /*
// void draw_face_boxes(dl_matrix3du_t *image_matrix, box_array_t *boxes, int face_id) {
//     int x, y, w, h, i;
//     uint32_t color = FACE_COLOR_YELLOW;
//     if(face_id < 0){
//         color = FACE_COLOR_RED;
//     } else if(face_id > 0){
//         color = FACE_COLOR_GREEN;
//     }
//     fb_data_t fb;
//     fb.width = image_matrix->w;
//     fb.height = image_matrix->h;
//     fb.data = image_matrix->item;
//     fb.bytes_per_pixel = 3;
//     fb.format = FB_BGR888;
//     for (i = 0; i < boxes->len; i++){
//         // rectangle box
//         x = (int)boxes->box[i].box_p[0];
//         y = (int)boxes->box[i].box_p[1];
//         w = (int)boxes->box[i].box_p[2] - x + 1;
//         h = (int)boxes->box[i].box_p[3] - y + 1;
//         fb_gfx_drawFastHLine(&fb, x, y, w, color);
//         fb_gfx_drawFastHLine(&fb, x, y+h-1, w, color);
//         fb_gfx_drawFastVLine(&fb, x, y, h, color);
//         fb_gfx_drawFastVLine(&fb, x+w-1, y, h, color);
// #if 0
//         // landmark
//         int x0, y0, j;
//         for (j = 0; j < 10; j+=2) {
//             x0 = (int)boxes->landmark[i].landmark_p[j];
//             y0 = (int)boxes->landmark[i].landmark_p[j+1];
//             fb_gfx_fillRect(&fb, x0, y0, 3, 3, color);
//         }
// #endif
//     }
// }
// */

// //#define DL_SPIRAM_SUPPORT

// uint32_t WcSetFaceDetect(int32_t value) {
//   if (value >= 0) { Wc.face_detect_time = value; }
//   return Wc.faces;
// }

// uint32_t WcDetectFace(void);

// uint32_t WcDetectFace(void) {
//   dl_matrix3du_t *image_matrix;
//   size_t out_len, out_width, out_height;
//   uint8_t * out_buf;
//   bool s;
//   bool detected = false;
//   int face_id = 0;
//   camera_fb_t *fb;

//   if ((millis() - Wc.face_ltime) > Wc.face_detect_time) {
//     Wc.face_ltime = millis();
//     fb = esp_camera_fb_get();
//     if (!fb) { return ESP_FAIL; }

//     image_matrix = dl_matrix3du_alloc(1, fb->width, fb->height, 3);
//     if (!image_matrix) {
//       ALOG_DBG(PSTR("CAM: dl_matrix3du_alloc failed"));
//       esp_camera_fb_return(fb);
//       return ESP_FAIL;
//     }

//     out_buf = image_matrix->item;
//     //out_len = fb->width * fb->height * 3;
//     //out_width = fb->width;
//     //out_height = fb->height;

//     s = fmt2rgb888(fb->buf, fb->len, fb->format, out_buf);
//     esp_camera_fb_return(fb);
//     if (!s){
//       dl_matrix3du_free(image_matrix);
//       ALOG_DBG(PSTR("CAM: to rgb888 failed"));
//       return ESP_FAIL;
//     }

//     box_array_t *net_boxes = face_detect(image_matrix, &Wc.mtmn_config);
//     if (net_boxes){
//       detected = true;
//       Wc.faces = net_boxes->len;
//       //if(recognition_enabled){
//       //    face_id = run_face_recognition(image_matrix, net_boxes);
//       //}
//       //draw_face_boxes(image_matrix, net_boxes, face_id);
//       free(net_boxes->score);
//       free(net_boxes->box);
//       free(net_boxes->landmark);
//       free(net_boxes);
//     } else {
//       Wc.faces = 0;
//     }
//     dl_matrix3du_free(image_matrix);
//     //Serial.printf("face detected: %d",Wc.faces);

//   }
//   return 0;
// }
// #endif

// /*********************************************************************************************/


// #ifdef COPYFRAME
// struct PICSTORE tmp_picstore;
// #endif

// uint32_t WcGetPicstore(int32_t num, uint8_t **buff) {
//   if (num<0) { return MAX_PICSTORE; }
//   *buff = Wc.picstore[num].buff;
//   return Wc.picstore[num].len;
// }

// uint32_t WcGetFrame(int32_t bnum) {
//   size_t _jpg_buf_len = 0;
//   uint8_t * _jpg_buf = NULL;
//   camera_fb_t *wc_fb = 0;
//   bool jpeg_converted = false;

//   if (bnum < 0) {
//     if (bnum < -MAX_PICSTORE) { bnum=-1; }
//     bnum = -bnum;
//     bnum--;
//     if (Wc.picstore[bnum].buff) { free(Wc.picstore[bnum].buff); }
//     Wc.picstore[bnum].len = 0;
//     return 0;
//   }

// #ifdef COPYFRAME
//   if (bnum & 0x10) {
//     bnum &= 0xf;
//     _jpg_buf = tmp_picstore.buff;
//     _jpg_buf_len = tmp_picstore.len;
//     if (!_jpg_buf_len) { return 0; }
//     goto pcopy;
//   }
// #endif

//   wc_fb = esp_camera_fb_get();
//   if (!wc_fb) {
//     ALOG_DBG(PSTR("CAM: Can't get frame"));
//     return 0;
//   }
//   if (!bnum) {
//     Wc.width = wc_fb->width;
//     Wc.height = wc_fb->height;
//     esp_camera_fb_return(wc_fb);
//     return 0;
//   }

//   if (wc_fb->format != PIXFORMAT_JPEG) {
//     jpeg_converted = frame2jpg(wc_fb, 80, &_jpg_buf, &_jpg_buf_len);
//     if (!jpeg_converted){
//       //Serial.println("JPEG compression failed");
//       _jpg_buf_len = wc_fb->len;
//       _jpg_buf = wc_fb->buf;
//     }
//   } else {
//     _jpg_buf_len = wc_fb->len;
//     _jpg_buf = wc_fb->buf;
//   }

// pcopy:
//   if ((bnum < 1) || (bnum > MAX_PICSTORE)) { bnum = 1; }
//   bnum--;
//   if (Wc.picstore[bnum].buff) { free(Wc.picstore[bnum].buff); }
//   Wc.picstore[bnum].buff = (uint8_t *)heap_caps_malloc(_jpg_buf_len+4, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
//   if (Wc.picstore[bnum].buff) {
//     memcpy(Wc.picstore[bnum].buff, _jpg_buf, _jpg_buf_len);
//     Wc.picstore[bnum].len = _jpg_buf_len;
//   } else {
//     ALOG_DBG(PSTR("CAM: Can't allocate picstore"));
//     Wc.picstore[bnum].len = 0;
//   }
//   if (wc_fb) { esp_camera_fb_return(wc_fb); }
//   if (jpeg_converted) { free(_jpg_buf); }
//   if (!Wc.picstore[bnum].buff) { return 0; }

//   return  _jpg_buf_len;
// }

// void HandleImage(void) {
//   if (!HttpCheckPriviledgedAccess()) { return; }

//   uint32_t bnum = Webserver->arg(F("p")).toInt();
//   if ((bnum < 0) || (bnum > MAX_PICSTORE)) { bnum= 1; }
//   WiFiClient client = Webserver->client();
//   String response = "HTTP/1.1 200 OK\r\n";
//   response += "Content-disposition: inline; filename=cap.jpg\r\n";
//   response += "Content-type: image/jpeg\r\n\r\n";
//   Webserver->sendContent(response);

//   if (!bnum) {
//     size_t _jpg_buf_len = 0;
//     uint8_t * _jpg_buf = NULL;
//     camera_fb_t *wc_fb = 0;
//     wc_fb = esp_camera_fb_get();
//     if (!wc_fb) { return; }
//     if (wc_fb->format != PIXFORMAT_JPEG) {
//       bool jpeg_converted = frame2jpg(wc_fb, 80, &_jpg_buf, &_jpg_buf_len);
//       if (!jpeg_converted) {
//         _jpg_buf_len = wc_fb->len;
//         _jpg_buf = wc_fb->buf;
//       }
//     } else {
//       _jpg_buf_len = wc_fb->len;
//       _jpg_buf = wc_fb->buf;
//     }
//     if (_jpg_buf_len) {
//       client.write((char *)_jpg_buf, _jpg_buf_len);
//     }
//     if (wc_fb) { esp_camera_fb_return(wc_fb); }
//   } else {
//     bnum--;
//     if (!Wc.picstore[bnum].len) {
//       ALOG_DBG(PSTR("CAM: No image #: %d"), bnum);
//       return;
//     }
//     client.write((char *)Wc.picstore[bnum].buff, Wc.picstore[bnum].len);
//   }
//   client.stop();

//   ALOG_DBM( PSTR("CAM: Sending image #: %d"), bnum+1);
// }

// void HandleImageBasic(void) {
//   if (!HttpCheckPriviledgedAccess()) { return; }

//   ALOG_DBM( PSTR(D_LOG_HTTP "Capture image"));

//   if (tkr_set->Settings.webcam_config.stream) {
//     if (!Wc.CamServer) {
//       WcStreamControl();
//     }
//   }

//   camera_fb_t *wc_fb;
//   wc_fb = esp_camera_fb_get();  // Acquire frame
//   if (!wc_fb) {
//     ALOG_DBG(PSTR("CAM: Frame buffer could not be acquired"));
//     return;
//   }

//   size_t _jpg_buf_len = 0;
//   uint8_t * _jpg_buf = NULL;
//   if (wc_fb->format != PIXFORMAT_JPEG) {
//     bool jpeg_converted = frame2jpg(wc_fb, 80, &_jpg_buf, &_jpg_buf_len);
//     if (!jpeg_converted) {
//       _jpg_buf_len = wc_fb->len;
//       _jpg_buf = wc_fb->buf;
//     }
//   } else {
//     _jpg_buf_len = wc_fb->len;
//     _jpg_buf = wc_fb->buf;
//   }

//   if (_jpg_buf_len) {
//     Webserver->client().flush();
//     WSHeaderSend();
//     Webserver->sendHeader(F("Content-disposition"), F("inline; filename=snapshot.jpg"));
//     Webserver->send_P(200, "image/jpeg", (char *)_jpg_buf, _jpg_buf_len);
//     Webserver->client().stop();
//   }

//   esp_camera_fb_return(wc_fb);  // Free frame buffer

//   ALOG_DBM( PSTR("CAM: Image sent"));
// }

void mCameraOV2640::HandleWebcamMjpeg(void) {
  ALOG_DBG(PSTR("CAM: Handle camserver"));
 if (!Wc.stream_active) {
// always restart stream
    Wc.stream_active = 1;
    Wc.client = Wc.CamServer->client();
    ALOG_DBG(PSTR("CAM: Create client"));
 }
}

void mCameraOV2640::HandleWebcamMjpegTask(void) {
  camera_fb_t *wc_fb;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;

  // WiFiClient client = CamServer->client();
  uint32_t tlen;
  bool jpeg_converted = false;

  if (!Wc.client.connected()) {
    ALOG_DBG(PSTR("CAM: Client fail"));
    Wc.stream_active = 0;
  }
  if (1 == Wc.stream_active) {
    Wc.client.flush();
    Wc.client.setTimeout(3);
    ALOG_DBG(PSTR("CAM: Start stream"));
    // Wc.client.print("HTTP/1.1 200 OK\r\n"
    //   "Content-Type: multipart/x-mixed-replace;boundary=" BOUNDARY "\r\n"
    //   "\r\n");
    Wc.stream_active = 2;
  }
  if (2 == Wc.stream_active) {
    wc_fb = esp_camera_fb_get();
    if (!wc_fb) {
      ALOG_DBG(PSTR("CAM: Frame fail"));
      Wc.stream_active = 0;
    }
  }
  if (2 == Wc.stream_active) {
    if (wc_fb->format != PIXFORMAT_JPEG) {
      jpeg_converted = frame2jpg(wc_fb, 80, &_jpg_buf, &_jpg_buf_len);
      if (!jpeg_converted){
        ALOG_DBG(PSTR("CAM: JPEG compression failed"));
        _jpg_buf_len = wc_fb->len;
        _jpg_buf = wc_fb->buf;
      }
    } else {
      _jpg_buf_len = wc_fb->len;
      _jpg_buf = wc_fb->buf;
    }

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
    // Wc.client.print("\r\n--" BOUNDARY "\r\n");

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

void mCameraOV2640::HandleWebcamRoot(void) 
{
  //CamServer->redirect("http://" + String(ip) + ":81/cam.mjpeg");
  Wc.CamServer->sendHeader("Location", WiFi.localIP().toString() + ":81/cam.mjpeg");
  Wc.CamServer->send(302, "", "");
  ALOG_DBG(PSTR("CAM: Root called"));
}

// /*********************************************************************************************/

uint32_t mCameraOV2640::WcSetStreamserver(uint32_t flag) {
ALOG_TST(PSTR("mCameraOV2640::WcSetStreamserver"));


if (tkr_set->global_state.network_down) { return 0; }

  Wc.stream_active = 0;

  if (flag) {
    if (!Wc.CamServer) {
      Wc.CamServer = new ESP8266WebServer(81);
      Wc.CamServer->on("/", [this](){HandleWebcamRoot();});
      Wc.CamServer->on("/cam.mjpeg", [this](){HandleWebcamMjpeg();});
      // Wc.CamServer->on("/cam.jpg", HandleWebcamMjpeg);
      // Wc.CamServer->on("/stream", HandleWebcamMjpeg);
      Wc.CamServer->on("/stream", [this](){HandleWebcamMjpeg();});
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
  return 0;
}

void mCameraOV2640::WcStreamControl() {
  WcSetStreamserver(tkr_set->Settings.webcam_config.stream);
  WcSetup(tkr_set->Settings.webcam_config.resolution);
}

// /*********************************************************************************************/


void mCameraOV2640::EveryLoop(void) {
  if (Wc.CamServer) {
    
ALOG_TST(PSTR("if (Wc.CamServer) "));
    Wc.CamServer->handleClient();
    if (Wc.stream_active) { HandleWebcamMjpegTask(); }
  }
  // if (wc_motion.motion_detect) { WcDetectMotion(); }
#ifdef USE_FACE_DETECT
  if (Wc.face_detect_time) { WcDetectFace(); }
#endif

#ifdef ENABLE_RTSPSERVER
    if (tkr_set->Settings.webcam_config.rtsp && !TasmotaGlobal.global_state.wifi_down && Wc.up) {
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

// void WcPicSetup(void) {
//   WebServer_on(PSTR("/wc.jpg"), HandleImage);
//   WebServer_on(PSTR("/wc.mjpeg"), HandleImage);
//   WebServer_on(PSTR("/snapshot.jpg"), HandleImage);
// }

void mCameraOV2640::WcShowStream(void) {
  if (tkr_set->Settings.webcam_config.stream) {
//    if (!Wc.CamServer || !Wc.up) {
    if (!Wc.CamServer) {
      WcStreamControl();
      delay(50);   // Give the webcam webserver some time to prepare the stream
    }
    if (Wc.CamServer && Wc.up) {
      // WSContentSend_P(PSTR("<p></p><center><img src='http://%_I:81/stream' alt='Webcam stream' style='width:99%%;'></center><p></p>"),
      //   (uint32_t)WiFi.localIP());
    }
  }
}







void mCameraOV2640::init(void)
{
  
    // ModuleSelected();
  // analogWriteFreq(25000);
  // if (SONOFF_IFAN03 == my_module_type) {
  //   SetSerial(9600, TS_SERIAL_8N1);
  // }
  // return false;  // Continue init chain

}

void mCameraOV2640::Pre_Init(){
  
// void WcInit(void) {
  // if (!tkr_set->Settings.webcam_config.data) {
    tkr_set->Settings.webcam_config.stream = 1;
    tkr_set->Settings.webcam_config.resolution = FRAMESIZE_QVGA;
    tkr_set->Settings.webcam_config.flip = 0;
    tkr_set->Settings.webcam_config.mirror = 0;
    tkr_set->Settings.webcam_config.saturation = 0;  // -2
    tkr_set->Settings.webcam_config.brightness = 3;  // 1
    tkr_set->Settings.webcam_config.contrast = 2;    // 0
  // }
// }

ALOG_TST(PSTR("mCameraOV2640::Pre_Init"));

}




uint8_t mCameraOV2640::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  // Active rgbcct palette used as scene

  //ShdSerial.println("mCameraOV2640::ConstructJSON_Settings");

  char buffer[30];
  
  JBI->Start();  

  // Got to ConstructJson_Scene out, or rename all the parameters as something else, or rgbcctactivepalette, or show them all? though that would need to run through, can only show
  // active_id, plus the values below
  // #ifndef ENABLE_DEVFEATURE_PHASING_SCENE_OUT
  //   JBI->Add_P(PM_SCENE_NAME, GetSceneName(buffer, sizeof(buffer)));  
  //   #endif //  ENABLE_DEVFEATURE_PHASING_SCENE_OUT
  
    // JBI->Add_P(PM_HUE, rgbcct_controller.getHue360());
    // JBI->Add_P(PM_SAT, rgbcct_controller.getSat255());
    // JBI->Add_P(PM_BRIGHTNESS_RGB, rgbcct_controller.getBrightnessRGB255());
    JBI->Add_P(PM_TIME, 1000);

    // JBI->Add("serial_active",Shd.hardware_serial_active);

    // JBI->Add("version_major",Shd.dimmer.version_major);
    // JBI->Add("version_minor",Shd.dimmer.version_minor);
    // JBI->Add("brightness",Shd.dimmer.brightness);
    // JBI->Add("power",Shd.dimmer.power);
    // JBI->Add("fade_rate",Shd.dimmer.fade_rate);



    // JBI->Add_P(PM_TIME_MS, animation.time_ms);
  return JBI->End();

}

uint8_t mCameraOV2640::ConstructJSON_State(uint8_t json_level, bool json_appending){
  
  JBI->Start();  

    // JBI->Add_P(PM_BRIGHTNESS, map(Shd.dimmer.brightness,0,1000,0,100));

  return JBI->End();

}


////////////////////// START OF MQTT /////////////////////////

void mCameraOV2640::MQTTHandler_Init(){

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_HOUR;//tkr_set->pCONT_mqtt->dt.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mCameraOV2640::ConstructJSON_Settings;

  
  ptr = &mqtthandler_state_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mCameraOV2640::ConstructJSON_State;

//   ptr = &mqtthandler_sensdebug_teleperiod;
//   ptr->tSavedLastSent = 0;
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = tkr_set->pCONT_mqtt->dt.ifchanged_secs; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_CTR;
//   ptr->ConstructJSON_function = &mCameraOV2640::ConstructJSON_Debug;

} 


void mCameraOV2640::MQTTHandler_RefreshAll(){

  mqtthandler_settings.flags.SendNow = true;
  // mqtthandler_animation_teleperiod.flags.SendNow = true;
  // mqtthandler_ambilight_teleperiod.flags.SendNow = true;
//   mqtthandler_scene_teleperiod.flags.SendNow = true;

} 


void mCameraOV2640::MQTTHandler_Rate(){

  mqtthandler_settings.tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
  // // mqtthandler_animation_teleperiod.tRateSecs = tkr_set->pCONT_mqtt->dt.teleperiod_secs;
  // // mqtthandler_ambilight_teleperiod.tRateSecs = tkr_set->pCONT_mqtt->dt.teleperiod_secs;
//   mqtthandler_scene_teleperiod.tRateSecs = tkr_set->pCONT_mqtt->dt.teleperiod_secs;
  
} //end "MQTTHandler_Rate"


void mCameraOV2640::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID
    //, MQTT_HANDLER_MODULE_SCENE_TELEPERIOD_ID, MQTT_HANDLER_MODULE_DEBUG_PARAMETERS_TELEPERIOD_ID
  };
  
  struct handler<mCameraOV2640>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings
    //, &mqtthandler_scene_teleperiod, &mqtthandler_debug_teleperiod
  };

  // pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, EM_MODULE_DRIVERS_CAMERA_OV2640_ID,
  //   mqtthandler_list_ptr, mqtthandler_list_ids, 
  //   sizeof(mqtthandler_list_ids)/sizeof(mqtthandler_list_ids[0]),
  //   mqtt_handler_id
  // );

}













// #if defined(CAMERA_MODEL_WROVER_KIT)
// #define PWDN_GPIO_NUM    -1
// #define RESET_GPIO_NUM   -1
// #define XCLK_GPIO_NUM    21
// #define SIOD_GPIO_NUM    26
// #define SIOC_GPIO_NUM    27

// #define Y9_GPIO_NUM      35
// #define Y8_GPIO_NUM      34
// #define Y7_GPIO_NUM      39
// #define Y6_GPIO_NUM      36
// #define Y5_GPIO_NUM      19
// #define Y4_GPIO_NUM      18
// #define Y3_GPIO_NUM       5
// #define Y2_GPIO_NUM       4
// #define VSYNC_GPIO_NUM   25
// #define HREF_GPIO_NUM    23
// #define PCLK_GPIO_NUM    22

// #elif defined(CAMERA_MODEL_ESP_EYE)
// #define PWDN_GPIO_NUM    -1
// #define RESET_GPIO_NUM   -1
// #define XCLK_GPIO_NUM    4
// #define SIOD_GPIO_NUM    18
// #define SIOC_GPIO_NUM    23

// #define Y9_GPIO_NUM      36
// #define Y8_GPIO_NUM      37
// #define Y7_GPIO_NUM      38
// #define Y6_GPIO_NUM      39
// #define Y5_GPIO_NUM      35
// #define Y4_GPIO_NUM      14
// #define Y3_GPIO_NUM      13
// #define Y2_GPIO_NUM      34
// #define VSYNC_GPIO_NUM   5
// #define HREF_GPIO_NUM    27
// #define PCLK_GPIO_NUM    25

// #elif defined(CAMERA_MODEL_M5STACK_PSRAM)
// #define PWDN_GPIO_NUM     -1
// #define RESET_GPIO_NUM    15
// #define XCLK_GPIO_NUM     27
// #define SIOD_GPIO_NUM     25
// #define SIOC_GPIO_NUM     23

// #define Y9_GPIO_NUM       19
// #define Y8_GPIO_NUM       36
// #define Y7_GPIO_NUM       18
// #define Y6_GPIO_NUM       39
// #define Y5_GPIO_NUM        5
// #define Y4_GPIO_NUM       34
// #define Y3_GPIO_NUM       35
// #define Y2_GPIO_NUM       32
// #define VSYNC_GPIO_NUM    22
// #define HREF_GPIO_NUM     26
// #define PCLK_GPIO_NUM     21

// #elif defined(CAMERA_MODEL_M5STACK_WIDE)
// #define PWDN_GPIO_NUM     -1
// #define RESET_GPIO_NUM    15
// #define XCLK_GPIO_NUM     27
// #define SIOD_GPIO_NUM     22
// #define SIOC_GPIO_NUM     23

// #define Y9_GPIO_NUM       19
// #define Y8_GPIO_NUM       36
// #define Y7_GPIO_NUM       18
// #define Y6_GPIO_NUM       39
// #define Y5_GPIO_NUM        5
// #define Y4_GPIO_NUM       34
// #define Y3_GPIO_NUM       35
// #define Y2_GPIO_NUM       32
// #define VSYNC_GPIO_NUM    25
// #define HREF_GPIO_NUM     26
// #define PCLK_GPIO_NUM     21

// #elif defined(CAMERA_MODEL_AI_THINKER)
// #define PWDN_GPIO_NUM     32
// #define RESET_GPIO_NUM    -1
// #define XCLK_GPIO_NUM      0
// #define SIOD_GPIO_NUM     26
// #define SIOC_GPIO_NUM     27

// #define Y9_GPIO_NUM       35
// #define Y8_GPIO_NUM       34
// #define Y7_GPIO_NUM       39
// #define Y6_GPIO_NUM       36
// #define Y5_GPIO_NUM       21
// #define Y4_GPIO_NUM       19
// #define Y3_GPIO_NUM       18
// #define Y2_GPIO_NUM        5
// #define VSYNC_GPIO_NUM    25
// #define HREF_GPIO_NUM     23
// #define PCLK_GPIO_NUM     22

// #else
// #error "Camera model not selected"
// #endif




//   #define SSID1 ""
//   #define PWD1 ""

// /*
//   Next one is an include with wifi credentials.
//   This is what you need to do:

//   1. Create a file called "home_wifi_multi.h" in the same folder   OR   under a separate subfolder of the "libraries" folder of Arduino IDE. (You are creating a "fake" library really - I called it "MySettings").
//   2. Place the following text in the file:
//   #define SSID1 "replace with your wifi ssid"
//   #define PWD1 "replace your wifi password"
//   3. Save.

//   Should work then
// */
// //#include "home_wifi_multi.h"

OV2640 cam;

WebServer server(80);

// ===== rtos task handles =========================
// Streaming is implemented with 3 tasks:
TaskHandle_t tMjpeg;   // handles client connections to the webserver
TaskHandle_t tCam;     // handles getting picture frames from the camera and storing them locally
TaskHandle_t tStream;  // actually streaming frames to all connected clients

// frameSync semaphore is used to prevent streaming buffer as it is replaced with the next frame
SemaphoreHandle_t frameSync = NULL;

// Queue stores currently connected clients to whom we are streaming
QueueHandle_t streamingClients;

// We will try to achieve 25 FPS frame rate
const int FPS = 14;

// We will handle web client requests every 50 ms (20 Hz)
const int WSINTERVAL = 100;

void handleJPGSstream(void);
void SubTask_GrabCameraFrames(void* pvParameters);
void SubTask_StreamCameraFrames(void * pvParameters);


// Commonly used variables:
volatile size_t camSize;    // size of the current frame, byte
volatile char* camBuf;      // pointer to the current frame


/**
 * Memory allocator that takes advantage of PSRAM if present 
 * */
char* allocateMemory(char* aPtr, size_t aSize) {

  //  Since current buffer is too smal, free it
  if (aPtr != NULL) free(aPtr);

  size_t freeHeap = ESP.getFreeHeap();
  char* ptr = NULL;

  // If memory requested is more than 2/3 of the currently free heap, try PSRAM immediately
  if ( aSize > freeHeap * 2 / 3 ) {
    if ( psramFound() && ESP.getFreePsram() > aSize ) {
      ptr = (char*) ps_malloc(aSize);
    }
  }
  else {
    //  Enough free heap - let's try allocating fast RAM as a buffer
    ptr = (char*) malloc(aSize);
    //  If allocation on the heap failed, let's give PSRAM one more chance:
    if ( ptr == NULL && psramFound() && ESP.getFreePsram() > aSize) {
      ptr = (char*) ps_malloc(aSize);
    }
  }

  // Finally, if the memory pointer is NULL, we were not able to allocate any memory, and that is a terminal condition.
  if (ptr == NULL) {
    // ESP.restart();
    // temporarily disable camera to retry other measures
  }
  return ptr;
}


/**
 * RTOS task to grab frames from the camera 
 * */
void SubTask_GrabCameraFrames(void* pvParameters) {

  Serial.println("SubTask_GrabCameraFrames1"); Serial.flush();
  TickType_t xLastWakeTime;

  //  A running interval associated with currently desired frame rate
  const TickType_t xFrequency = pdMS_TO_TICKS(1000 / FPS);

  // Mutex for the critical section of swithing the active frames around
  portMUX_TYPE xSemaphore = portMUX_INITIALIZER_UNLOCKED;

  //  Pointers to the 2 frames, their respective sizes and index of the current frame
  char* fbs[2] = { NULL, NULL };
  size_t fSize[2] = { 0, 0 };
  int ifb = 0;

  //=== loop() section  ===================
  xLastWakeTime = xTaskGetTickCount();

  Serial.println("SubTask_GrabCameraFrames2"); Serial.flush();
  for (;;) {

    //  Grab a frame from the camera and query its size
    cam.run();
    size_t s = cam.getSize();

    //  If frame size is more that we have previously allocated - request  125% of the current frame space
    if (s > fSize[ifb]) {
      fSize[ifb] = s * 4 / 3;
      fbs[ifb] = allocateMemory(fbs[ifb], fSize[ifb]);
    }

    //  Copy current frame into local buffer
    char* b = (char*) cam.getfb();
    memcpy(fbs[ifb], b, s);

    //  Let other tasks run and wait until the end of the current frame rate interval (if any time left)
    taskYIELD();
    vTaskDelayUntil(&xLastWakeTime, xFrequency);

    //  Only switch frames around if no frame is currently being streamed to a client
    //  Wait on a semaphore until client operation completes
    xSemaphoreTake( frameSync, portMAX_DELAY );

    //  Do not allow interrupts while switching the current frame
    portENTER_CRITICAL(&xSemaphore);
    camBuf = fbs[ifb];
    camSize = s;
    ifb++;
    ifb &= 1;  // this should produce 1, 0, 1, 0, 1 ... sequence
    portEXIT_CRITICAL(&xSemaphore);

    //  Let anyone waiting for a frame know that the frame is ready
    xSemaphoreGive( frameSync );

    //  Technically only needed once: let the streaming task know that we have at least one frame
    //  and it could start sending frames to the clients, if any
    xTaskNotifyGive( tStream );

    //  Immediately let other (streaming) tasks run
    taskYIELD();
    yield();

    //  If streaming task has suspended itself (no active clients to stream to)
    //  there is no need to grab frames from the camera. We can save some juice
    //  by suspedning the tasks
    if ( eTaskGetState( tStream ) == eSuspended ) {
      vTaskSuspend(NULL);  // passing NULL means "suspend yourself"
    }
  }
}



// ==== STREAMING ======================================================
const char HEADER[] = "HTTP/1.1 200 OK\r\n" \
                      "Access-Control-Allow-Origin: *\r\n" \
                      "Content-Type: multipart/x-mixed-replace; boundary=123456789000000000000987654321\r\n";
const char BOUNDARY[] = "\r\n--123456789000000000000987654321\r\n";
const char CTNTTYPE[] = "Content-Type: image/jpeg\r\nContent-Length: ";
const int hdrLen = strlen(HEADER);
const int bdrLen = strlen(BOUNDARY);
const int cntLen = strlen(CTNTTYPE);



// ==== Handle connection request from clients ===============================
void handleJPGSstream(void)
{
  Serial.println("handleJPGSstream0"); Serial.flush();
  //  Can only acommodate 10 clients. The limit is a default for WiFi connections
  if ( !uxQueueSpacesAvailable(streamingClients) ) return;

  Serial.println("handleJPGSstream1"); Serial.flush();

  //  Create a new WiFi Client object to keep track of this one
  WiFiClient client;
  client = server.client();
  client.write(HEADER, hdrLen);
  client.write(BOUNDARY, bdrLen);

  Serial.println("handleJPGSstream2"); Serial.flush();
  // Push the client to the streaming queue
  xQueueSend(streamingClients, (void *) &client, 0);

  // Wake up streaming tasks, if they were previously suspended:
  Serial.println("handleJPGSstream3"); Serial.flush();
  if ( eTaskGetState( tCam ) == eSuspended ) vTaskResume( tCam );
  Serial.println("handleJPGSstream4"); Serial.flush();
  if ( eTaskGetState( tStream ) == eSuspended ) vTaskResume( tStream );
  Serial.println("handleJPGSstream5"); Serial.flush();
}


// ==== Actually stream content to all connected clients ========================
void SubTask_StreamCameraFrames(void * pvParameters) {

  char buf[16];
  TickType_t xLastWakeTime;
  TickType_t xFrequency;

  //  Wait until the first frame is captured and there is something to send to clients
  ulTaskNotifyTake( pdTRUE,          /* Clear the notification value before exiting. */
                    portMAX_DELAY ); /* Block indefinitely. */

  xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    // Default assumption we are running according to the FPS
    xFrequency = pdMS_TO_TICKS(1000 / FPS);

    //  Only bother to send anything if there is someone watching
    UBaseType_t activeClients = uxQueueMessagesWaiting(streamingClients);
    if ( activeClients ) {
      // Adjust the period to the number of connected clients
      xFrequency /= activeClients;

      //  Since we are sending the same frame to everyone,
      //  pop a client from the the front of the queue
      WiFiClient *client;
      xQueueReceive (streamingClients, (void*) &client, 0);

      //  Check if this client is still connected.

      if (!client->connected()) {
        //  delete this client reference if s/he has disconnected
        //  and don't put it back on the queue anymore. Bye!
        delete client;
      }
      else {

        //  Ok. This is an actively connected client.
        //  Let's grab a semaphore to prevent frame changes while we
        //  are serving this frame
        xSemaphoreTake( frameSync, portMAX_DELAY );

        client->write(CTNTTYPE, cntLen);
        sprintf(buf, "%d\r\n\r\n", camSize);
        client->write(buf, strlen(buf));
        client->write((char*) camBuf, (size_t)camSize);
        client->write(BOUNDARY, bdrLen);

        // Since this client is still connected, push it to the end
        // of the queue for further processing
        xQueueSend(streamingClients, (void *) &client, 0);

        //  The frame has been served. Release the semaphore and let other tasks run.
        //  If there is a frame switch ready, it will happen now in between frames
        xSemaphoreGive( frameSync );
        taskYIELD();
        yield();
      }
    }
    else {
      //  Since there are no connected clients, there is no reason to waste battery running
      vTaskSuspend(NULL);
    }
    //  Let other tasks run after serving every client
    taskYIELD();
    yield();
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}



const char JHEADER[] = "HTTP/1.1 200 OK\r\n" \
                       "Content-disposition: inline; filename=capture.jpg\r\n" \
                       "Content-type: image/jpeg\r\n\r\n";
const int jhdLen = strlen(JHEADER);

// ==== Serve up one JPEG frame =============================================
void handleJPG(void)
{
  WiFiClient client = server.client();

  if (!client.connected()) return;
  cam.run();
  client.write(JHEADER, jhdLen);
  client.write((char*)cam.getfb(), cam.getSize());


}


// ==== Handle invalid URL requests ============================================
void handleNotFound()
{
  String message = "Server is running!\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  server.send(200, "text / plain", message);
}


// ======== Server Connection Handler Task ==========================
void SubTask_StartServerHandlerTask1(void* pvParameters) {
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = pdMS_TO_TICKS(WSINTERVAL);

  // Creating frame synchronization semaphore and initializing it
  frameSync = xSemaphoreCreateBinary();
  xSemaphoreGive( frameSync );

  // Creating a queue to track all connected clients
  streamingClients = xQueueCreate( 10, sizeof(WiFiClient*) );

  //=== setup section  ==================

  //  Creating RTOS task for grabbing frames from the camera
  xTaskCreatePinnedToCore(
    SubTask_GrabCameraFrames,        // callback
    "cam",        // name
    4096,         // stacj size
    NULL,         // parameters
    2,            // priority
    &tCam,        // RTOS task handle
    APP_CPU);     // core

  //  Creating task to push the stream to all connected clients
  xTaskCreatePinnedToCore(
    SubTask_StreamCameraFrames,
    "strmCB",
    4 * 1024,
    NULL,
    2,
    &tStream,
    APP_CPU);

  //  Registering webserver handling routines
  server.on("/vid", handleJPGSstream);
  server.on("/pic", handleJPG);
  server.onNotFound(handleNotFound);

  //  Starting webserver
  server.begin();

  //=== loop() section  ===================
  xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    server.handleClient();
    //  After every server client handling request, we let other tasks run and then pause
    taskYIELD();
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }

}



void startCameraServer();

// ==== SETUP method ==================================================================
void mCameraOV2640::setup_cam()
{

//   // Setup Serial connection:
//   Serial.begin(115200);
//   delay(1000); // wait for a second to let Serial connect
//   Serial.println("Rebooting");


  // Configure the camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
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
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // Frame parameters: pick one
  //  config.frame_size = FRAMESIZE_UXGA;
  //  config.frame_size = FRAMESIZE_SVGA;
  //  config.frame_size = FRAMESIZE_QVGA;
  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 12;
  config.fb_count = 2;

  if (cam.init(config) != ESP_OK) {
    Serial.println("Error initializing the camera");
    // delay(10000);
    // ESP.restart();

  }else{
    
    settings.caminit = true;

    // Start mainstreaming RTOS task
    xTaskCreatePinnedToCore(
      SubTask_StartServerHandlerTask1,
      "mjpeg",
      4 * 1024,
      NULL,
      2,
      &tMjpeg,
      APP_CPU
    );

    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(WSINTERVAL);

    // Creating frame synchronization semaphore and initializing it
    frameSync = xSemaphoreCreateBinary();
    xSemaphoreGive( frameSync );

    // Creating a queue to track all connected clients
    streamingClients = xQueueCreate( 10, sizeof(WiFiClient*) );

    //=== setup section  ==================

    //  Creating RTOS task for grabbing frames from the camera
    xTaskCreatePinnedToCore(
      SubTask_GrabCameraFrames,        // callback
      "cam",        // name
      4096,         // stacj size
      NULL,         // parameters
      2,            // priority
      &tCam,        // RTOS task handle
      APP_CPU);     // core

    //  Creating task to push the stream to all connected clients
    xTaskCreatePinnedToCore(
      SubTask_StreamCameraFrames,
      "strmCB",
      4 * 1024,
      NULL,
      2,
      &tStream,
      APP_CPU);

    //  Registering webserver handling routines
    server.on("/vid", handleJPGSstream);
    server.on("/pic", handleJPG);
    server.onNotFound(handleNotFound);

    //  Starting webserver
    server.begin();

  }

}


int8_t mCameraOV2640::Tasker(uint8_t function, JsonParserObject obj){

  /************
   * INIT SECTION * 
  *******************/
  if(function == TASK_PRE_INIT){
    Pre_Init();
  }else
  if(function == TASK_INIT){
    init();
  }

  // Only continue in to tasker if module was configured properly
  // if(!Settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP:
      EveryLoop();

      if(settings.caminit)
      {
        server.handleClient();
      }
    break;
    case TASK_EVERY_SECOND:  
      // Poll();    
      // WcStreamControl();
      // WcSetStreamserver(1);
      ALOG_TST(PSTR("caminit = %d"),  settings.caminit);
      // setup_cam();
    break;
    case TASK_EVERY_MINUTE:
      if(!settings.caminit && (tkr_time->uptime_seconds_nonreset>60)){
        setup_cam();
      }
    break;
    case TASK_WIFI_CONNECTED:
      setup_cam();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      // parse_JSONCommand(obj);
    break;
    // case TASK_SET_DEVICE_POWER:
    //   SetPower();
    // break;
    /************
     * RULES SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_RULES
    case TASK_EVENT_SET_POWER_ID:
      // RulesEvent_Set_Power();
    break;
    #endif// USE_MODULE_CORE_RULES

    
//     case TASK_WEB_ADD_HANDLER:
//       WcPicSetup();
//       break;


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
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT

  }
  
//     // case TASK_COMMAND:
//     //   result = DecodeCommand(kWCCommands, WCCommand);
//     //   break;


  /************
   * WEBPAGE SECTION * 
  *******************/
  
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_NETWORK_WEBSERVER

} // END Tasker



#endif // USE_MODULE_DRIVERS_CAMERA_OV2640