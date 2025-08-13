#pragma once
#include <thread>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "Player.hpp"
#include "LocalPlayer.hpp"
#include "Offsets.hpp"
#include "Camera.hpp"
#include "GlowMode.hpp"
#include "DMALibrary/Memory/Memory.h"
#include <array>

struct Sense {
    // Variables
    Camera* GameCamera;
    LocalPlayer* Myself;
    std::vector<Player*>* Players;
    int TotalSpectators = 0;
    std::vector<std::string> Spectators;
    uint64_t HighlightSettingsPointer;

    bool ItemGlow = false;
    int MinimumItemRarity = 54;

    // Define rarity constants
    const int goldItems = 15;
    const int redItems = 42;
    const int purpleItems = 47;
    const int blueItems = 54;
    const int ammoBoxes = 58;
    const int whiteItems = 65;

    // Dynamic array for items to glow based on MinimumItemRarity
    std::vector<int> itemsToGlow;

    // Colors
    float InvisibleGlowColor[3] = { 0, 1, 0 };
    float VisibleGlowColor[3] = { 1, 0, 0 };
    std::array<float, 3> highlightGlowColorRGB = { 0, 1, 0 };

    Sense(std::vector<Player*>* Players, Camera* GameCamera, LocalPlayer* Myself) {
        srand(static_cast<unsigned int>(time(nullptr))); // Seed RNG once per instance
        this->Players = Players;
        this->GameCamera = GameCamera;
        this->Myself = Myself;
        updateItemsToGlowArray(); // Initialize the items array
    }

    void Initialize() {
        // idk, nothing for now
    }

    void updateItemsToGlowArray() {
        itemsToGlow.clear();

        switch (MinimumItemRarity) {
        case 54:
            itemsToGlow.push_back(54);
            [[fallthrough]];
        case 47:
            itemsToGlow.push_back(47);
            [[fallthrough]];
        case 15:
            itemsToGlow.push_back(15);
            [[fallthrough]];
        case 42:
            itemsToGlow.push_back(42);
            break;
        default:
            itemsToGlow.push_back(54);
            itemsToGlow.push_back(47);
            itemsToGlow.push_back(15);
            itemsToGlow.push_back(42);
            break;
        }
    }

    void setCustomGlow(Player* Target, int enable, int wall, bool isVisible) {
        if (Target->GlowEnable == 0 && Target->GlowThroughWall == 0 && Target->HighlightID == 0) {
            return;
        }
        uint64_t basePointer = Target->BasePointer;
        if (!mem.IsValidPointer(basePointer)) {
            LOG("Invalid BasePointer 0x%llX for player, skipping glow\n", basePointer);
            return;
		}

        std::array<unsigned char, 4> highlightFunctionBits = {
            2,   // InsideFunction
            125, // OutlineFunction: HIGHLIGHT_OUTLINE_OBJECTIVE
            50,  // OutlineRadius: size * 255 / 8
            64   // (EntityVisible << 6) | State & 0x3F | (AfterPostProcess << 7)
        };

        int settingIndex = isVisible ? 66 : 65; // Simple: visible=66, invisible=65
        if (isVisible) {
            highlightGlowColorRGB = { 0.0f, 1.0f, 0.0f }; // Green
        }
        else {
            highlightGlowColorRGB = { 1.0f, 0.0f, 0.0f }; // Red
        }


        if (Target->GlowEnable != enable) {
            uint64_t glowEnableAddress = basePointer + OFF_GLOW_ENABLE;
            uint64_t PhysAddr;
            if (mem.VirtToPhys(glowEnableAddress, PhysAddr)) {
                // Random delay 10-60 microseconds
                std::this_thread::sleep_for(std::chrono::microseconds(10 + (rand() % 50)));
                mem.Write(PhysAddr, enable);
            }
        }

        if (Target->GlowThroughWall != wall) {
            uint64_t glowThroughWallAddress = basePointer + OFF_GLOW_THROUGH_WALL;
            uint64_t PhysAddr;
            if (mem.VirtToPhys(glowThroughWallAddress, PhysAddr)) {
                std::this_thread::sleep_for(std::chrono::microseconds(10 + (rand() % 50)));
                mem.Write(PhysAddr, wall);
            }
        }

        uint64_t highlightIdAddress = basePointer + OFF_GLOW_HIGHLIGHT_ID;
        unsigned char value = settingIndex;
        uint64_t PhysAddr;
        if (mem.VirtToPhys(highlightIdAddress, PhysAddr)) {
            std::this_thread::sleep_for(std::chrono::microseconds(10 + (rand() % 50)));
            mem.Write(PhysAddr, value);
        }

        uint64_t highlightSettingsPtr = HighlightSettingsPointer;
        if (mem.IsValidPointer(highlightSettingsPtr)) {
            auto handle = mem.CreateScatterHandle(-1);
            uint64_t Pa1;
            uint64_t Pa2;

            if (mem.VirtToPhys(highlightSettingsPtr + OFF_GLOW_HIGHLIGHT_TYPE_SIZE * settingIndex + 0x0, Pa1)) {
                mem.AddScatterWriteRequest(handle, Pa1, &highlightFunctionBits, sizeof(highlightFunctionBits));
            }

            if (mem.VirtToPhys(highlightSettingsPtr + OFF_GLOW_HIGHLIGHT_TYPE_SIZE * settingIndex + 0x4, Pa2)) {
                mem.AddScatterWriteRequest(handle, Pa2, &highlightGlowColorRGB, sizeof(highlightGlowColorRGB));
            }

            mem.ExecuteWriteScatter(handle);
            mem.CloseScatterHandle(handle);
        }

        uint64_t glowFixAddress = basePointer + OFF_GLOW_FIX;
        if (mem.VirtToPhys(glowFixAddress, PhysAddr)) {
            std::this_thread::sleep_for(std::chrono::microseconds(10 + (rand() % 50)));
            mem.Write(PhysAddr, 0);
        }
    }

