#pragma once

#include <event2/event.h>

namespace nydus {
namespace co_event {

using EventType = short;

const EventType kEvTimeout = EV_TIMEOUT;
const EventType kEvRead = EV_READ;
const EventType kEvWrite = EV_WRITE;
const EventType kEvSignal = EV_SIGNAL;
const EventType kEvPersist = EV_PERSIST;
const EventType kEvET = EV_ET;
const EventType kEvFinalize = EV_FINALIZE;
const EventType kEvClose = EV_CLOSED;

}  // namespace co_event
}  // namespace nydus
