#pragma once
#include "LocalPlayer.hpp"
#include "Player.hpp"
#include "Camera.hpp"
#include "Spectator.hpp"
#include "Glow.hpp"
#include "Trigger.hpp"

void Render(LocalPlayer* Myself, std::vector<Player*>* Players, Camera* GameCamera, Spectator* Spectators, Sense* ESP, TriggerBot* Trigger);