#include "collision_optimized_scene.h"
#include "object/object.h"
#include "object/collision/collider.h"
#include "object/collision/aabb.h"
#include "object/collision/sphere.h"
#include <unordered_set>

CollisionOptimizedScene::CollisionOptimizedScene() : Scene() {}

// ─── Grid helpers ─────────────────────────────────────────────────────────────

GridCell CollisionOptimizedScene::WorldToCell(const glm::vec3& pos) const {
    return {
        (int)std::floor(pos.x / CELL_SIZE),
        (int)std::floor(pos.y / CELL_SIZE),
        (int)std::floor(pos.z / CELL_SIZE)
    };
}

std::vector<GridCell> CollisionOptimizedScene::GetOverlappingCells(Collider* collider) const {
    glm::vec3 minP, maxP;

    if (const AABBCollider* aabb = dynamic_cast<const AABBCollider*>(collider)) {
        minP = aabb->GetGlobalMinPoint();
        maxP = aabb->GetGlobalMaxPoint();
    }
    else if (const SphereCollider* sphere = dynamic_cast<const SphereCollider*>(collider)) {
        glm::vec3 c = sphere->GetGlobalCenter();
        float     r = sphere->GetGlobalRadius();
        minP = c - glm::vec3(r);
        maxP = c + glm::vec3(r);
    }
    else {
        // Fallback: treat as a point at the collider's world position
        minP = maxP = collider->GetGlobalPosition();
    }

    GridCell cMin = WorldToCell(minP);
    GridCell cMax = WorldToCell(maxP);

    std::vector<GridCell> cells;
    for (int x = cMin.x; x <= cMax.x; ++x)
        for (int y = cMin.y; y <= cMax.y; ++y)
            for (int z = cMin.z; z <= cMax.z; ++z)
                cells.push_back({x, y, z});

    return cells;
}

void CollisionOptimizedScene::RebuildGrid() {
    dynamicGrid.clear();
    staticGrid.clear();

    for (Collider* c : dynamicCollider)
        for (const GridCell& cell : GetOverlappingCells(c))
            dynamicGrid[cell].push_back(c);

    for (Collider* c : staticCollider)
        for (const GridCell& cell : GetOverlappingCells(c))
            staticGrid[cell].push_back(c);
}

// ─── Candidate pair collection (avoids duplicates via pointer-ordered set) ───

namespace {
    struct PairHash {
        size_t operator()(const std::pair<Collider*, Collider*>& p) const {
            auto h1 = std::hash<Collider*>{}(p.first);
            auto h2 = std::hash<Collider*>{}(p.second);
            return h1 ^ (h2 << 32 | h2 >> 32);
        }
    };
}

void CollisionOptimizedScene::CollectCandidatePairs(
    std::vector<std::pair<Collider*, Collider*>>& dynDynPairs,
    std::vector<std::pair<Collider*, Collider*>>& statDynPairs) const
{
    // Use ordered pointer pairs as keys to deduplicate (A,B) == (B,A)
    std::unordered_set<std::pair<Collider*,Collider*>, PairHash> seenDynDyn;
    std::unordered_set<std::pair<Collider*,Collider*>, PairHash> seenStatDyn;

    // For every dynamic collider, look into its occupied cells and all 26 neighbors
    for (Collider* dynC : dynamicCollider) {
        for (const GridCell& cell : GetOverlappingCells(dynC)) {
            // Check 3×3×3 neighborhood (27 cells including the cell itself)
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dz = -1; dz <= 1; ++dz) {
                        GridCell neighbor{cell.x+dx, cell.y+dy, cell.z+dz};

                        // dynamic vs dynamic
                        auto itD = dynamicGrid.find(neighbor);
                        if (itD != dynamicGrid.end()) {
                            for (Collider* other : itD->second) {
                                if (other == dynC) continue;
                                // Canonical ordering to avoid (A,B) and (B,A) duplicates
                                Collider* a = (dynC < other) ? dynC : other;
                                Collider* b = (dynC < other) ? other : dynC;
                                if (seenDynDyn.insert({a, b}).second)
                                    dynDynPairs.push_back({a, b});
                            }
                        }

                        // static vs dynamic
                        auto itS = staticGrid.find(neighbor);
                        if (itS != staticGrid.end()) {
                            for (Collider* stat : itS->second) {
                                auto key = std::make_pair(stat, dynC);
                                if (seenStatDyn.insert(key).second)
                                    statDynPairs.push_back(key);
                            }
                        }
                    }
                }
            }
        }
    }
}

// ─── CollisionCheck ───────────────────────────────────────────────────────────

void CollisionOptimizedScene::CollisionCheck() {
    if (!useSpeedUp) {
        Scene::CollisionCheck();
    } else {
        // ── Spatial-partition path ────────────────────────────────────────────
        // rebuild the grid every frame (colliders move)
        RebuildGrid();

        // collect candidate pairs from neighboring cells only
        std::vector<std::pair<Collider*, Collider*>> dynDynPairs, statDynPairs;
        CollectCandidatePairs(dynDynPairs, statDynPairs);

        // narrow-phase test on candidates only
        for (auto& [a, b] : dynDynPairs) {
            HitResult hit;
            if (a->CollideWith(b, hit)) {
                if (a->GetHitCallback()) a->GetHitCallback()(a, b, hit);
                if (b->GetHitCallback()) {
                    hit.hitNormal = -hit.hitNormal;
                    b->GetHitCallback()(b, a, hit);
                }
            }
        }
        for (auto& [stat, dyn] : statDynPairs) {
            HitResult hit;
            if (stat->CollideWith(dyn, hit)) {
                if (stat->GetHitCallback()) stat->GetHitCallback()(stat, dyn, hit);
                if (dyn->GetHitCallback()) {
                    hit.hitNormal = -hit.hitNormal;
                    dyn->GetHitCallback()(dyn, stat, hit);
                }
            }
        }
    }
}