#pragma once

#include <event2/buffer.h>

#include <expected>

#include "boost/core/noncopyable.hpp"
#include "status/error.h"

namespace nydus {
namespace co_event {

using nydus::status::Code;
using nydus::status::Error;
using nydus::status::Result;
using std::unexpected;

/**
   Wrapper of libevent::evbuffer
 */
class Buffer : public boost::noncopyable {
 public:
  Buffer() : buffer_{evbuffer_new()} {}
  Buffer(Buffer&& other) : buffer_{other.buffer_} { other.buffer_ = nullptr; }
  Buffer& operator=(Buffer&& other) {
    buffer_ = other.buffer_;
    other.buffer_ = nullptr;
    return *this;
  }

  ~Buffer() {
    if (buffer_) {
      evbuffer_free(buffer_);
      buffer_ = nullptr;
    }
  }

  Result<void> SetMaxRead() {
    return {};
  }

 private:
  evbuffer* buffer_{};
};

}  // namespace co_event

}  // namespace nydus
