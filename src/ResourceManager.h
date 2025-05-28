#pragma once

#include <unordered_map>
#include <iostream>
#include <string>


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Utils.h"

class ResourceManager
{
public:
	static void Initialise();
	static const ResourceManager& GetInstance();
	static void Shutdown();

	const sf::Texture& GetPieceTexture(PieceType type) const;

	const sf::Texture& AddTexture(const std::string& filePath, const std::string& name, bool applyMask = false);
	const sf::Font& AddFont(const std::string& filePath, const std::string& name);
	const sf::SoundBuffer& AddSound(const std::string& filePath, const std::string& name);

	const sf::Texture& GetTexture(const std::string& key) const;
	const sf::Font& GetFont(const std::string& key) const ;
	const sf::SoundBuffer& GetSoundBuffer(const std::string& key) const;
private:
	void LoadAssets();
	ResourceManager();

private:
	std::unordered_map<TextureString, sf::Texture> m_Textures;
	std::unordered_map<FontString, sf::Font> m_Fonts;
	std::unordered_map<SoundString, sf::SoundBuffer> m_Sounds;

	static inline ResourceManager* s_ResourceManager = nullptr;
};
