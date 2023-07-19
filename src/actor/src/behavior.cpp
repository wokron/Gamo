#include "actor.h"
#include "event.h"

namespace gamo
{
    void Behavior::RegisterOnStart()
    {
        _onstart_handle = EventDispatcher::GetInstance()->Append(EVENT_ON_START, MEMBER_METHOD(this, &Behavior::OnStartListener));
    }

    void Behavior::UnregisterOnStart()
    {
        EventDispatcher::GetInstance()->Remove(EVENT_ON_START, _onstart_handle);
    }

    void Behavior::RegisterOnUpdate()
    {
        _onupdate_handle = EventDispatcher::GetInstance()->Append(EVENT_ON_UPDATE, MEMBER_METHOD(this, &Behavior::OnUpdateListener));
    }

    void Behavior::UnregisterOnUpdate()
    {
        EventDispatcher::GetInstance()->Remove(EVENT_ON_UPDATE, _onupdate_handle);
    }

    void Behavior::RegisterOnLateUpdate()
    {
        _onlateupdate_handle = EventDispatcher::GetInstance()->Append(EVENT_ON_LATE_UPDATE, MEMBER_METHOD(this, &Behavior::OnLateUpdateListener));
    }

    void Behavior::UnregisterOnLateUpdate()
    {
        EventDispatcher::GetInstance()->Remove(EVENT_ON_LATE_UPDATE, _onlateupdate_handle);
    }

} // namespace gamo
