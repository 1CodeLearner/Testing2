#pragma once

#include "ecs/ecs.h"
#include <queue>
#include "Components.h"
#include <fstream>
#include <exception>
#include "Constants.h"

struct Tile {
    int x, y, w, h;
    uint8_t textureId;
};

class Map {

public:
    Map() = delete;

    explicit Map(std::string&& mapPath) {
        std::cout << "Parsing" << std::endl;
        std::ifstream infile(mapPath, std::ios::out | std::ios::binary);
        if (!infile) throw std::invalid_argument("Map path not found: " + mapPath);

        uint8_t texture;

        uint16_t mapWidth;
        infile.read(reinterpret_cast<char*>(&mapWidth), sizeof(uint16_t));
        uint16_t mapHeight;
        infile.read(reinterpret_cast<char*>(&mapHeight), sizeof(uint16_t));

        for (int x = 0; x < mapWidth; x++) {
            for (int y = 0; y < mapHeight; y++) {
                infile.read(reinterpret_cast<char*>(&texture), sizeof(uint8_t));
                if (texture != 0) {
                    auto tile = new Tile{x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, texture};
                    tiles.emplace(tile);
                }
            }
        }

        infile.close();
    }

    std::queue<Tile*> tiles;
};

class MapSystem : public System {
public:

    MapSystem();

    void onAddedToWorld(World* world) override;

    void tick(World* world) override;

    void onRemovedFromWorld(World* world) override;

    ~MapSystem() override;

private:
    Map map;
    CameraComponent* camera{};
};