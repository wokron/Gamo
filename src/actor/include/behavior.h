#pragma once

#include "characteristic_base.h"
#include "event.h"

namespace gamo
{
    class Behavior : public Characteristic
    {
    private: 
        EventHandle _onstart_handle;
        EventHandle _onupdate_handle;
        EventHandle _onlateupdate_handle;

    public:
        Behavior(Actor *actor) : Characteristic(actor) {}
        ~Behavior() { OnDestroy(); }
        void OnStartListener(Event *e) { OnStart(); }
        void OnUpdateListener(Event *e) { OnUpdate(); }
        void OnLateUpdateListener(Event *e) { OnLateUpdate(); }
        
        virtual void OnStart() {}
        virtual void OnUpdate() {}
        virtual void OnLateUpdate() {}
        virtual void OnDestroy() {}

        void RegisterOnStart();
        void UnregisterOnStart();
        void RegisterOnUpdate();
        void UnregisterOnUpdate();
        void RegisterOnLateUpdate();
        void UnregisterOnLateUpdate();

        std::string Type() { return std::string("Behavior"); }
    };
} // namespace gamo
