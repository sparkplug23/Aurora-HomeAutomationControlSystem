/*
  Asynchronous WebServer library for Espressif MCUs

  Copyright (c) 2016 Hristo Gochkov. All rights reserved.
  This file is part of the esp8266 core for Arduino environment.

  Modified by my_name_here to remove as much dependency on Strings as possible

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
#include "ESPAsyncWebServer.h"
#include "WebResponseImpl.h"
#include "cbuf.h"

// #define DEBUG_ASYNC

// Since ESP8266 does not link memchr by default, here's its implementation.
void* memchr(void* ptr, int ch, size_t count)
{
  unsigned char* p = static_cast<unsigned char*>(ptr);
  while(count--)
    if(*p++ == static_cast<unsigned char>(ch))
      return --p;
  return nullptr;
}

/*
 * Abstract Response
 * */
const char* AsyncWebServerResponse::_responseCodeToString(int code) {
  switch (code) {
    case 100: return PSTR("Continue");
    case 101: return PSTR("Switching Protocols");
    case 200: return PSTR("OK");
    case 201: return PSTR("Created");
    case 202: return PSTR("Accepted");
    case 203: return PSTR("Non-Authoritative Information");
    case 204: return PSTR("No Content");
    case 205: return PSTR("Reset Content");
    case 206: return PSTR("Partial Content");
    case 300: return PSTR("Multiple Choices");
    case 301: return PSTR("Moved Permanently");
    case 302: return PSTR("Found");
    case 303: return PSTR("See Other");
    case 304: return PSTR("Not Modified");
    case 305: return PSTR("Use Proxy");
    case 307: return PSTR("Temporary Redirect");
    case 400: return PSTR("Bad Request");
    case 401: return PSTR("Unauthorized");
    case 402: return PSTR("Payment Required");
    case 403: return PSTR("Forbidden");
    case 404: return PSTR("Not Found");
    case 405: return PSTR("Method Not Allowed");
    case 406: return PSTR("Not Acceptable");
    case 407: return PSTR("Proxy Authentication Required");
    case 408: return PSTR("Request Time-out");
    case 409: return PSTR("Conflict");
    case 410: return PSTR("Gone");
    case 411: return PSTR("Length Required");
    case 412: return PSTR("Precondition Failed");
    case 413: return PSTR("Request Entity Too Large");
    case 414: return PSTR("Request-URI Too Large");
    case 415: return PSTR("Unsupported Media Type");
    case 416: return PSTR("Requested range not satisfiable");
    case 417: return PSTR("Expectation Failed");
    case 500: return PSTR("Internal Server Error");
    case 501: return PSTR("Not Implemented");
    case 502: return PSTR("Bad Gateway");
    case 503: return PSTR("Service Unavailable");
    case 504: return PSTR("Gateway Time-out");
    case 505: return PSTR("HTTP Version not supported");
    default:  return PSTR("");
  }
}

AsyncWebServerResponse::AsyncWebServerResponse()
  : _code(0)
  , _headers(LinkedList<AsyncWebHeader *>([](AsyncWebHeader *h){ delete h; }))
  , _contentType()
  , _contentLength(0)
  , _sendContentLength(true)
  , _chunked(false)
  , _headLength(0)
  , _sentLength(0)
  , _ackedLength(0)
  , _writtenLength(0)
  , _state(RESPONSE_SETUP)
{
  for(auto header: DefaultHeaders::Instance()) {
    _headers.add(new AsyncWebHeader(header->name(), header->value()));
  }
}

AsyncWebServerResponse::~AsyncWebServerResponse(){
  _headers.free();
}

void AsyncWebServerResponse::setCode(int code){
  if(_state == RESPONSE_SETUP)
    _code = code;
}

void AsyncWebServerResponse::setContentLength(size_t len){
  if(_state == RESPONSE_SETUP)
    _contentLength = len;
}

void AsyncWebServerResponse::setContentType(uint8_t type){
  if(_state == RESPONSE_SETUP)
    _contentType = type;
}


void AsyncWebServerResponse::addHeader(const char* name, const char* value){
  #ifdef DEBUG_ASYNC
    Serial.printf("addHeader\n\r");
  #endif
  _headers.add(new AsyncWebHeader(String(name), String(value)));
}


