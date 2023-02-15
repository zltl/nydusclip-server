#pragma once

#include <algorithm>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>
#include <boost/core/noncopyable.hpp>
#include <cstdlib>
#include <exception>
#include <expected>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

#include "log/log.h"
#include "option/option.h"
#include "server/beast.h"
#include "server/mime.h"
#include "server/net.h"
#include "status/error.h"

namespace nydus {
namespace server {

class Server : public boost::noncopyable {
 public:
  Server(int threads, std::string_view addr, unsigned short port);
  ~Server();
  status::Result<Void> Start();

 private:
  net::io_context ioc_;
  net::ip::address address_;
  unsigned short port_;
  int threads_;
};

void Tx();

}  // namespace server
}  // namespace nydus
