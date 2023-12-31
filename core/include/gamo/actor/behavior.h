#pragma once

#include "characteristic_base.h"
#include "gamo/event/event.h"

namespace gamo
{
    class Collider;
    
    class Behavior : public Characteristic
    {
    private:
    public:
        Behavior(Actor *actor) : Characteristic(actor) {}
        ~Behavior() { OnDestroy(); }

        EVENT_DECLARE(Behavior, HandleOnStart, EVENT_ON_START) { OnStart(); UnregisterHandleOnStart(); }
        EVENT_DECLARE(Behavior, HandleOnUpdate, EVENT_ON_UPDATE) { OnUpdate(); }
        EVENT_DECLARE(Behavior, HandleOnLateUpdate, EVENT_ON_LATE_UPDATE) { OnLateUpdate(); }

        void RegisterEvents() override { RegisterHandleOnStart(); RegisterHandleOnUpdate(); RegisterHandleOnLateUpdate(); Characteristic::RegisterEvents(); }
        void UnregisterEvents() override { UnregisterHandleOnStart(); UnregisterHandleOnUpdate(); UnregisterHandleOnLateUpdate(); Characteristic::UnregisterEvents(); }

        virtual void OnStart() {}
        virtual void OnUpdate() {}
        virtual void OnLateUpdate() {}
        virtual void OnDestroy() {}

        virtual void OnCollisionBegin(Collider *other) {}
        virtual void OnCollisionEnd(Collider *other) {}

        std::string Type() { return std::string("Behavior"); }

        virtual Behavior *Clone() override { return new Behavior(nullptr); }
    };
} // namespace gamo