uint16_t AsyncWebServerResponse::_assembleHead(char* buff, uint8_t version, uint16_t buff_size){
  
  #ifdef DEBUG_ASYNC
  Serial.println("_assembleHead"); Serial.flush();
  #endif

  if(version){
    addHeader(("Accept-Ranges"),("none")); //adds to linked list
    // addHeader(("Page-Name"),("none")); //adds to linked list
    if(_chunked)
      addHeader(("Transfer-Encoding"),("chunked"));
  }
  // CheckAndAddUserHeaders();
  _headLength = 0;
  _headLength += snprintf_P(buff+_headLength, buff_size, PSTR("HTTP/1.%d %d %s\r\n"), version, _code, _responseCodeToString(_code));
  
  if(_sendContentLength) {
    _headLength += snprintf_P(buff+_headLength, buff_size, PSTR("Content-Length: %d\r\n"), _contentLength);
  }
  // if(strlen_P(_contentType)) {
  //   // _headLength += snprintf(buff+_headLength, buff_size, PSTR("Content-Type: %s\r\n"), _contentType);
  //   _headLength += sprintf(buff+_headLength, PSTR("Content-Type: %s\r\n"), "_contentType");
  // }
  
  // if(_contentType_id == 0){ //assume default

  // }


  if(_contentType_id){
    // Index from 0, but 0 is skipped in enums
    char buffer_contentType[30];
    // sprintf(buffer_contentType,"TEST");

    switch(_contentType_id){
      default:
      case CONTENT_TYPE_TEXT_HTML_ID: 
        memcpy_P(buffer_contentType, PM_WEB_CONTENT_TYPE_TEXT_HTML, sizeof(PM_WEB_CONTENT_TYPE_TEXT_HTML)); 
      break; // Necessary casts and dereferencing, just copy.
      case CONTENT_TYPE_TEXT_JAVASCRIPT_ID: 
        memcpy_P(buffer_contentType, PM_WEB_CONTENT_TYPE_TEXT_JAVASCRIPT, sizeof(PM_WEB_CONTENT_TYPE_TEXT_JAVASCRIPT)); 
      break;
      case CONTENT_TYPE_APPLICATION_JSON_ID: 
        memcpy_P(buffer_contentType, PM_WEB_CONTENT_TYPE_APPLICATION_JSON_JAVASCRIPT, sizeof(PM_WEB_CONTENT_TYPE_APPLICATION_JSON_JAVASCRIPT)); 
      break;
      case CONTENT_TYPE_TEXT_CSS_ID: 
        memcpy_P(buffer_contentType, PM_WEB_CONTENT_TYPE_TEXT_CSS, sizeof(PM_WEB_CONTENT_TYPE_TEXT_CSS)); 
      break;
    }
    
    _headLength += sprintf_P(buff+_headLength, PSTR("Content-Type: %s\r\n"), buffer_contentType);
  
  }

  for(const auto& header: _headers){
    _headLength += snprintf_P(buff+_headLength, buff_size, PSTR("%s: %s\r\n"), header->name().c_str(), header->value().c_str());
  }
  _headers.free();

  _headLength += snprintf_P(buff+_headLength, buff_size, PSTR("\r\n"));

  #ifdef DEBUG_ASYNC
  Serial.println(buff); Serial.flush();
  #endif

  return _headLength;
}

bool AsyncWebServerResponse::_started() const { return _state > RESPONSE_SETUP; }
bool AsyncWebServerResponse::_finished() const { return _state > RESPONSE_WAIT_ACK; }
bool AsyncWebServerResponse::_failed() const { return _state == RESPONSE_FAILED; }
bool AsyncWebServerResponse::_sourceValid() const { return false; }
void AsyncWebServerResponse::_respond(AsyncWebServerRequest *request){ _state = RESPONSE_END; request->client()->close(); }
size_t AsyncWebServerResponse::_ack(AsyncWebServerRequest *request, size_t len, uint32_t time){ return 0; }

/*
 * Built a basic response by configuring parameters
 * */
// #ifdef USE_CONTENT_TYPE_ID
AsyncBasicResponse::AsyncBasicResponse(int code, uint8_t contentType, char* content_ctr, uint16_t content_len){
  _code = code;
  _content_ptr = content_ctr;
  _contentType = contentType;
  _content_len = content_len;

  #ifdef DEBUG_ASYNC
  Serial.println("AsyncBasicResponse");
  Serial.printf("_code2=%d\n\r",_code);
  Serial.printf("_content_ptr=%s\n\r",_content_ptr);
  // Serial.printf("_contentType=%s\n\r",_contentType.c_str());
  #endif

  // if a value was passed for length, save into _contentLength
  if(content_len){
    #ifdef DEBUG_ASYNC
    Serial.printf("_content_len=%d\n\r",_content_len);
    #endif
    _contentLength = _content_len;
    //assume default type if non is set
    // if(!strlen(_contentType))      
    //   sprintf(_contentType,"%s","text/plain");
  } 
  //close connection after transmission, basic single packet response
  addHeader("Connection","close");
}
// #else
// AsyncBasicResponse::AsyncBasicResponse(int code, uint8_t contentType, char* content_ctr, uint16_t content_len){
//   _code = code;
//   _content_ptr = content_ctr;
//     _contentType = contentType;
//   _content_len = content_len;

