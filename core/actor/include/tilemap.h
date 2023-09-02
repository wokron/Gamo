#pragma once

#include "characteristic.h"
#include "utils.h"
#include "assert.h"

namespace gamo
{
    class Tilemap : public Characteristic
    {
    private:
        Vect _cell_size = {1, 1};

    public:
        Tilemap(Actor *actor) : Characteristic(actor) {}
        Vect CellSize() { return _cell_size; }
        void CellSize(Vect cell_size)
        {
            assert(cell_size.x > 0 && cell_size.y > 0);
            _cell_size = cell_size;
        }

        std::string Type() override { return std::string("Tilemap"); }

        Tilemap *Clone() override;
    };

    class Tile : public Characteristic
    {
    private:
        Vect _position = {0, 0};
        Vect _anchor = {0, 0};

    public:
        Tile(Actor *actor) : Characteristic(actor) {}
        Vect Position() { return _position; }
        void Position(int x, int y) { _position = {(float)x, (float)y}; }

        Vect Anchor() { return _anchor; }
        void Anchor(Vect anchor) { _anchor = anchor; }

        Vect GlobalOffset(Vect cell_size);

        EVENT_DECLARE(Tile, HandleInit, EVENT_TILE_INIT);

        void RegisterEvents() override
        {
            RegisterHandleInit();
            Characteristic::RegisterEvents();
        }
        void UnregisterEvents() override
        {
            UnregisterHandleInit();
            Characteristic::UnregisterEvents();
        }

        std::string Type() override { return std::string("Tile"); }

        Tile *Clone() override;

    private:
        Tilemap *SearchTilemap();
    };

} // namespace gamo
