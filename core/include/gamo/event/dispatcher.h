#pragma once

#include "eventpp/eventdispatcher.h"
#include "event_base.h"
#include "gamo/utils/utils.h"

/// @brief this macro is used to represent a member function (such as "object.func()")
/// as a parameter of the Append function of EventDispatcher,
/// since member functions cannot be used as callback
/// @param objptr the pointer to the object whose member function you want to pass
/// @param method the member method itself, this param should be like this: "&YourClass::your_method"
#define MEMBER_METHOD(objptr, method) (std::bind((method), (objptr), std::placeholders::_1))

namespace gamo
{
    struct EventPolicies
    {
        static int getEvent(Event *e)
        {
            return e != nullptr ? e->Type() : 0;
        }
    };

    using CallBack = void (Event*);
    using BareDispatcher = eventpp::EventDispatcher<int, CallBack, EventPolicies>;
    using EventHandle = BareDispatcher::Handle;

    SINGLETON_BEGIN(EventDispatcher)
    private:
        BareDispatcher _dispatcher;

    public:
        /// @brief add a new listener to the specified event
        /// @param event_type the event type of the specified event (the type can be int or EventType)
        /// @param callback the callback function as listener
        /// @return an unique object indicating the newly added listener
        EventHandle Append(int event_type, const std::function<CallBack> &callback);

        /// @brief remove the listener in the specified event
        /// @param event_type the event type of the specified event (the type can be int or EventType)
        /// @param identity the unique object indicating the listener
        /// @return 0 on success, < 0 on listener not found
        int Remove(int event_type, EventHandle identity);

        /// @brief dispatch an event, use the type method of the event object to specify which event to dispatch
        /// @param e should be a child class of Event, which can include some infomation on it
        void Dispatch(Event *e);

        /// @brief dispatch an event, but instead of using the Type method, 
        /// just specify the event with event_type param
        /// (this is useful if you don't want to send the Event object)
        /// @param event_type the event type of the specified event (the type can be int or EventType)
        /// @param e should be a child class of Event, which can include some infomation on it
        void Dispatch(int event_type, Event *e);

    private:
        EventDispatcher() {}
    SINGLETON_END

} // namespace gamo
