/*
  Asynchronous WebServer library for Espressif MCUs

  Copyright (c) 2016 Hristo Gochkov. All rights reserved.
  This file is part of the esp8266 core for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef ASYNCWEBSERVERRESPONSEIMPL_H_
#define ASYNCWEBSERVERRESPONSEIMPL_H_

#ifdef Arduino_h
// arduino is not compatible with std::vector
#undef min
#undef max
#endif
#include <vector>
// It is possible to restore these defines, but one can use _min and _max instead. Or std::min, std::max.

#define DEBUG_ASYNC_STRINGS

#define USE_CONTENT_TYPE_ID


const char PM_WEB_CONTENT_TYPE_TEXT_HTML[] PROGMEM = "text/html";
const char PM_WEB_CONTENT_TYPE_TEXT_JAVASCRIPT[] PROGMEM = "text/javascript";
const char PM_WEB_CONTENT_TYPE_APPLICATION_JSON_JAVASCRIPT[] PROGMEM = "application/json";
const char PM_WEB_CONTENT_TYPE_TEXT_CSS[] PROGMEM = "text/css";

// #define WEB_CONTENT_TYPE_TEXT_HTML "text/html"
// #define WEB_CONTENT_TYPE_TEXT_JAVASCRIPT "text/javascript"
// #define CONTENT_TYPE_APPLICATION_JSON_ID "application/json"

enum CONTENT_TYPE_IDS{
  // 0 is unset
  CONTENT_TYPE_TEXT_HTML_ID=1,
  CONTENT_TYPE_TEXT_JAVASCRIPT_ID,
  CONTENT_TYPE_APPLICATION_JSON_ID,
  CONTENT_TYPE_TEXT_CSS_ID,
};


// const char *const CONTENT_STYPE_TABLE_ARRAY[] PROGMEM = {
//   PM_WEB_CONTENT_TYPE_TEXT_HTML, 
//   PM_WEB_CONTENT_TYPE_TEXT_JAVASCRIPT,
//   PM_WEB_CONTENT_TYPE_APPLICATION_JSON_ID_JAVASCRIPT
// };


class AsyncBasicResponse: public AsyncWebServerResponse {
  private:
    char* _content_ptr = nullptr;
    uint16_t _content_sent; //for incremental transmission
    uint16_t _content_len;
  public:
    //#ifdef USE_CONTENT_TYPE_ID
      AsyncBasicResponse(int code, const char* contentType, char* _content_ptr = nullptr, uint16_t _content_len = 0);
    //#else 
      AsyncBasicResponse(int code, uint8_t contentType_id, char* _content_ptr = nullptr, uint16_t _content_len = 0);
    //#endif
    void _respond(AsyncWebServerRequest *request);
    size_t _ack(AsyncWebServerRequest *request, size_t len, uint32_t time);
    bool _sourceValid() const { return true; }
};

class AsyncAbstractResponse: public AsyncWebServerResponse {
  private:
    String _head;
    // Data is inserted into cache at begin(). 
    // This is inefficient with vector, but if we use some other container, 
    // we won't be able to access it as contiguous array of bytes when reading from it,
    // so by gaining performance in one place, we'll lose it in another.
    std::vector<uint8_t> _cache;
    size_t _readDataFromCacheOrContent(uint8_t* data, const size_t len);
    size_t _fillBufferAndProcessTemplates(uint8_t* buf, size_t maxLen);
  protected:
    AwsTemplateProcessor _callback;
  public:
    AsyncAbstractResponse(AwsTemplateProcessor callback=nullptr);
    void _respond(AsyncWebServerRequest *request);
    size_t _ack(AsyncWebServerRequest *request, size_t len, uint32_t time);
    bool _sourceValid() const { return false; }
    virtual size_t _fillBuffer(uint8_t *buf __attribute__((unused)), size_t maxLen __attribute__((unused))) { return 0; }
};

#ifndef TEMPLATE_PLACEHOLDER
#define TEMPLATE_PLACEHOLDER '%'
#endif

#define TEMPLATE_PARAM_NAME_LENGTH 32
class AsyncFileResponse: public AsyncAbstractResponse {
  using File = fs::File;
  using FS = fs::FS;
  private:
    File _content;
    String _path;
    void _setContentType(const String& path);
  public:
    AsyncFileResponse(FS &fs,       const char* path, uint8_t contentType, bool download=false, AwsTemplateProcessor callback=nullptr);
    AsyncFileResponse(File content, const char* path, uint8_t contentType, bool download=false, AwsTemplateProcessor callback=nullptr);
    ~AsyncFileResponse();
    bool _sourceValid() const { return !!(_content); }
    virtual size_t _fillBuffer(uint8_t *buf, size_t maxLen) override;
};

class AsyncStreamResponse: public AsyncAbstractResponse {
  private:
    Stream *_content;
  public:
    AsyncStreamResponse(Stream &stream, uint8_t contentType, size_t len=0, AwsTemplateProcessor callback=nullptr);
    bool _sourceValid() const { return !!(_content); }
    virtual size_t _fillBuffer(uint8_t *buf, size_t maxLen) override;
};

class AsyncCallbackResponse: public AsyncAbstractResponse {
  private:
    AwsResponseFiller _content;
    size_t _filledLength;
  public:
    AsyncCallbackResponse(uint8_t contentType=0, size_t len=0, AwsResponseFiller callback=nullptr, AwsTemplateProcessor templateCallback=nullptr);
    bool _sourceValid() const { return !!(_content); }
    virtual size_t _fillBuffer(uint8_t *buf, size_t maxLen) override;
};

class AsyncChunkedResponse: public AsyncAbstractResponse {
  private:
    AwsResponseFiller _content;
    size_t _filledLength;
  public:
    AsyncChunkedResponse(uint8_t contentType, AwsResponseFiller callback, AwsTemplateProcessor templateCallback=nullptr);
    bool _sourceValid() const { return !!(_content); }
    virtual size_t _fillBuffer(uint8_t *buf, size_t maxLen) override;
};

class AsyncProgmemResponse: public AsyncAbstractResponse {
  private:
    const uint8_t * _content_ptr;
    size_t _readLength;
  public:
    AsyncProgmemResponse(int code, uint8_t contentType, const uint8_t * content, size_t len, AwsTemplateProcessor callback=nullptr);
    // AsyncProgmemResponse2(int code, const String& contentType, char* content_ptr, size_t len, AwsTemplateProcessor callback=nullptr);
    bool _sourceValid() const { return true; }
    virtual size_t _fillBuffer(uint8_t *buf, size_t maxLen) override;
};

class cbuf;

class AsyncResponseStream: public AsyncAbstractResponse, public Print {
  private:
    cbuf *_content;
    char* _content_ptr;
  public:
    AsyncResponseStream(uint8_t contentType, size_t bufferSize);
    ~AsyncResponseStream();
    bool _sourceValid() const { return (_state < RESPONSE_END); }
    virtual size_t _fillBuffer(uint8_t *buf, size_t maxLen) override;
    size_t write(const uint8_t *data, size_t len);
    size_t write(uint8_t data);
    using Print::write;
};

#endif /* ASYNCWEBSERVERRESPONSEIMPL_H_ */
