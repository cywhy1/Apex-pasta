#pragma once
#include "pch.h"
#include "Player.hpp"
#include "LocalPlayer.hpp"
#include "Camera.hpp"
#include "DMALibrary/Memory/InputManager.h"
#include "DMALibrary/Memory/Memory.h"
#include "Offsets.hpp"
#include <thread>
#include <chrono>
#include <cstdlib>

class TriggerBot {
private:
    LocalPlayer* localPlayer;
    std::vector<Player*>* players;
    Camera* gameCamera;
    c_keys* inputManager;
    Level* Map;
    
    float triggerFOV = 2.0f;
    float maxTriggerDistance = 300.0f;
    float yawBiasDegrees = 0.0f;
    int triggerDelayMs = 30;
    bool enableTriggerBot = true;
    uint32_t triggerKey = VK_SHIFT;
    bool debugMode = false;
    
    std::chrono::steady_clock::time_point lastShotTime;
    std::chrono::steady_clock::time_point lastInAttackWrite;
    int minInAttackIntervalMs = 30;
    
public:
    TriggerBot(LocalPlayer* lp, std::vector<Player*>* p, Camera* cam, c_keys* input, Level* map);
    ~TriggerBot() = default;
    
    void Update();
    void SetTriggerKey(uint32_t key) { triggerKey = key; }
    void SetTriggerFOV(float fov) { triggerFOV = fov; }
    void SetMaxDistance(float distance) { maxTriggerDistance = distance; }
    void SetYawBias(float deg) { yawBiasDegrees = deg; }
    void SetTriggerDelayMs(int ms) { triggerDelayMs = ms; }
    void SetEnabled(bool enabled) { enableTriggerBot = enabled; }
    void SetDebugMode(bool debug) { debugMode = debug; }
    bool IsEnabled() const { return enableTriggerBot; }
    uint32_t GetTriggerKey() const { return triggerKey; }
    float GetTriggerFOV() const { return triggerFOV; }
    float GetMaxDistance() const { return maxTriggerDistance; }
    float GetYawBias() const { return yawBiasDegrees; }
    int GetTriggerDelayMs() const { return triggerDelayMs; }
    bool GetDebugMode() const { return debugMode; }
    
private:
    Player* FindTargetAtCrosshair();
    bool IsValidTarget(Player* player);
    float CalculateAngleFromCrosshair(Player* player);
    bool ShouldTrigger(Player* target);
    void ExecuteTrigger();
    bool SendInAttackClick();
    bool HasLineOfSight(Player* target);
    bool IsHumanLikeTiming();
    void UpdateAntiDetection();
    bool PassesAntiDetectionChecks();
};