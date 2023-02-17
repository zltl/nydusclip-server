#pragma once

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

/**
   A thread containing postgres connection.
 */
class Conn : public boost::noncopyable {
 public:
  Conn(const std::string& url);

  template <class F, class... Args>
  void Do(F&& f, Args&&... args) {
    std::scoped_lock lock{mu_};
    auto fn = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    fn_ = fn;
  }

  void Start();

  void Stop();

 private:
  void WorkThread();

  std::string url_;
  std::mutex mu_{};
  std::condition_variable cond_{};
  std::stop_source stop_source_{};
  pqxx::connection conn_;
  std::jthread t_;
  std::optional<std::function<void()>> fn_;
};

}  // namespace db

}  // namespace nydus
