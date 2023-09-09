#include "gamo/actor/actor.h"
#include "spdlog/spdlog.h"

namespace gamo
{
    Tilemap *Tilemap::Clone()
    {
        auto obj = new Tilemap(nullptr);
        obj->_cell_size = _cell_size;
        return obj;
    }

    void Tile::HandleInit(Event *e)
    {
        auto tilemap = SearchTilemap();
        if (tilemap == nullptr)
        {
            spdlog::error("fail to init tile, tilemap unfound");
            UnregisterHandleInit();
            return;
        }
        // set transform
        auto tile_offset = GlobalOffset(tilemap->CellSize());
        BelongActor()->GetTransform()->Position(tile_offset);

        // register terrain
        auto colliders = BelongActor()->GetCharacteristicsByType("Collider");
        for (auto item : colliders)
        {
            auto collider = (Collider *)item;
            auto shape = collider->ColliderShape();
            if (shape == nullptr)
                continue;
            auto terrain = dynamic_cast<Terrain*>(shape);
            if (terrain == nullptr)
                continue;
            tilemap->RegisterTerrain(terrain);
        }
        
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

    Tile *Tile::Clone()
    {
        auto obj = new Tile(nullptr);
        obj->_position = _position;
        obj->_anchor = _anchor;
        return obj;
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
