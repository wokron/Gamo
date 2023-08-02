#pragma once

#include <vector>
#include <string>
#include "resource.h"
#include "spdlog/spdlog.h"

namespace gamo
{
    class Actor;
    class Transform;

    class Characteristic : public IResource
    {
    private:
        Actor *_belong_actor;

    public:
        Characteristic(Actor *actor) { _belong_actor = actor; }

        Actor *BelongActor() { return _belong_actor; }

        std::vector<Characteristic *> &GetCharacteristics();

        Transform *GetTransform();

        /// @brief register events in characteristic
        virtual void RegisterEvents() {}

        /// @brief unregister events in characteristic
        virtual void UnregisterEvents() {}

        /// @brief get the type of the characteristic,
        /// this is used to solve the problem that there is no method like "instanceof" in c++
        /// @return the type of characteristic
        virtual std::string Type() = 0;

        void Destroy() override;
        void Deref() override;

        OVERRIDE_HANDLE_MEM_FREE(Characteristic)
        {
            UnregisterHandleMemFree();
            spdlog::info("characteristic[{}] is destroying...", fmt::ptr(this));
            delete this;
        }
    };
}