    Vector2D DummyVector = { 0, 0 };
    void Update() {
        //randome batch operation
		int maxBatchSize = 3 + (rand() % 4);
		int currentBatchCount = 0;

        auto physhandle = mem.CreateScatterHandle(-1); //_PID of -1 is physical (for virtophys use)
        //Yes i know its fucking scuffed ok?

        uint64_t highlightSettingsPtr = HighlightSettingsPointer;
        if (mem.IsValidPointer(highlightSettingsPtr)) {
            uint64_t highlightSize = OFF_GLOW_HIGHLIGHT_TYPE_SIZE;

            const int allPossibleItems[] = { redItems, goldItems, purpleItems, blueItems };
            const int allPossibleItemsCount = 4;

            if (ItemGlow) {
                GlowMode activeGlowMode = { 137, 138, 16, 127 };

                for (int item : itemsToGlow) {
                    const uintptr_t address = highlightSettingsPtr + (highlightSize * item);
                    const GlowMode oldGlowMode = mem.Read<GlowMode>(address);
                    if (activeGlowMode != oldGlowMode) {
                        uint64_t PhysAddr;
                        if (mem.VirtToPhys(address, PhysAddr)) {
                            if (rand() % 4 == 0) {
                                std::this_thread::sleep_for(std::chrono::microseconds(10 + (rand() % 50)));
                                mem.Write(PhysAddr, activeGlowMode);
                            }else{
                               //Scatter write request
                                mem.AddScatterWriteRequest(physhandle, PhysAddr, &activeGlowMode, sizeof(activeGlowMode));
								currentBatchCount++;
                                if (currentBatchCount >= maxBatchSize) {
                                    mem.ExecuteWriteScatter(physhandle);
									mem.CloseScatterHandle(physhandle);
									physhandle = mem.CreateScatterHandle(-1); // Create a new handle for the next batch
									currentBatchCount = 0; // Reset 
									maxBatchSize = 3 + (rand() % 4); // Randomize the next batch size
                                }
                            }
                            
                        }
                    }
                }

                GlowMode blankGlowMode = { 0, 0, 0, 0 };

                for (int i = 0; i < allPossibleItemsCount; i++) {
                    int item = allPossibleItems[i];

                    bool shouldGlow = false;
                    for (int glowItem : itemsToGlow) {
                        if (item == glowItem) {
                            shouldGlow = true;
                            break;
                        }
                    }

                    if (!shouldGlow) {
                        const uintptr_t address = highlightSettingsPtr + (highlightSize * item);
                        const GlowMode oldGlowMode = mem.Read<GlowMode>(address);
                        if (blankGlowMode != oldGlowMode) {
                            uint64_t PhysAddr;
                            if (mem.VirtToPhys(address, PhysAddr)) {
                                if (rand() % 4 == 0) {
                                    std::this_thread::sleep_for(std::chrono::microseconds(10 + (rand() % 50)));
                                    mem.Write(PhysAddr, blankGlowMode);
                                }
                                else {
                                    mem.AddScatterWriteRequest(physhandle, PhysAddr, &blankGlowMode, sizeof(blankGlowMode));
                                    currentBatchCount++;

                                    if (currentBatchCount >= maxBatchSize) {
                                        mem.ExecuteWriteScatter(physhandle);
                                        mem.CloseScatterHandle(physhandle);
                                        physhandle = mem.CreateScatterHandle(-1);
                                        currentBatchCount = 0;
                                        maxBatchSize = 3 + (rand() % 4);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else {
                GlowMode BlankGlow = { 0, 0, 0, 0 };

                for (int i = 0; i < allPossibleItemsCount; i++) {
                    int item = allPossibleItems[i];
                    const uintptr_t address = highlightSettingsPtr + (highlightSize * item);
                    const GlowMode oldGlowMode = mem.Read<GlowMode>(address);
                    if (BlankGlow != oldGlowMode) {
                        uint64_t PhysAddr;
                        if (mem.VirtToPhys(address, PhysAddr)) {
                            if (rand() % 4 == 0) {
                                std::this_thread::sleep_for(std::chrono::microseconds(10 + (rand() % 50)));
                                mem.Write(PhysAddr, BlankGlow);
                            }
                            else {
                                mem.AddScatterWriteRequest(physhandle, PhysAddr, &BlankGlow, sizeof(BlankGlow));
                                currentBatchCount++;

                                if (currentBatchCount >= maxBatchSize) {
                                    mem.ExecuteWriteScatter(physhandle);
                                    mem.CloseScatterHandle(physhandle);
                                    physhandle = mem.CreateScatterHandle(-1); // Create a new handle for the next batch
                                    currentBatchCount = 0; // Reset 
                                    maxBatchSize = 3 + (rand() % 4); // Randomize the next batch size
                                }
                            }
                        }
                    }
                }
            }
        }

        // Execute any remaining operations in the final batch
        if (currentBatchCount > 0) {
            mem.ExecuteWriteScatter(physhandle);
        }
        mem.CloseScatterHandle(physhandle);

        for (int i = 0; i < Players->size(); i++) {
            Player* Target = Players->at(i);
            if (!Target->IsValid()) continue;
            if (Target->IsDummy()) continue;
            if (Target->IsLocal) continue;
            if (!Target->IsHostile) continue;

            if (GameCamera->WorldToScreen(Target->LocalOrigin.ModifyZ(30), DummyVector)) {
                setCustomGlow(Target, 1, 1, Target->IsVisible);
            }
        }
    }
};