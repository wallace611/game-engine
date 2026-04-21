#pragma once
#include "object/scene_object.h"
#include "object/collision/aabb.h"
#include <unordered_map>
#include <list>

// Key for a 3D grid cell
struct GridCell {
    int x, y, z;
    bool operator==(const GridCell& o) const {
        return x == o.x && y == o.y && z == o.z;
    }
};

struct GridCellHash {
    size_t operator()(const GridCell& c) const {
        // A simple but decent hash combining the three integers
        size_t h = (size_t)(c.x * 73856093) ^ (size_t)(c.y * 19349663) ^ (size_t)(c.z * 83492791);
        return h;
    }
};

class CollisionOptimizedScene : public Scene {
protected:
    static constexpr float CELL_SIZE = 3.0f; // 3m × 3m × 3m

    bool useSpeedUp = false;

    // Maps each grid cell to the list of colliders whose AABB overlaps that cell
    using CellMap = std::unordered_map<GridCell, std::list<Collider*>, GridCellHash>;
    CellMap dynamicGrid;
    CellMap staticGrid;

    // Convert world position to grid cell index
    GridCell WorldToCell(const glm::vec3& pos) const;

    // Get all cells an AABB overlaps (a collider can span multiple cells)
    std::vector<GridCell> GetOverlappingCells(Collider* collider) const;

    // Rebuild the spatial grid from the current collider lists
    void RebuildGrid();

    // Collect unique candidate pairs from the grid (avoids duplicate checks)
    // Returns pairs (a, b) where a appears in dynamicCollider
    void CollectCandidatePairs(
        std::vector<std::pair<Collider*, Collider*>>& dynDynPairs,
        std::vector<std::pair<Collider*, Collider*>>& statDynPairs) const;

public:
    CollisionOptimizedScene();

    // Toggle between exhaustive O(n²) and spatial-partition collision detection
    void SetSpeedUp(bool enabled) { useSpeedUp = enabled; }
    bool IsSpeedUp() const { return useSpeedUp; }

    virtual void CollisionCheck() override;
};