#include "server/listen.h"

#include "server/beast.h"
#include "server/net.h"
#include "server/session.h"

namespace nydus {
namespace server {

// Accepts incoming connections and launches the sessions
net::awaitable<void> do_listen(tcp::endpoint endpoint) {
  // Open the acceptor
  auto acceptor = net::use_awaitable.as_default_on(
      tcp::acceptor(co_await net::this_coro::executor));
  acceptor.open(endpoint.protocol());

  // Allow address reuse
  acceptor.set_option(net::socket_base::reuse_address(true));

  // Bind to the server address
  acceptor.bind(endpoint);

  // Start listening for connections
  acceptor.listen(net::socket_base::max_listen_connections);

  for (;;)
    boost::asio::co_spawn(
        acceptor.get_executor(),
        do_session(tcp_stream(co_await acceptor.async_accept())),
        [](std::exception_ptr e) {
          try {
            std::rethrow_exception(e);
          } catch (std::exception& e) {
            std::cerr << "Error in session: " << e.what() << "\n";
          }
        });
}

}  // namespace server
}  // namespace nydus
