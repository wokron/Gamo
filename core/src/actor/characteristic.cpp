#include "gamo/actor/actor.h"

namespace gamo
{
    std::vector<Characteristic *> &Characteristic::GetCharacteristics()
    {
        return _belong_actor->GetCharacteristics();
    }

    Transform *Characteristic::GetTransform()
    {
        return _belong_actor->GetTransform();
    }

    void Characteristic::Destroy()
    {
        if (_belong_actor != nullptr)
        {
            _belong_actor->RemoveResource(this);
        }
    }

    void Characteristic::Deref()
    {
        UnregisterEvents();
        RegisterHandleMemFree();
    }

} // namespace gamo
