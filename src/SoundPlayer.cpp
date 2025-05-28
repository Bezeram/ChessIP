#include "SoundPlayer.h"

#include "ResourceManager.h"

#include <algorithm>

void SoundPlayer::Initialize()
{
    assert(s_SoundPlayer == nullptr);

    s_SoundPlayer = new SoundPlayer();
}

SoundPlayer& SoundPlayer::GetInstance()
{
    assert(s_SoundPlayer != nullptr);

    return *s_SoundPlayer;
}

SoundPlayer::SoundPlayer()
{
    // Initialize any default settings here
}

SoundPlayer::~SoundPlayer()
{
    delete s_SoundPlayer;
}

void SoundPlayer::PlaySound(const std::string& name)
{
    if (m_muted) return;

    try {
        const auto& buffer = ResourceManager::GetInstance().GetSoundBuffer(name.c_str());

        // Create a NEW sound object (not unique_ptr) and store it directly
        m_activeSounds.emplace_back(new sf::Sound(buffer));
        auto& sound = *m_activeSounds.back();

        sound.setVolume(m_masterVolume);
        sound.play();
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to play sound '" << name << "': " << e.what() << std::endl;
    }
}

void SoundPlayer::Update()
{
    // Remove only sounds that have finished playing
    m_activeSounds.erase(
        std::remove_if(m_activeSounds.begin(), m_activeSounds.end(),
            [](const std::unique_ptr<sf::Sound>& sound) {
                return sound->getStatus() == sf::SoundSource::Status::Stopped;
            }),
        m_activeSounds.end());
}

void SoundPlayer::SetMasterVolume(float volume)
{
    m_masterVolume = std::clamp(volume, 0.0f, 100.0f);
    for (auto& sound : m_activeSounds) {
        sound->setVolume(m_masterVolume);
    }
}

float SoundPlayer::GetMasterVolume() const
{
    return m_masterVolume;
}

void SoundPlayer::ToggleMute()
{
    m_muted = !m_muted;
    for (auto& sound : m_activeSounds) {
        sound->setVolume(m_muted ? 0.0f : m_masterVolume);
    }
}

bool SoundPlayer::IsMuted() const
{
    return m_muted;
}