//   #ifdef DEBUG_ASYNC
//   Serial.println("AsyncBasicResponse");
//   Serial.printf("_code2=%d\n\r",_code);
//   Serial.printf("_content_ptr=%s\n\r",_content_ptr);
//   Serial.printf("_contentType=%s\n\r",_contentType.c_str());
//   #endif

//   // if a value was passed for length, save into _contentLength
//   if(content_len){
//     #ifdef DEBUG_ASYNC
//     Serial.printf("_content_len=%d\n\r",_content_len);
//     #endif
//     _contentLength = _content_len;
//     //assume default type if non is set
//     // if(!strlen(_contentType))      
//     //   sprintf(_contentType,"%s","text/plain");
//   } 
//   //close connection after transmission, basic single packet response
//   addHeader(PSTR("Connection"),PSTR("close"));
// }
// #endif
// AsyncBasicResponse::AsyncBasicResponse(int code, uint8_t contentType, char* content_ctr, uint16_t content_len){
//   _code = code;
//   _content_ptr = content_ctr;
//   // switch(contentType_id){
//   //   case 0: sprintf(_contentType,"%s",contentType); break;
//   // }
//   sprintf(_contentType,"%s",contentType);
//   _content_len = content_len;

//   #ifdef DEBUG_ASYNC
//   Serial.println("AsyncBasicResponse");
//   Serial.printf("_code2=%d\n\r",_code);
//   Serial.printf("_content_ptr=%s\n\r",_content_ptr);
//   Serial.printf("_contentType=%s\n\r",_contentType.c_str());
//   #endif

//   // if a value was passed for length, save into _contentLength
//   if(content_len){
//     #ifdef DEBUG_ASYNC
//     Serial.printf("_content_len=%d\n\r",_content_len);
//     #endif
//     _contentLength = _content_len;
//     //assume default type if non is set
//     if(!strlen(_contentType))      
//       sprintf(_contentType,"%s","text/plain");
//   } 
//   //close connection after transmission, basic single packet response
//   addHeader(PSTR("Connection"),PSTR("close"));
// }

/*
 * Called by callback, used to actually transmit response back to client
 * */
void AsyncBasicResponse::_respond(AsyncWebServerRequest *request){
  #ifdef DEBUG_ASYNC
  Serial.println("AsyncBasicResponse::_respond(request) = Primary send mechanism"); Serial.flush();
  #endif
  _state = RESPONSE_HEADERS;

  uint16_t header_max_length = 250;
  char header_ctr[header_max_length];//write header into output string //maximum header size for stability
  char* header_ptr = &header_ctr[0];
  size_t header_len = _assembleHead(header_ptr, request->version(), header_max_length);
  size_t space = request->client()->space();

  if(!_contentLength && space >= header_len){ 
    #ifdef DEBUG_ASYNC
      Serial.printf("{No Body: Send head}!_contentLength[%d] && space[%d] >= outLen[%d]\n\r",_contentLength,space,header_len);
    #endif
    if(header_ptr != nullptr){
      _writtenLength += request->client()->write(header_ptr, header_len);
    }
    _state = RESPONSE_WAIT_ACK;
  } else 
  if(_contentLength && space >= header_len + _contentLength){ 
    #ifdef DEBUG_ASYNC
      Serial.printf("{Header+Body: Send all}_contentLength[%d] && space[%d] >= outLen[%d] + _contentLength[%d]\n\r",_contentLength,space,header_len,_contentLength);
      Serial.printf("out.c_str()=BEFORE\n\r");
      // Serial.println(header_str);
    #endif
    
    // Transmit header
    if(header_ptr != nullptr){
      _writtenLength += request->client()->write(header_ptr, header_len);
    }
    // Transmit body using null terminated array IF it was set
    if(_content_ptr != nullptr){
      #ifdef DEBUG_ASYNC
      Serial.printf("_content_ptr != nullptr\n\r");
      #endif
      _writtenLength += request->client()->write(_content_ptr, _content_len);
    }
    #ifdef DEBUG_ASYNC
      Serial.printf("_writtenLength=%d\n\r",_writtenLength);
    #endif
    _state = RESPONSE_WAIT_ACK;
  } 
  // else if(space && space < outLen){
  // #ifdef DEBUG_ASYNC
  //   Serial.printf("{Send partial}space[%d] && space[%d] < outLen[%d]\n\r",space,space,outLen);
  //   #endif
  //   String partial = out.substring(0, space);
  //   _content = out.substring(space) + _content;
  //   _contentLength += outLen - space;
  //   _writtenLength += request->client()->write(partial.c_str(), partial.length());
  //   _state = RESPONSE_CONTENT;
  // } else if(space > outLen && space < (outLen + _contentLength)){ 
  // #ifdef DEBUG_ASYNC
  //   Serial.printf("{Send partial remaining}space[%d] > outLen[%d] && space[%d] < (outLen[%d] + _contentLength[%d])\n\r",
  //     space,outLen,space,outLen,_contentLength);
  //     #endif
  //   size_t shift = space - outLen;
  //   outLen += shift;
  //   _sentLength += shift;
  //   out += _content.substring(0, shift);
  //   _content = _content.substring(shift); 
  // #ifdef DEBUG_ASYNC
  //   Serial.printf("request->client()->write(out.ctr(),outlen[%d])  shift[%d] \n\r",outLen,shift);
  //   #endif
  //   _writtenLength += request->client()->write(out.c_str(), outLen); 
  // #ifdef DEBUG_ASYNC
  //   Serial.println("request->client()->write DONE");
  //   #endif
  //   _state = RESPONSE_CONTENT;
  // }
   else { 
    #ifdef DEBUG_ASYNC
      Serial.println("{Send nothing}else");
    #endif
    // _content_ptr += _contentLength;    
    // _content = out + _content;
    // _contentLength += outLen;
    _state = RESPONSE_CONTENT;
  }

  #ifdef DEBUG_ASYNC
  Serial.printf("space=%d\n\r",space);
  Serial.printf("_writtenLength=%d of %d\n\r",_writtenLength, header_len);
  Serial.flush();
  #endif

}

