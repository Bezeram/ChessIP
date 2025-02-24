#pragma once

#include <unordered_map>
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Utils.h"

namespace ChessAI
{
	class ResourceManager
	{
	public:
		static void Initialise();
		static const ResourceManager& GetInstance();
		static void Shutdown();

		const sf::Texture& GetPieceTexture(PieceType type) const;

		const sf::Texture& AddTexture(const char* filePath, const char* name);
		const sf::Font& AddFont(const char* filePath, const char* name);
		const sf::SoundBuffer& AddSound(const char* filePath, const char* name);

		const sf::Texture& GetTexture(const char* key) const;
		const sf::Font& GetFont(const char* key) const ;
		const sf::SoundBuffer& GetSound(const char* key) const;
	private:
		void LoadAssets();
		ResourceManager();
		void SetPieceIDToResource();

	private:
		std::unordered_map<std::string, sf::Texture> m_Textures;
		std::unordered_map<std::string, sf::Font> m_Fonts;
		std::unordered_map<std::string, sf::SoundBuffer> m_Sounds;

		std::unordered_map<PieceType, std::string> m_PieceIDToResource;

		static inline ResourceManager* s_ResourceManager = nullptr;
	};
}
