#pragma once

#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include <string>

class SoundPlayer
{
public:
    static void Initialize();
    static SoundPlayer& GetInstance();
    // Delete copy constructor and assignment operator
    SoundPlayer(const SoundPlayer&) = delete;
    SoundPlayer& operator=(const SoundPlayer&) = delete;

    // Sound control
    void PlaySound(const std::string& name);
    void SetMasterVolume(float volume);
    float GetMasterVolume() const;
    void ToggleMute();
    bool IsMuted() const;

    // Clean up finished sounds
    void Update();

private:
    SoundPlayer(); // Private constructor for singleton
    ~SoundPlayer() = default;

    static inline SoundPlayer* s_SoundPlayer = nullptr;

    std::vector<std::unique_ptr<sf::Sound>> m_activeSounds;
    float m_masterVolume = 100.0f;
    bool m_muted = false;
};
