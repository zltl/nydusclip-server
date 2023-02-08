#pragma once

#include <event2/buffer.h>

#include <exception>
#include <expected>

#include "boost/core/noncopyable.hpp"
#include "status/error.h"

namespace nydus {
namespace co_event {

using nydus::status::Code;
using nydus::status::Error;
using nydus::status::Result;
using nydus::status::Void;
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

  size_t Length() { return evbuffer_get_length(buffer_); }

  Result<Void> Add(const void* data_in, size_t len) {
    int r = evbuffer_add(buffer_, data_in, len);
    if (r == 0) {
      return {};
    }
    return std::unexpected(
        Error{Code::kLibeventError, "evbuffer_add() - failure"});
  }

 private:
  evbuffer* buffer_{};
};

}  // namespace co_event

}  // namespace nydus
