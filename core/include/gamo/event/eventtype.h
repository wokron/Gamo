#pragma once

namespace gamo
{
    enum EventType
    {
        EVENT_BASE,
        EVENT_ON_START,
        EVENT_ON_UPDATE,
        EVENT_ON_LATE_UPDATE,
        EVENT_RENDER,
        EVENT_CAMERA_DETECT,
        EVENT_RIGIDBODY_INIT,
        EVENT_COLLIDER_INIT,
        EVENT_PHYSICS_BEFORE_STEP,
        EVENT_PHYSICS_AFTER_STEP,
        EVENT_MEM_FREE,
        EVENT_TILE_INIT,
    };
} // namespace gamo
