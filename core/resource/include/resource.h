#pragma once

#include "event.h"

#define OVERRIDE_HANDLE_MEM_FREE(class) \
EVENT_DECLARE(class, HandleMemFree, EVENT_MEM_FREE) override

namespace gamo
{
    class IResource
    {
    public:
        virtual void Destroy() {}
        virtual void Deref() {}

        virtual void HandleMemFree(Event *e) {}
    };

    template <typename T>
    class IResourceSet
    {
    public:
        virtual void Remove(T *item) {}
        virtual void RemoveAll() {}
    };
} // namespace gamo
