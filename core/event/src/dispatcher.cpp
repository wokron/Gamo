#include "event.h"

namespace gamo
{
    EventDispatcher *EventDispatcher::_instance = nullptr; // init static member

    EventHandle EventDispatcher::Append(int event_type, const std::function<CallBack> &callback)
    {
        return _dispatcher.appendListener(event_type, callback);
    }

    int EventDispatcher::Remove(int event_type, EventHandle identity)
    {
        return _dispatcher.removeListener(event_type, identity) ? 0 : -1;
    }

    void EventDispatcher::Dispatch(Event *e)
    {
        _dispatcher.dispatch(e);
    }

    void EventDispatcher::Dispatch(int event_type, Event *e)
    {
        _dispatcher.dispatch(event_type, e);
    }

} // namespace gamo
