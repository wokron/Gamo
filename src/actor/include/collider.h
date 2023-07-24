#pragma once

#include "characteristic.h"
#include "physics.h"
#include "event.h"

namespace gamo
{
    class Collider : public Characteristic
    {
    private:
        FixtureDef _fixturedef;
        b2Fixture *_fixture = nullptr;

    public:
        Collider(Actor *actor) : Characteristic(actor) {}
        ~Collider();
        FixtureDef &Define() { return _fixturedef; }
        b2Fixture *Fixture() { return _fixture; }

        EVENT_DECALRE(Collider, HandleInit, EVENT_COLLIDER_INIT);
        EVENT_DECALRE(Collider, HandleBeforeStep, EVENT_PHYSICS_BEFORE_STEP);

        void RegisterEvents() override { RegisterHandleInit(); RegisterHandleBeforeStep(); }
        void UnregisterEvents() override { UnregisterHandleInit(); UnregisterHandleBeforeStep(); }

        std::string Type() override { return std::string("Collider"); }
    };

} // namespace gamo
