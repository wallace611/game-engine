#include "collision_optimized_scene.h"
#include "object/object.h"
#include "object/collision/collider.h"
#include "object/collision/aabb.h"
#include "object/collision/sphere.h"
#include <algorithm> // Required for std::sort and std::unique

CollisionOptimizedScene::CollisionOptimizedScene() : Scene() {
    // Reserve memory upfront to minimize allocations during runtime
    tempCellsBuffer.reserve(64);
    dynDynPairsBuffer.reserve(1024);
    statDynPairsBuffer.reserve(1024);
}

GridCell CollisionOptimizedScene::WorldToCell(const glm::vec3& pos) const {
    return {
        static_cast<int>(std::floor(pos.x / CELL_SIZE)),
        static_cast<int>(std::floor(pos.y / CELL_SIZE)),
        static_cast<int>(std::floor(pos.z / CELL_SIZE))
    };
}

void CollisionOptimizedScene::GetOverlappingCellsFast(Collider* collider, std::vector<GridCell>& outCells) const {
    outCells.clear(); // Clear previous data without releasing memory capacity

    glm::vec3 minP, maxP;

    // Fallback logic using dynamic_cast. 
    // For extreme performance, consider adding a virtual GetBoundingBox() method to Collider.
    if (const AABBCollider* aabb = dynamic_cast<const AABBCollider*>(collider)) {
        minP = aabb->GetGlobalMinPoint();
        maxP = aabb->GetGlobalMaxPoint();
    }
    else if (const SphereCollider* sphere = dynamic_cast<const SphereCollider*>(collider)) {
        glm::vec3 c = sphere->GetGlobalCenter();
        float r = sphere->GetGlobalRadius();
        minP = c - glm::vec3(r);
        maxP = c + glm::vec3(r);
    }
    else {
        // Treat as a single point if type is unknown
        minP = maxP = collider->GetGlobalPosition();
    }

    GridCell cMin = WorldToCell(minP);
    GridCell cMax = WorldToCell(maxP);

    // Populate the referenced vector with all intersecting cells
    for (int x = cMin.x; x <= cMax.x; ++x) {
        for (int y = cMin.y; y <= cMax.y; ++y) {
            for (int z = cMin.z; z <= cMax.z; ++z) {
                outCells.push_back({x, y, z});
            }
        }
    }
}

void CollisionOptimizedScene::RebuildStaticGrid() {
    staticGrid.clear();
    
    for (Collider* c : staticCollider) {
        GetOverlappingCellsFast(c, tempCellsBuffer);
        for (const GridCell& cell : tempCellsBuffer) {
            staticGrid[cell].push_back(c);
        }
    }
    isStaticGridBuilt = true;
}

void CollisionOptimizedScene::RebuildDynamicGrid() {
    // Only clear the dynamic grid; static objects do not move
    dynamicGrid.clear();

    for (Collider* c : dynamicCollider) {
        GetOverlappingCellsFast(c, tempCellsBuffer);
        for (const GridCell& cell : tempCellsBuffer) {
            dynamicGrid[cell].push_back(c);
        }
    }
}

void CollisionOptimizedScene::CollectCandidatePairs(
    std::vector<std::pair<Collider*, Collider*>>& dynDynPairs,
    std::vector<std::pair<Collider*, Collider*>>& statDynPairs) 
{
    dynDynPairs.clear();
    statDynPairs.clear();

    // Iterate exclusively through populated dynamic cells
    for (const auto& [cell, dynList] : dynamicGrid) {
        
        // 1. Dynamic vs Dynamic check (Same cell only)
        size_t numDyn = dynList.size();
        for (size_t i = 0; i < numDyn; ++i) {
            for (size_t j = i + 1; j < numDyn; ++j) {
                Collider* a = dynList[i];
                Collider* b = dynList[j];
                
                // Enforce canonical ordering (Pointer address comparison) to unify pairs like (A,B) and (B,A)
                if (a > b) {
                    std::swap(a, b);
                }
                dynDynPairs.push_back({a, b});
            }
        }

        // 2. Static vs Dynamic check (Same cell only)
        auto itS = staticGrid.find(cell);
        if (itS != staticGrid.end()) {
            const auto& statList = itS->second;
            for (Collider* dyn : dynList) {
                for (Collider* stat : statList) {
                    statDynPairs.push_back({stat, dyn});
                }
            }
        }
    }

    // Fast memory-contiguous deduplication for Dynamic-Dynamic pairs
    std::sort(dynDynPairs.begin(), dynDynPairs.end());
    dynDynPairs.erase(std::unique(dynDynPairs.begin(), dynDynPairs.end()), dynDynPairs.end());

    // Fast memory-contiguous deduplication for Static-Dynamic pairs
    std::sort(statDynPairs.begin(), statDynPairs.end());
    statDynPairs.erase(std::unique(statDynPairs.begin(), statDynPairs.end()), statDynPairs.end());
}

void CollisionOptimizedScene::CollisionCheck() {
    // Assuming 'useSpeedUp' is inherited from the base Scene class
    if (!useSpeedUp) {
        Scene::CollisionCheck();
        return;
    } 

    // Build static grid only once during the first accelerated frame
    if (!isStaticGridBuilt) {
        RebuildStaticGrid();
    }

    // Rebuild dynamic grid every frame as objects move
    RebuildDynamicGrid();

    // Collect pairs directly into pre-allocated member buffers
    CollectCandidatePairs(dynDynPairsBuffer, statDynPairsBuffer);

    // Narrow-phase execution: Dynamic vs Dynamic
    for (auto& [a, b] : dynDynPairsBuffer) {
        HitResult hit;
        if (a->CollideWith(b, hit)) {
            if (a->GetHitCallback()) {
                a->GetHitCallback()(a, b, hit);
            }
            if (b->GetHitCallback()) {
                // Reverse the normal from B's perspective
                hit.hitNormal = -hit.hitNormal;
                b->GetHitCallback()(b, a, hit);
            }
        }
    }

    // Narrow-phase execution: Static vs Dynamic
    for (auto& [stat, dyn] : statDynPairsBuffer) {
        HitResult hit;
        if (stat->CollideWith(dyn, hit)) {
            if (stat->GetHitCallback()) {
                stat->GetHitCallback()(stat, dyn, hit);
            }
            if (dyn->GetHitCallback()) {
                // Reverse the normal from Dynamic's perspective
                hit.hitNormal = -hit.hitNormal;
                dyn->GetHitCallback()(dyn, stat, hit);
            }
        }
    }
}