/**
 * Converted to send pointer
 * * */
size_t AsyncBasicResponse::_ack(AsyncWebServerRequest *request, size_t len, uint32_t time){
  _ackedLength += len;
  if(_state == RESPONSE_CONTENT){
    size_t available = _contentLength - _sentLength;
    size_t space = request->client()->space();
    //we can fit in this packet
    if(space > available){
      if(_content_ptr != nullptr){
        _writtenLength += request->client()->write(_content_ptr, available);
        _content_ptr = 0; //_content_str = String();    //resetting
      }      
      _state = RESPONSE_WAIT_ACK;
      return available;
    }
    //send some data, the rest on ack
    if(_content_ptr != nullptr){
      _writtenLength += request->client()->write(_content_ptr+_content_sent, space);
      _content_sent += _writtenLength;
      _sentLength += _content_sent;
    }
    return space;
  } else if(_state == RESPONSE_WAIT_ACK){
    if(_ackedLength >= _writtenLength){
      _state = RESPONSE_END;
    }
  }
  return 0;
}

/*
 * Abstract Response
 * */
AsyncAbstractResponse::AsyncAbstractResponse(AwsTemplateProcessor callback): _callback(callback)
{
  // In case of template processing, we're unable to determine real response size
  if(callback) {
    _contentLength = 0;
    _sendContentLength = false;
    _chunked = true;
  }
}

void AsyncAbstractResponse::_respond(AsyncWebServerRequest *request){
  
      // Serial.println("AsyncAbstractResponse::_respond"); Serial.flush();
  addHeader(PSTR("Connection"),PSTR("close"));
  uint16_t header_max_length = 250;
  char header_ctr[header_max_length];//write header into output string //maximum header size for stability
  size_t header_len = _assembleHead(header_ctr, request->version(), header_max_length);
  
      // Serial.printf("AsyncAbstractResponse::_respond header_ctr=%s\n\r",header_ctr); Serial.flush();
  _head = String(header_ctr);//_assembleHead(request->version());
  _state = RESPONSE_HEADERS;
  _ack(request, 0, 0);
}

