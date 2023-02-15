#include "server/session.h"
#include "boost/beast/http/buffer_body.hpp"
#include "boost/beast/http/dynamic_body.hpp"
#include "server/net.h"
#include "server/beast.h"

namespace nydus {
namespace server {

// Handles an HTTP server connection
net::awaitable<void> do_session(tcp_stream stream) {
  beast::error_code ec;

  // This buffer is required to persist across reads
  beast::flat_buffer buffer;

  // This lambda is used to send messages
  for (;;) try {
      // Set the timeout.
      stream.expires_after(std::chrono::seconds(30));

      // Read a request
      RequestType req;
      co_await http::async_read(stream, buffer, req);

      // Handle the request
      http::message_generator msg = handle_request(".", std::move(req));

      // Determine if we should close the connection
      bool keep_alive = msg.keep_alive();

      // Send the response
      co_await beast::async_write(stream, std::move(msg), net::use_awaitable);

      if (!keep_alive) {
        // This means we should close the connection, usually because
        // the response indicated the "Connection: close" semantic.
        break;
      }
    } catch (boost::system::system_error& se) {
      if (se.code() != http::error::end_of_stream) throw;
    }

  // Send a TCP shutdown
  stream.socket().shutdown(tcp::socket::shutdown_send, ec);

  // At this point the connection is closed gracefully
}

}  // namespace server
}  // namespace nydus
