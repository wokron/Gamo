#pragma once

#include "event_base.h"
#include "dispatcher.h"
#include "eventtype.h"

/// @brief this macro provide a easy way to declare a event, 
/// for example, EVENT_DECLARE(Renderer, HandleRender, EVENT_RENDER)
/// declare a function HandleRender with two tool functions RegisterHandleRender and UnregisterHandleRender
/// @param class which class this declare in, because there is't macro like __class__ in c/c++
/// @param func_name the exact function to declare
/// @param event_type the event type bind to the function, type is int or EventType
#define EVENT_DECALRE(class, func_name, event_type)                                                        \
private:                                                                                                   \
    EventHandle _##func_name##_handle;                                                                     \
                                                                                                           \
public:                                                                                                    \
    void Register##func_name()                                                                             \
    {                                                                                                      \
        _##func_name##_handle =                                                                            \
            EventDispatcher::GetInstance()->Append((event_type), MEMBER_METHOD(this, &class ::func_name)); \
    }                                                                                                      \
    void Unregister##func_name()                                                                           \
    {                                                                                                      \
        EventDispatcher::GetInstance()->Remove((event_type), _##func_name##_handle);                       \
    }                                                                                                      \
    void(func_name)(Event * e)