size_t AsyncAbstractResponse::_ack(AsyncWebServerRequest *request, size_t len, uint32_t time){

  
      // Serial.printf("AsyncAbstractResponse::_ack\n\r"); Serial.flush();
  if(!_sourceValid()){
    _state = RESPONSE_FAILED;
    request->client()->close();
    return 0;
  }
  _ackedLength += len;
  size_t space = request->client()->space();

  size_t headLen = _head.length();
  if(_state == RESPONSE_HEADERS){
    if(space >= headLen){
      _state = RESPONSE_CONTENT;
      space -= headLen;
    } else {
      String out = _head.substring(0, space);
      _head = _head.substring(space);
      _writtenLength += request->client()->write(out.c_str(), out.length());
      return out.length();
    }
  }

  if(_state == RESPONSE_CONTENT){
    size_t outLen;
    if(_chunked){
      if(space <= 8){
        return 0;
      }
      outLen = space;
    } else if(!_sendContentLength){
      outLen = space;
    } else {
      outLen = ((_contentLength - _sentLength) > space)?space:(_contentLength - _sentLength);
    }

    uint8_t *buf = (uint8_t *)malloc(outLen+headLen);
    if (!buf) {
      // os_printf("_ack malloc %d failed\n", outLen+headLen);
      return 0;
    }

    if(headLen){
      memcpy(buf, _head.c_str(), _head.length());
    }

    size_t readLen = 0;

    if(_chunked){
      // HTTP 1.1 allows leading zeros in chunk length. Or spaces may be added.
      // See RFC2616 sections 2, 3.6.1.
      readLen = _fillBufferAndProcessTemplates(buf+headLen+6, outLen - 8);
      if(readLen == RESPONSE_TRY_AGAIN){
          free(buf);
          return 0;
      }
      outLen = sprintf((char*)buf+headLen, "%x", readLen) + headLen;
      while(outLen < headLen + 4) buf[outLen++] = ' ';
      buf[outLen++] = '\r';
      buf[outLen++] = '\n';
      outLen += readLen;
      buf[outLen++] = '\r';
      buf[outLen++] = '\n';
    } else {
      readLen = _fillBufferAndProcessTemplates(buf+headLen, outLen);
      if(readLen == RESPONSE_TRY_AGAIN){
          free(buf);
          return 0;
      }
      outLen = readLen + headLen;
    }

    if(headLen){
        _head = String();
    }

    if(outLen){
        _writtenLength += request->client()->write((const char*)buf, outLen);
    }

    if(_chunked){
        _sentLength += readLen;
    } else {
        _sentLength += outLen - headLen;
    }

      // Serial.printf("AsyncAbstractResponse::_ack free(buf)=%s\n\r",buf); Serial.flush();
    free(buf);

    if((_chunked && readLen == 0) || (!_sendContentLength && outLen == 0) || (!_chunked && _sentLength == _contentLength)){
      _state = RESPONSE_WAIT_ACK;
    }
    return outLen;

  } else if(_state == RESPONSE_WAIT_ACK){
    if(!_sendContentLength || _ackedLength >= _writtenLength){
      _state = RESPONSE_END;
      if(!_chunked && !_sendContentLength)
        request->client()->close(true);
    }
  }
  return 0;
}

size_t AsyncAbstractResponse::_readDataFromCacheOrContent(uint8_t* data, const size_t len)
{
    // If we have something in cache, copy it to buffer
    const size_t readFromCache = std::min(len, _cache.size());
    if(readFromCache) {
      memcpy(data, _cache.data(), readFromCache);
      _cache.erase(_cache.begin(), _cache.begin() + readFromCache);
    }
    // If we need to read more...
    const size_t needFromFile = len - readFromCache;
    const size_t readFromContent = _fillBuffer(data + readFromCache, needFromFile);
    return readFromCache + readFromContent;
}

