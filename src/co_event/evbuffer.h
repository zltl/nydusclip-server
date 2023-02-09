#pragma once

#include <event2/buffer.h>

#include <exception>
#include <expected>
#include <format>
#include <stdexcept>

#include "boost/core/noncopyable.hpp"
#include "co_event/buffer.h"
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
class EvBuffer : public boost::noncopyable {
 public:
  EvBuffer() : buffer_{evbuffer_new()} {}
  EvBuffer(EvBuffer&& other) : buffer_{other.buffer_} {
    other.buffer_ = nullptr;
  }
  EvBuffer& operator=(EvBuffer&& other) {
    buffer_ = other.buffer_;
    other.buffer_ = nullptr;
    return *this;
  }

  ~EvBuffer() {
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
  Result<Void> Add(BufferRef bf) { return Add(bf.data(), bf.size()); }

  Result<size_t> MoveTo(BufferRef& bf) {
    int ncopy = evbuffer_remove(buffer_, bf.data(), bf.size());
    if (ncopy >= 0) {
      bf.set_size(ncopy);
      return ncopy;
    }
    return unexpected(Error{Code::kLibeventError,
                            "evbuffer_remove() - we can't drain the buffer"});
  }

  Result<Void> MoveTo(EvBuffer& other, size_t len) {
    int ncopy = evbuffer_remove_buffer(buffer_, other.buffer_, len);
    if (ncopy >= 0) {
      return ncopy;
    }
    return unexpected(Error{Code::kLibeventError,
                            "evbuffer_remove() - we can't drain the buffer"});
  }

  Result<Void> CopyTo(BufferRef& bf) {
    int ncopy = evbuffer_copyout(buffer_, bf.data(), bf.size());
    if (ncopy >= 0) {
      bf.set_size(ncopy);
      return {};
    }
    return unexpected(Error{Code::kLibeventError,
                            "evbuffer_remove() - we can't drain the buffer"});
  }

  char& at(size_t i) {
    if (i < 0 || i >= Length()) {
      throw std::out_of_range(std::format(
          "index {} out of range of buffer length {}", i, Length()));
    }

    evbuffer_ptr ptr;
    int r = evbuffer_ptr_set(buffer_, &ptr, i, EVBUFFER_PTR_SET);
    if (r != 0) {
      throw std::out_of_range(std::format(
          "cannot set position of index {} to buffer length {}", i, Length()));
    }

    // TODO: not reference to interner_
    return ((char*)ptr.internal_.chain)[ptr.internal_.pos_in_chain];
  }

  char& operator[](size_t i) { return at(i); }

  Result<Void> Drain(size_t len) {
    int r = evbuffer_drain(buffer_, len);
    if (r == 0) {
      return {};
    }
    return unexpected(
        Error{Code::kLibeventError, "evbuffer_drain() - failured"});
  }

 private:
  evbuffer* buffer_{};
};

}  // namespace co_event

}  // namespace nydus
