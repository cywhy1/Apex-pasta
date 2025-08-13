#include "pch.h"
#include "Trigger.hpp"
#include "Vector3D.hpp"
#include "QAngle.hpp"
#include "Resolver.hpp"
#include "Conversion.hpp"
#include "Level.hpp"

TriggerBot::TriggerBot(LocalPlayer* lp, std::vector<Player*>* p, Camera* cam, c_keys* input, Level* map) 
    : localPlayer(lp), players(p), gameCamera(cam), inputManager(input), Map(map) {
    srand(static_cast<unsigned int>(time(nullptr)));
    lastShotTime = std::chrono::steady_clock::now();
}

void TriggerBot::Update() {
    if (!enableTriggerBot) return;
    
    UpdateAntiDetection();
    
    if (!inputManager->IsKeyDown(triggerKey)) return;
    
    if (!localPlayer || !localPlayer->IsCombatReady()) return;
    if (localPlayer->IsHoldingGrenade || localPlayer->IsReloading) return;
    
    if (localPlayer->IsInAttack) {
        if (debugMode) printf("[DEBUG] Already firing, skipping\n");
        return;
    }
    
    Player* target = FindTargetAtCrosshair();
    if (!target || !ShouldTrigger(target)) return;
    
    ExecuteTrigger();
}

Player* TriggerBot::FindTargetAtCrosshair() {
    if (!players) return nullptr;
    
    for (auto& player : *players) {
        if (!IsValidTarget(player)) continue;
        
        if (debugMode) {
            printf("[DEBUG] Player valid - IsAimedAt: %s, Distance: %.1f\n", 
                   player->IsAimedAt ? "true" : "false", 
                   player->DistanceToLocalPlayer);
        }
        
        if (player->IsAimedAt) {
            if (debugMode) printf("[DEBUG] Found aimed target!\n");
            return player;
        }
    }
    
    if (debugMode) printf("[DEBUG] No aimed targets found\n");
    return nullptr;
}

bool TriggerBot::IsValidTarget(Player* player) {
    if (!player || !player->IsValid()) return false;
    if (player->Health <= 0) return false;
    
    float distance = localPlayer->LocalOrigin.Distance(player->LocalOrigin);
    if (distance > Conversion::ToGameUnits(maxTriggerDistance)) return false;
    
    if (player->IsDummy() && Map->IsFiringRange) {
        return player->IsVisible;
    }
    
    if (!player->IsHostile || !player->IsVisible) return false;
    
    return true;
}

float TriggerBot::CalculateAngleFromCrosshair(Player* player) {
    Vector3D cameraPos = localPlayer->CameraPosition;
    QAngle viewAngles = QAngle(localPlayer->ViewAngles.x, localPlayer->ViewAngles.y + yawBiasDegrees);

    int testBones[] = { (int)HitboxType::Head, 3, 4, 5, 6, 7, 8 };
    float best = 9999.0f;
    for (int i = 0; i < (int)(sizeof(testBones) / sizeof(testBones[0])); ++i) {
        Vector3D pos = player->GetBonePosition(static_cast<HitboxType>(testBones[i]));
        if (pos.IsValid()) {
            QAngle targetAngle = Resolver::CalculateAngle(cameraPos, pos);
            float d = viewAngles.distanceTo(targetAngle);
            if (d < best) best = d;
        }
    }
    return best;
}

bool TriggerBot::ShouldTrigger(Player* target) {
    if (!target) return false;
    if (!inputManager->IsReactionTimeValid()) return false;
    
    if (!target->IsAimedAt) {
        if (debugMode) printf("[DEBUG] Not aimed at target\n");
        return false;
    }
    
    if (target->DistanceToLocalPlayer > Conversion::ToGameUnits(maxTriggerDistance)) {
        if (debugMode) printf("[DEBUG] Target too far: %.1f\n", target->DistanceToLocalPlayer);
        return false;
    }
    
    if (debugMode) printf("[DEBUG] Triggering on target!\n");
    return true;
}

void TriggerBot::ExecuteTrigger() {
    std::this_thread::sleep_for(std::chrono::milliseconds(triggerDelayMs));

    if (SendInAttackClick()) {
        lastShotTime = std::chrono::steady_clock::now();
    }
}

bool TriggerBot::IsHumanLikeTiming() { return true; }

void TriggerBot::UpdateAntiDetection() {
}

bool TriggerBot::SendInAttackClick() {
    auto now = std::chrono::steady_clock::now();
    if (lastInAttackWrite.time_since_epoch().count() != 0) {
        auto since = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastInAttackWrite).count();
        if (since >= 0 && since < minInAttackIntervalMs) return false;
    }

    uint64_t addr = mem.OFF_BASE + OFF_INATTACK + 0x8;

    uint32_t cur = 0;
    mem.Read(static_cast<uintptr_t>(addr), &cur, sizeof(cur), mem.current_process.PID);

    const uint32_t release = 4, press = 5;

    if (cur != release) {
        uint32_t v = release;
        if (!mem.Write(static_cast<uintptr_t>(addr), &v, sizeof(v), mem.current_process.PID)) return false;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1 + (rand() % 2)));

    {
        uint32_t v = press;
        if (!mem.Write(static_cast<uintptr_t>(addr), &v, sizeof(v), mem.current_process.PID)) return false;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1 + (rand() % 2)));

    {
        uint32_t v = release;
        if (!mem.Write(static_cast<uintptr_t>(addr), &v, sizeof(v), mem.current_process.PID)) return false;
    }

    lastInAttackWrite = std::chrono::steady_clock::now();
    return true;
}

bool TriggerBot::HasLineOfSight(Player* target) {
    if (!target || !localPlayer) return false;
    
    Vector3D targetPos = target->LocalOrigin;
    
    for (auto& player : *players) {
        if (!player || player == target) continue;
        if (!player->IsValid() || !player->IsHostile) continue;
        
        float distBetweenPlayers = player->LocalOrigin.Distance(targetPos);
        
        if (distBetweenPlayers < 30.0f) {
            if (debugMode) printf("[DEBUG] Players too close together, distance: %.1f\n", distBetweenPlayers);
            return false;
        }
    }
    
    return true;
}