size_t AsyncAbstractResponse::_fillBufferAndProcessTemplates(uint8_t* data, size_t len)
{
  if(!_callback)
    return _fillBuffer(data, len);

  const size_t originalLen = len;
  len = _readDataFromCacheOrContent(data, len);
  // Now we've read 'len' bytes, either from cache or from file
  // Search for template placeholders
  uint8_t* pTemplateStart = data;
  while((pTemplateStart < &data[len]) && (pTemplateStart = (uint8_t*)memchr(pTemplateStart, TEMPLATE_PLACEHOLDER, &data[len - 1] - pTemplateStart + 1))) { // data[0] ... data[len - 1]
    uint8_t* pTemplateEnd = (pTemplateStart < &data[len - 1]) ? (uint8_t*)memchr(pTemplateStart + 1, TEMPLATE_PLACEHOLDER, &data[len - 1] - pTemplateStart) : nullptr;
    // temporary buffer to hold parameter name
    uint8_t buf[TEMPLATE_PARAM_NAME_LENGTH + 1];
    String paramName;
    // If closing placeholder is found:
    if(pTemplateEnd) {
      // prepare argument to callback
      const size_t paramNameLength = std::min(sizeof(buf) - 1, (unsigned int)(pTemplateEnd - pTemplateStart - 1));
      if(paramNameLength) {
        memcpy(buf, pTemplateStart + 1, paramNameLength);
        buf[paramNameLength] = 0;
        paramName = String(reinterpret_cast<char*>(buf));
      } else { // double percent sign encountered, this is single percent sign escaped.
        // remove the 2nd percent sign
        memmove(pTemplateEnd, pTemplateEnd + 1, &data[len] - pTemplateEnd - 1);
        len += _readDataFromCacheOrContent(&data[len - 1], 1) - 1;
        ++pTemplateStart;
      }
    } else if(&data[len - 1] - pTemplateStart + 1 < TEMPLATE_PARAM_NAME_LENGTH + 2) { // closing placeholder not found, check if it's in the remaining file data
      memcpy(buf, pTemplateStart + 1, &data[len - 1] - pTemplateStart);
      const size_t readFromCacheOrContent = _readDataFromCacheOrContent(buf + (&data[len - 1] - pTemplateStart), TEMPLATE_PARAM_NAME_LENGTH + 2 - (&data[len - 1] - pTemplateStart + 1));
      if(readFromCacheOrContent) {
        pTemplateEnd = (uint8_t*)memchr(buf + (&data[len - 1] - pTemplateStart), TEMPLATE_PLACEHOLDER, readFromCacheOrContent);
        if(pTemplateEnd) {
          // prepare argument to callback
          *pTemplateEnd = 0;
          paramName = String(reinterpret_cast<char*>(buf));
          // Copy remaining read-ahead data into cache
          _cache.insert(_cache.begin(), pTemplateEnd + 1, buf + (&data[len - 1] - pTemplateStart) + readFromCacheOrContent);
          pTemplateEnd = &data[len - 1];
        }
        else // closing placeholder not found in file data, store found percent symbol as is and advance to the next position
        {
          // but first, store read file data in cache
          _cache.insert(_cache.begin(), buf + (&data[len - 1] - pTemplateStart), buf + (&data[len - 1] - pTemplateStart) + readFromCacheOrContent);
          ++pTemplateStart;
        }
      }
      else // closing placeholder not found in content data, store found percent symbol as is and advance to the next position
        ++pTemplateStart;
    }
    else // closing placeholder not found in content data, store found percent symbol as is and advance to the next position
      ++pTemplateStart;
    if(paramName.length()) {
      // call callback and replace with result.
      // Everything in range [pTemplateStart, pTemplateEnd] can be safely replaced with parameter value.
      // Data after pTemplateEnd may need to be moved.
      // The first byte of data after placeholder is located at pTemplateEnd + 1.
      // It should be located at pTemplateStart + numBytesCopied (to begin right after inserted parameter value).
      const String paramValue(_callback(paramName));
      const char* pvstr = paramValue.c_str();
      const unsigned int pvlen = paramValue.length();
      const size_t numBytesCopied = std::min(pvlen, static_cast<unsigned int>(&data[originalLen - 1] - pTemplateStart + 1));
      // make room for param value
      // 1. move extra data to cache if parameter value is longer than placeholder AND if there is no room to store
      if((pTemplateEnd + 1 < pTemplateStart + numBytesCopied) && (originalLen - (pTemplateStart + numBytesCopied - pTemplateEnd - 1) < len)) {
        _cache.insert(_cache.begin(), &data[originalLen - (pTemplateStart + numBytesCopied - pTemplateEnd - 1)], &data[len]);
        //2. parameter value is longer than placeholder text, push the data after placeholder which not saved into cache further to the end
        memmove(pTemplateStart + numBytesCopied, pTemplateEnd + 1, &data[originalLen] - pTemplateStart - numBytesCopied);
      } else if(pTemplateEnd + 1 != pTemplateStart + numBytesCopied)
        //2. Either parameter value is shorter than placeholder text OR there is enough free space in buffer to fit.
        //   Move the entire data after the placeholder
        memmove(pTemplateStart + numBytesCopied, pTemplateEnd + 1, &data[len] - pTemplateEnd - 1);
      // 3. replace placeholder with actual value
      memcpy(pTemplateStart, pvstr, numBytesCopied);
      // If result is longer than buffer, copy the remainder into cache (this could happen only if placeholder text itself did not fit entirely in buffer)
      if(numBytesCopied < pvlen) {
        _cache.insert(_cache.begin(), pvstr + numBytesCopied, pvstr + pvlen);
      } else if(pTemplateStart + numBytesCopied < pTemplateEnd + 1) { // result is copied fully; if result is shorter than placeholder text...
        // there is some free room, fill it from cache
        const size_t roomFreed = pTemplateEnd + 1 - pTemplateStart - numBytesCopied;
        const size_t totalFreeRoom = originalLen - len + roomFreed;
        len += _readDataFromCacheOrContent(&data[len - roomFreed], totalFreeRoom) - roomFreed;
      } else { // result is copied fully; it is longer than placeholder text
        const size_t roomTaken = pTemplateStart + numBytesCopied - pTemplateEnd - 1;
        len = std::min(len + roomTaken, originalLen);
      }
    }
  } // while(pTemplateStart)
  return len;
}


