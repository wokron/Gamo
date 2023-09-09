#include "gtest/gtest.h"
#include "gamo/actor/actor.h"

using namespace gamo;

TEST(TestTilemap, test_position)
{
    auto tile = new Tile(nullptr);
    auto offset = tile->GlobalOffset({1, 1});
    ASSERT_FLOAT_EQ(offset.x, 0.5);
    ASSERT_FLOAT_EQ(offset.y, 0.5);

    tile->Position(1, 2);
    offset = tile->GlobalOffset({1, 1});
    ASSERT_FLOAT_EQ(offset.x, 1.5);
    ASSERT_FLOAT_EQ(offset.y, 2.5);

    tile->Anchor({0.5, 0.5});
    offset = tile->GlobalOffset({1, 1});
    ASSERT_FLOAT_EQ(offset.x, 2);
    ASSERT_FLOAT_EQ(offset.y, 3);

    offset = tile->GlobalOffset({2, 1});
    ASSERT_FLOAT_EQ(offset.x, 4);
    ASSERT_FLOAT_EQ(offset.y, 3);

    tile->Position(-1, -1);
    tile->Anchor({0, 0});
    offset = tile->GlobalOffset({1, 1});
    ASSERT_FLOAT_EQ(offset.x, -0.5);
    ASSERT_FLOAT_EQ(offset.y, -0.5);
}

TEST(TestTilemap, test_tile_init_event)
{
    auto tilemap_actor = new Actor({0, 0}, 0, {1, 1});
    auto tilemap = new Tilemap(tilemap_actor);
    tilemap_actor->AddCharacteristic(tilemap);
    auto tile_actor = tilemap_actor->CreateSubActor({0, 0}, 0, {1, 1});
    auto tile = new Tile(tile_actor);
    tile_actor->AddCharacteristic(tile);
    tilemap->CellSize({1, 2});
    tile->Position(2, 1);
    tile->Anchor({0, 0.5});

    tile->RegisterEvents();

    EventDispatcher::GetInstance()->Dispatch(EVENT_TILE_INIT, nullptr);

    auto pos = tile_actor->GetTransform()->Position();
    ASSERT_FLOAT_EQ(pos.x, 2.5);
    ASSERT_FLOAT_EQ(pos.y, 4);
}
