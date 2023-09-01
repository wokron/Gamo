#include "actor.h"
#include "tilemap.h"
#include "spdlog/spdlog.h"

namespace gamo
{
    void Tile::HandleInit(Event *e)
    {
        auto tilemap = SearchTilemap();
        if (tilemap == nullptr)
        {
            spdlog::error("fail to init tile, tilemap unfound");
            UnregisterHandleInit();
            return;
        }
        auto tile_offset = GlobalOffset(tilemap->CellSize());
        BelongActor()->GetTransform()->Position(tile_offset);
        UnregisterHandleInit();
    }

    Vect Tile::GlobalOffset(Vect cell_size)
    {
        assert(cell_size.x > 0 && cell_size.y > 0);
        auto total_position = Position() + Anchor();
        // corner_position is the positon of the lower left corner of the grid
        auto corner_position = cell_size * total_position;
        return corner_position + cell_size / 2;
    }

    Tilemap *Tile::SearchTilemap()
    {
        Actor *actor;
        Tilemap *tilemap;
        if ((actor = BelongActor()->SupActor()) == nullptr ||
            (tilemap = (Tilemap *)actor->GetCharacteristicByType("Tilemap")) == nullptr)
        {
            return nullptr;
        }

        return tilemap;
    }
} // namespace gamo
