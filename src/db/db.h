#pragma once

#include <option/option.h>

#include <format>
#include <iterator>
#include <memory>
#include <mutex>
#include <pqxx/pqxx>
#include <queue>
#include <semaphore>
#include <string_view>
#include <utility>

#include "db/conn.h"
#include "status/error.h"

namespace nydus {
namespace db {

/**
   Each thread contains a connection.
 */
class Pool {
 public:
  Pool(nydus::option::Db& conf)
      : url_{std::format("postgresql://{}@{}/{}", conf.username, conf.password,
                         conf.dbname)},
        max_connection_{conf.max_connections},
        max_idle_connection_{conf.max_idle_connections},
        conns_sem_{max_connection_} {}

  template <class F, class... Args>
  void Do(F&& f, Args&&... args) {
    conns_sem_.aquire();
    std::scoped_lock lock{mu_};
    std::shared_ptr<Conn> c{nullptr};
    if (idle_conns_.empty()) {
      c = std::make_shared<Conn>(url_);
      c->Start();
    } else {
      c = idle_conns_.front();
      idle_conns_.pop();
    }
    auto fwrap = [&]() {
      auto user_fn = std::bind(std::forward(f), c, std::forward(args)...);
      std::unique_lock lock{mu_};
      if (static_cast<int>(idle_conns_.size()) + 1 < max_idle_connection_) {
        idle_conns_.push(c);
      }
      busy_conns_num_--;
      lock.unlock();
      conns_sem_.release();
    };

    busy_conns_num_++;
    c->Do(fwrap);
  }

 private:
  std::mutex mu_;

  std::string url_;
  int max_connection_;
  int max_idle_connection_;
  int busy_conns_num_{0};
  std::counting_semaphore conns_sem_;
  std::queue<std::shared_ptr<Conn>> idle_conns_;
};

}  // namespace db
}  // namespace nydus
