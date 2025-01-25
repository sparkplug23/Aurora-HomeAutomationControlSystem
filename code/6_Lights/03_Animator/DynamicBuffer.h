// // DynamicBuffer and DynamicBufferList
// // Simple class for holding a dynamically allocated buffer

// #pragma once

// #include "Arduino.h"
// #include <memory>
// #include <list>
// #include <utility>

// // Forward declaration
// class SharedBuffer;

// // The DynamicBuffer class holds a malloc() allocated heap buffer.
// // It's similar to std::vector<char>, but permits allocation failures without crashing the system.
// class DynamicBuffer {
//   char* _data;
//   size_t _len;

//   public:

//   void clear() { if (_data) free(_data); _data = nullptr; _len = 0; }

//   DynamicBuffer() : _data(nullptr), _len(0) {};
//   explicit DynamicBuffer(size_t len) : _data(len ? reinterpret_cast<char*>(malloc(len)): nullptr), _len(_data ? len : 0) {};
//   DynamicBuffer(const char* buf, size_t len) : DynamicBuffer(len) { if (_data) memcpy(_data, buf, len); };
//   explicit DynamicBuffer(const String& s) : DynamicBuffer(s.begin(), s.length()) {};
//   explicit DynamicBuffer(String&&);  // Move string contents in to buffer if possible
//   DynamicBuffer(const SharedBuffer&);
//   DynamicBuffer(SharedBuffer&&);
//   ~DynamicBuffer() { clear(); };
  
//   // Move
//   DynamicBuffer(DynamicBuffer&& d) : _data(d._data), _len(d._len) { d._data = nullptr; d._len = 0; };
//   DynamicBuffer& operator=(DynamicBuffer&& d) { std::swap(_data, d._data); std::swap(_len, d._len); return *this; };

//   // Copy
//   DynamicBuffer(const DynamicBuffer& d) : DynamicBuffer(d._data, d._len) {};  // copy
//   DynamicBuffer& operator=(const DynamicBuffer& d) { *this = DynamicBuffer(d); return *this; }; // use move to copy

//   // Accessors
//   char* data() const { return _data; };
//   size_t size() const { return _len; };

//   explicit operator bool() const { return (_data != nullptr) && (_len > 0); }

//   // Release the buffer without freeing it
//   char* release() { char* temp = _data; _data = nullptr; _len = 0; return temp; }

//   // TODO, if it ever matters - resizing
// };

// // Same interface as DynamicBuffer, but with shared_ptr semantics: buffer is held until last copy releases it.
// class SharedBuffer {
//   std::shared_ptr<DynamicBuffer> _buf;
//   friend class DynamicBuffer;

//   public:

//   SharedBuffer() = default;
//   explicit SharedBuffer(size_t len) : _buf(std::make_shared<DynamicBuffer>(len)) {};
//   SharedBuffer(const char* buf, size_t len) : _buf(std::make_shared<DynamicBuffer>(buf, len)) {};
//   explicit SharedBuffer(const String& s) : _buf(std::make_shared<DynamicBuffer>(s)) {};
//   explicit SharedBuffer(String&& s) : _buf(std::make_shared<DynamicBuffer>(std::move(s))) {};
//   SharedBuffer(const DynamicBuffer &d) : _buf(std::make_shared<DynamicBuffer>(d)) {};
//   SharedBuffer(DynamicBuffer&& d) : _buf(std::make_shared<DynamicBuffer>(std::move(d))) {};

//   char* data() const { return _buf ? _buf->data() : nullptr; };
//   size_t size() const { return _buf ? _buf->size() : 0U; };
//   void clear() { _buf.reset(); };

//   explicit operator bool() const { return _buf && *_buf; };
//   DynamicBuffer copy() const { return *_buf; }; // Make a copy of the buffer
// };

// // Utility functions
// String toString(DynamicBuffer buf);   // Move a buffer in to a string.  Buffer will be moved if buf is an rvalue, copied otherwise.


// // DynamicBufferList - an RAII list of DynamicBuffers
// // This structure can be used to create a chain of buffers, useful when the heap could get fragmented
// typedef std::list<DynamicBuffer> DynamicBufferList;
// typedef std::list<SharedBuffer> SharedBufferList;

// // Factory function: creates a list of buffers of max_buffer_size up to total bytes long.
// // The last buffer will fit exactly the required size.
// DynamicBufferList allocateDynamicBufferList(size_t total, size_t max_buffer_size = 0);
// SharedBufferList allocateSharedBufferList(size_t total, size_t max_buffer_size = 0);

// // Returns the total size of a DynamicBufferList
// size_t totalSize(const DynamicBufferList&);
// size_t totalSize(const SharedBufferList&);

// // Helper class - lets you stream something in to a DynamicBufferList, such as ArduinoJSON
// // If buffer size is nonzero, allocates additional buffers of buffer_size as required.
// template<typename list_type>
// class BufferListPrint : public Print {
//   private:
//     list_type& _list;
//     size_t _buffer_size;
//     typename list_type::const_iterator _next;
//     size_t _offset;
//     bool _valid;

//   public:
//     BufferListPrint(list_type& list, size_t buffer_size = 0) : _list(list), _buffer_size(buffer_size), _next(_list.begin()), _offset(0), _valid(true) {};
//     virtual ~BufferListPrint(){}

//     size_t write(const uint8_t *buffer, size_t size) {
//       if (!_valid) return 0;  // short circuit exit if we ran out of space (or memory, if allocating new buffers)
//       size_t written = 0;
//       while(size) {
//         if (_next == _list.end()) {
//           if (_buffer_size > 0) {
//             _list.emplace_back(_buffer_size);
//             if (!_list.back().size()) { // out of memory
//               _list.pop_back();
//               _valid = false;
//               return written;
//             }
//             _next = _list.end();
//             --_next;
//           } else {
//             _valid = false;
//             return written;
//           }
//         }
//         size_t to_write = std::min(_next->size() - _offset, size);
//         memcpy(_next->data() + _offset, buffer, to_write);
//         written += to_write;
//         buffer += to_write;
//         size -= to_write;
//         _offset += to_write;
//         if (_offset == _next->size()) {
//           ++_next;
//           _offset = 0;
//         }
//       }
//       return written;      
//     }

//     size_t write(uint8_t c) {
//       return this->write(&c, 1);
//     }

//     bool valid() const { return _valid; };
//     explicit operator bool() const { return valid(); };
// };

// typedef BufferListPrint<DynamicBufferList> DynamicBufferListPrint;
// typedef BufferListPrint<SharedBufferList> SharedBufferListPrint;

