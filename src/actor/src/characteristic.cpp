#include "actor.h"

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
} // namespace gamo
