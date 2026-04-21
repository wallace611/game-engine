#pragma once

#include "object/scene_object.h"
#include <vector>
#include <unordered_map>
#include <utility>
#include <glm/glm.hpp>

class Collider;

// Define the cell structure for spatial hashing
struct GridCell {
    int x, y, z;

    // Equality operator required for unordered_map
    bool operator==(const GridCell& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

// Custom hash function for GridCell
struct GridCellHash {
    std::size_t operator()(const GridCell& cell) const {
        std::size_t h1 = std::hash<int>{}(cell.x);
        std::size_t h2 = std::hash<int>{}(cell.y);
        std::size_t h3 = std::hash<int>{}(cell.z);
        // Bitwise XOR and shift to combine hashes efficiently
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

class CollisionOptimizedScene : public Scene {
public:
    CollisionOptimizedScene();
    bool useSpeedUp = true;
    virtual void CollisionCheck() override;

private:
    // Configurable cell size. Tune this to roughly 2x-3x your average object size.
    static constexpr float CELL_SIZE = 10.0f; 
    
    // Flag to ensure the static grid is only built once
    bool isStaticGridBuilt = false;

    // Spatial grids mapped by GridCell
    std::unordered_map<GridCell, std::vector<Collider*>, GridCellHash> dynamicGrid;
    std::unordered_map<GridCell, std::vector<Collider*>, GridCellHash> staticGrid;

    // Pre-allocated buffers to prevent heap allocations during the game loop
    std::vector<GridCell> tempCellsBuffer;
    std::vector<std::pair<Collider*, Collider*>> dynDynPairsBuffer;
    std::vector<std::pair<Collider*, Collider*>> statDynPairsBuffer;

    // Helper functions
    GridCell WorldToCell(const glm::vec3& pos) const;
    
    // Calculates overlapping cells and writes to the provided vector (avoids allocations)
    void GetOverlappingCellsFast(Collider* collider, std::vector<GridCell>& outCells) const;
    
    void RebuildDynamicGrid();
    void RebuildStaticGrid();
    
    // Gathers potential collision pairs strictly from the same spatial cells
    void CollectCandidatePairs(
        std::vector<std::pair<Collider*, Collider*>>& dynDynPairs,
        std::vector<std::pair<Collider*, Collider*>>& statDynPairs);
};