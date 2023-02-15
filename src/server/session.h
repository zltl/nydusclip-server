#pragma once

#include "server/beast.h"
#include "server/handle.h"
#include "server/net.h"

namespace nydus {
namespace server {

using tcp_stream = typename beast::tcp_stream::rebind_executor<
    net::use_awaitable_t<>::executor_with_default<net::any_io_executor>>::other;

// Handles an HTTP server connection
net::awaitable<void> do_session(tcp_stream stream);

}  // namespace server
}  // namespace nydus
