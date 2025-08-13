#pragma once
#include "../pch.h"
#include "Registry.h"

class c_keys
{
private:
	uint64_t gafAsyncKeyStateExport = 0;
	uint8_t state_bitmap[64] { };
	uint8_t previous_state_bitmap[256 / 8] { };
	uint64_t win32kbase = 0;

	int win_logon_pid = 0;

	c_registry registry;
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
	std::chrono::steady_clock::time_point lastTrigger;
    std::chrono::steady_clock::time_point lastInAttackWrite;
    int minInAttackIntervalMs = 90; // lowkey rate limit
public:
	c_keys() = default;

	~c_keys() = default;

	bool InitKeyboard();

	void UpdateKeys();
	bool IsKeyDown(uint32_t virtual_key_code);
    bool SafeTriggerShot();
    bool IsReactionTimeValid();

    // No kernel-injection configuration retained; only in_attack
};
