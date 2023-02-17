#include "db/conn.h"

#include <condition_variable>
#include <functional>
#include <mutex>
#include <pqxx/pqxx>
#include <queue>
#include <semaphore>
#include <sstream>
#include <stop_token>
#include <string_view>
#include <thread>
#include <utility>

#include "boost/core/noncopyable.hpp"
#include "log/log.h"

namespace nydus {
namespace db {

Conn::Conn(const std::string& url) : url_{url} {}

void Conn::Start() {
  t_ = std::jthread{[this] { WorkThread(); }};
  stop_source_ = std::stop_source{};
}

void Conn::Stop() {
  stop_source_.request_stop();
  t_.join();
}

void Conn::WorkThread() {

  // print thread id
  auto tid = std::this_thread::get_id();
  std::stringstream ss;
  ss << tid;
  DEBUG("conn thread {} start", ss.str());

  conn_ = pqxx::connection{url_};

  auto token = stop_source_.get_token();
  std::stop_callback cb(token, [this]() {
    cond_.notify_all();  // Wake thread on stop request
  });

  for (;;) {
    std::unique_lock lock{mu_};
    cond_.wait(lock, [this, token]() {
      return fn_.has_value() || token.stop_requested();
    });
    if (token.stop_requested()) {
      DEBUG("conn thread {} stopping", ss.str());
      break;
    }

    if (fn_.has_value()) {
      auto f = fn_.value();
      f();
      fn_.reset();
    }
  }
}

}  // namespace db
}  // namespace nydus
