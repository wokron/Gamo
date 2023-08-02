#include "gtest/gtest.h"
#include "scene.h"
#include "actor.h"
#include "event.h"
#include "spdlog/spdlog.h"

using namespace gamo;

TEST(TestMemFree, test_actor_destroy)
{
    auto s1 = new Scene();
    auto a1 = s1->CreateActor({0, 0}, 0, {1, 1});
    auto a2 = s1->CreateActor({0, 0}, 0, {1, 1});
    auto s2 = new Scene();
    auto a3 = s2->CreateActor({0, 0}, 0, {1, 1});

    a1->Destroy();
    a2->Destroy();
    spdlog::info("call destroy");

    spdlog::info("dispatching mem free event, target {}, {}", fmt::ptr(a1), fmt::ptr(a2));
    EventDispatcher::GetInstance()->Dispatch(EVENT_MEM_FREE, nullptr);

}

TEST(TestMemFree, test_subactor_destroy)
{
    auto s1 = new Scene();
    auto a1 = s1->CreateActor({0, 0}, 0, {1, 1});
    auto a2 = a1->CreateSubActor({0, 0}, 0, {1, 1});
    auto s2 = new Scene();
    auto a3 = s2->CreateActor({0, 0}, 0, {1, 1});

    a1->Destroy();
    spdlog::info("call destroy");

    spdlog::info("dispatching mem free event, target {}, {}", fmt::ptr(a1), fmt::ptr(a2));
    EventDispatcher::GetInstance()->Dispatch(EVENT_MEM_FREE, nullptr);
}