/*
 * File Response
 * */

AsyncFileResponse::~AsyncFileResponse(){
  if(_content)
    _content.close();
}

void AsyncFileResponse::_setContentType(const String& path){
  // if (path.endsWith(".html"))       sprintf(_contentType,PSTR("text/html"));
  // else if (path.endsWith(".htm"))   sprintf(_contentType,PSTR("text/html"));
  // else if (path.endsWith(".css"))   sprintf(_contentType,PSTR("text/css"));
  // else if (path.endsWith(".json"))  sprintf(_contentType,PSTR("application/json"));
  // else if (path.endsWith(".js"))    sprintf(_contentType,PSTR("application/javascript"));
  // else if (path.endsWith(".png"))   sprintf(_contentType,PSTR("image/png"));
  // else if (path.endsWith(".gif"))   sprintf(_contentType,PSTR("image/gif"));
  // else if (path.endsWith(".jpg"))   sprintf(_contentType,PSTR("image/jpeg"));
  // else if (path.endsWith(".ico"))   sprintf(_contentType,PSTR("image/x-icon"));
  // else if (path.endsWith(".svg"))   sprintf(_contentType,PSTR("image/svg+xml"));
  // else if (path.endsWith(".eot"))   sprintf(_contentType,PSTR("font/eot"));
  // else if (path.endsWith(".woff"))  sprintf(_contentType,PSTR("font/woff"));
  // else if (path.endsWith(".woff2")) sprintf(_contentType,PSTR("font/woff2"));
  // else if (path.endsWith(".ttf"))   sprintf(_contentType,PSTR("font/ttf"));
  // else if (path.endsWith(".xml"))   sprintf(_contentType,PSTR("text/xml"));
  // else if (path.endsWith(".pdf"))   sprintf(_contentType,PSTR("application/pdf"));
  // else if (path.endsWith(".zip"))   sprintf(_contentType,PSTR("application/zip"));
  // else if(path.endsWith(".gz"))     sprintf(_contentType,PSTR("application/x-gzip"));
  // else                              sprintf(_contentType,PSTR("text/plain"));
}

AsyncFileResponse::AsyncFileResponse(FS &fs, const char* path1, uint8_t contentType, bool download, AwsTemplateProcessor callback): AsyncAbstractResponse(callback){
  _code = 200;
  String path = String(path1);
  _path = path;

  if(!download && !fs.exists(_path) && fs.exists(_path+".gz")){
    _path = _path+".gz";
    addHeader(PSTR("Content-Encoding"), PSTR("gzip"));
    _callback = nullptr; // Unable to process zipped templates
    _sendContentLength = true;
    _chunked = false;
  }

  _content = fs.open(_path, "r");
  _contentLength = _content.size();

  if(contentType == 0)
    _setContentType(path);
  else
    _contentType = 0;//(char*)contentType;

  int filenameStart = path.lastIndexOf('/') + 1;
  char buf[26+path.length()-filenameStart];
  char* filename = (char*)path.c_str() + filenameStart;

  if(download) {
    // set filename and force download
    snprintf(buf, sizeof (buf), PSTR("attachment; filename=\"%s\""), filename);
  } else {
    // set filename and force rendering
    snprintf(buf, sizeof (buf), PSTR("inline; filename=\"%s\""), filename);
  }
  addHeader(PSTR("Content-Disposition"), buf);
}

AsyncFileResponse::AsyncFileResponse(File content, const char* path1, uint8_t contentType, bool download, AwsTemplateProcessor callback): AsyncAbstractResponse(callback){
  _code = 200;
  
  String path = String(path1);
  _path = path;

  if(!download && String(content.name()).endsWith(".gz") && !path.endsWith(".gz")){
    addHeader(PSTR("Content-Encoding"), PSTR("gzip"));
    _callback = nullptr; // Unable to process gzipped templates
    _sendContentLength = true;
    _chunked = false;
  }

  _content = content;
  _contentLength = _content.size();

  if(contentType == 0)
    _setContentType(path);
  else
    _contentType = 0;//(char*)contentType;
    // sprintf(_contentType,"%s",contentType);

  int filenameStart = path.lastIndexOf('/') + 1;
  char buf[26+path.length()-filenameStart];
  char* filename = (char*)path.c_str() + filenameStart;

  if(download) {
    snprintf(buf, sizeof (buf), PSTR("attachment; filename=\"%s\""), filename);
  } else {
    snprintf(buf, sizeof (buf), PSTR("inline; filename=\"%s\""), filename);
  }
  addHeader(PSTR("Content-Disposition"), buf);
}

