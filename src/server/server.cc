#include "server/server.h"

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
#include "server/listen.h"

namespace nydus {
namespace server {


Server::Server(int threads, std::string_view addr, unsigned short port)
    : ioc_{threads},
      address_{net::ip::make_address(addr)},
      port_{port},
      threads_{threads} {}
Server::~Server() { ioc_.stop(); }

status::Result<Void> Server::Start() {
  // Spawn a listening port
  boost::asio::co_spawn(ioc_, do_listen(tcp::endpoint{address_, port_}),
                        [](std::exception_ptr e) {
                          if (e) try {
                              std::rethrow_exception(e);
                            } catch (std::exception& e) {
                              ERROR("Error in acceptor: {}", e.what());
                            }
                        });
  // Run the I/O service on the requested number of threads
  std::vector<std::thread> v;
  v.reserve(threads_ - 1);
  for (auto i = threads_ - 1; i > 0; --i) {
    v.emplace_back([this] { ioc_.run(); });
  }

  try {
    ioc_.run();
    return {};
  } catch (std::exception& e) {
    ERROR("context run error: {}", e.what());
  }
  return std::unexpected(Error{Code::kAsioError, "ioc_.run() - error"});
}

void Tx() {
  using Conf = nydus::option::Conf;
  auto conf = Conf::conf;

  Server s{conf().server.io_threads, conf().server.addr,
           static_cast<unsigned short>(conf().server.port)};
  s.Start();
}

}  // namespace server
}  // namespace nydus
