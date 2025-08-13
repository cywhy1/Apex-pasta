// Config.hpp
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Camera.hpp"
#include "Glow.hpp"
#include "Trigger.hpp"
#include "DMALibrary/Memory/InputManager.h"

class Config {
public:
    static Config& GetInstance() {
        static Config instance;
        return instance;
    }

    // Method to initialize the Config instance.
    void Initialize(const std::string& filePath, Camera* gameCamera, Sense* esp) {
        if (!initialized) {
            configFilePath = filePath;
            GameCamera = gameCamera;
            ESP = esp;

            Update();
            initialized = true;
        }
    }

    // Attach late-created components (TriggerBot, Keyboard) and re-apply config
    void Attach(TriggerBot* triggerBot, c_keys* keyboard) {
        Trigger = triggerBot;
        Keyboard = keyboard;
        Update();
    }

    void Update() {
        std::ifstream configFile(configFilePath);
        std::string line;

        if (!configFile.is_open()) {
            std::cerr << "Failed to open config file: " << configFilePath << std::endl;
            return;
        }

        while (getline(configFile, line)) {
            std::istringstream lineStream(line);
            std::string key, value;
            if (getline(lineStream, key, '=') && getline(lineStream, value)) {
                updateVariable(key, value);
            }
        }
    }

    void Save() {
        std::ofstream configFile(configFilePath);

        if (!configFile.is_open()) {
            std::cerr << "Failed to open config file: " << configFilePath << std::endl;
            return;
        }

        configFile << "GlowItem=" << (ESP->ItemGlow ? "true" : "false") << std::endl;
        configFile << "GlowItemRarity=" << ESP->MinimumItemRarity << std::endl;
        configFile << "ResolutionX=" << GameCamera->ScreenSize.x << std::endl;
		configFile << "ResolutionY=" << GameCamera->ScreenSize.y << std::endl;
		configFile << "FOV=" << GameCamera->FOV << std::endl;


        // TriggerBot
        if (Trigger) {
            configFile << "TriggerEnabled=" << (Trigger->IsEnabled() ? "true" : "false") << std::endl;
            configFile << "TriggerFOV=" << Trigger->GetTriggerFOV() << std::endl;
            configFile << "TriggerMaxDistance=" << Trigger->GetMaxDistance() << std::endl;
            configFile << "TriggerKey=" << Trigger->GetTriggerKey() << std::endl;
            configFile << "TriggerYawBias=" << Trigger->GetYawBias() << std::endl;
            configFile << "TriggerDelayMs=" << Trigger->GetTriggerDelayMs() << std::endl;
        }

        // No kernel input settings
    }

private:
    std::string configFilePath;
    Camera* GameCamera = nullptr;
    Sense* ESP = nullptr;
    TriggerBot* Trigger = nullptr;
    c_keys* Keyboard = nullptr;
    bool initialized = false;

    // Private constructor
    Config() {}

    // Existing methods...

    // Delete copy constructor and assignment operator
    Config(Config const&) = delete;
    void operator=(Config const&) = delete;

    void updateVariable(const std::string& key, const std::string& value) {
		if (key == "GlowItem") {
            if (value == "true") {
                ESP->ItemGlow = true;
            }
            else if (value == "false") {
                ESP->ItemGlow = false;
            }
		}
        if (key == "GlowItemRarity") {
			ESP->MinimumItemRarity = std::stoi(value);
		}
        if (key == "ResolutionX") {
			GameCamera->ScreenSize.x = std::stoi(value);
		}
        if (key == "ResolutionY") {
            GameCamera->ScreenSize.y = std::stoi(value);
        }
        if (key == "FOV") {
			GameCamera->FOV = std::stof(value);
		}
        if (key == "TriggerEnabled" && Trigger) {
            Trigger->SetEnabled(value == "true");
        }
        if (key == "TriggerFOV" && Trigger) {
            Trigger->SetTriggerFOV(std::stof(value));
        }
        if (key == "TriggerMaxDistance" && Trigger) {
            Trigger->SetMaxDistance(std::stof(value));
        }
        if (key == "TriggerKey" && Trigger) {
            Trigger->SetTriggerKey(static_cast<uint32_t>(std::stoi(value)));
        }
        if (key == "TriggerYawBias" && Trigger) {
            Trigger->SetYawBias(std::stof(value));
        }
        if (key == "TriggerDelayMs" && Trigger) {
            Trigger->SetTriggerDelayMs(std::stoi(value));
        }
        // No kernel input settings
    }
};