size_t AsyncFileResponse::_fillBuffer(uint8_t *data, size_t len){
  return _content.read(data, len);
}

/*
 * Stream Response
 * */

AsyncStreamResponse::AsyncStreamResponse(Stream &stream, uint8_t contentType_id, size_t len, AwsTemplateProcessor callback): AsyncAbstractResponse(callback) {
  _code = 200;
  _content = &stream;
  _contentLength = len;
  _contentType = contentType_id;
}

size_t AsyncStreamResponse::_fillBuffer(uint8_t *data, size_t len){
  size_t available = _content->available();
  size_t outLen = (available > len)?len:available;
  size_t i;
  for(i=0;i<outLen;i++)
    data[i] = _content->read();
  return outLen;
}

/*
 * Callback Response
 * */

AsyncCallbackResponse::AsyncCallbackResponse(uint8_t contentType, size_t len, AwsResponseFiller callback, AwsTemplateProcessor templateCallback): AsyncAbstractResponse(templateCallback) {
  _code = 200;
  _content = callback;
  _contentLength = len;
  if(!len)
    _sendContentLength = false;
  _contentType = 0;//contentType;
  _filledLength = 0;
}

size_t AsyncCallbackResponse::_fillBuffer(uint8_t *data, size_t len){
  size_t ret = _content(data, len, _filledLength);
  if(ret != RESPONSE_TRY_AGAIN){
      _filledLength += ret;
  }
  return ret;
}

/*
 * Chunked Response
 * */

AsyncChunkedResponse::AsyncChunkedResponse(uint8_t contentType, AwsResponseFiller callback, AwsTemplateProcessor processorCallback): AsyncAbstractResponse(processorCallback) {
  _code = 200;
  _content = callback;
  _contentLength = 0;
  _contentType = 0;//contentType;
  _sendContentLength = false;
  _chunked = true;
  _filledLength = 0;
}

size_t AsyncChunkedResponse::_fillBuffer(uint8_t *data, size_t len){
  size_t ret = _content(data, len, _filledLength);
  if(ret != RESPONSE_TRY_AGAIN){
      _filledLength += ret;
  }
  return ret;
}

/*
 * Progmem Response
 * */

AsyncProgmemResponse::AsyncProgmemResponse(int code, uint8_t contentType_id, const uint8_t * content, size_t len, AwsTemplateProcessor callback): AsyncAbstractResponse(callback) {
  _code = code;
  _content_ptr = content;
  // _contentType = (char*)contentType;
  _contentType_id = contentType_id;
  _contentLength = len;
  _readLength = 0;

  #ifdef DEBUG_ASYNC
  Serial.printf("AsyncProgmemResponse built");
  #endif
}

size_t AsyncProgmemResponse::_fillBuffer(uint8_t *data, size_t len){
  #ifdef DEBUG_ASYNC
  Serial.println("_fillBuffer(uint8_t *data, size_t len)");
  #endif
  size_t left = _contentLength - _readLength;
  if (left > len) {
    memcpy_P(data, _content_ptr + _readLength, len);
    _readLength += len;
    return len;
  }
  memcpy_P(data, _content_ptr + _readLength, left);
  _readLength += left;
  return left;
}


/*
 * Response Stream (You can print/write/printf to it, up to the contentLen bytes)
 * */

AsyncResponseStream::AsyncResponseStream(uint8_t contentType, size_t bufferSize){
  _code = 200;
  _contentLength = 0;
  _contentType = 0;//contentType;
  _content = new cbuf(bufferSize);
}

AsyncResponseStream::~AsyncResponseStream(){
  delete _content;
}

size_t AsyncResponseStream::_fillBuffer(uint8_t *buf, size_t maxLen){
  return _content->read((char*)buf, maxLen);
}

size_t AsyncResponseStream::write(const uint8_t *data, size_t len){
  //Serial.println("AsyncResponseStream::write");
  if(_started()){   
   //Serial.println("here?");
    return 0;
  }

  if(len > _content->room()){
    size_t needed = len - _content->room();
    _content->resizeAdd(needed);
  }
  size_t written = _content->write((const char*)data, len);
  _contentLength += written;
  return written;
}

size_t AsyncResponseStream::write(uint8_t data){
  return write(&data, 1);
}
