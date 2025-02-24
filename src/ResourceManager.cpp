#include "ResourceManager.h"

void ChessAI::ResourceManager::Initialise()
{
	assert(s_ResourceManager == nullptr);

	s_ResourceManager = new ResourceManager();
}

const ChessAI::ResourceManager& ChessAI::ResourceManager::GetInstance()
{
	assert(s_ResourceManager != nullptr);

	return *s_ResourceManager;
}

void ChessAI::ResourceManager::Shutdown()
{
	assert(s_ResourceManager != nullptr);

	delete s_ResourceManager;
	s_ResourceManager = nullptr;
}

void ChessAI::ResourceManager::LoadAssets()
{
	// Load textures
	for (int i = 0; i < 12; i++)
	{
		PieceType type = PieceType(i);
		std::string name = m_PieceIDToResource[type];
		std::string path = "assets/textures/" + name + ".png";
		AddTexture(path.c_str(), name.c_str());
	}

	// Load font
	{
		std::string name = "JetBrainsMono-Bold";
		std::string path = "assets/fonts/" + name + ".ttf";
		AddFont(path.c_str(), name.c_str());
	}
}

const sf::Texture& ChessAI::ResourceManager::GetPieceTexture(PieceType type) const
{
	const std::string& key = m_PieceIDToResource.at(type);
	return m_Textures.at(key);
}

const sf::Texture& ChessAI::ResourceManager::AddTexture(const char* filePath, const char* name)
{
	auto it = m_Textures.find(name);
	if (it != m_Textures.end())
	{
		std::cout << "Texture " << name << " has already been loaded.\n";
	}
		
	sf::Texture texture;
	if (!texture.loadFromFile(filePath)) 
	{
		throw std::runtime_error("Failed to load texture at " + std::string(filePath));
	}

	m_Textures[name] = std::move(texture);
	return m_Textures[name];
}

const sf::Font& ChessAI::ResourceManager::AddFont(const char* filePath, const char* name)
{
	auto it = m_Fonts.find(name);
	if (it != m_Fonts.end())
	{
		std::cout << "Font " << name << " has already been loaded.\n";
	}

	sf::Font font;
	if (!font.openFromFile(filePath))
	{
		throw std::runtime_error("Failed to load font at " + std::string(filePath));
	}

	m_Fonts[name] = std::move(font);
	return m_Fonts[name];
}

const sf::SoundBuffer& ChessAI::ResourceManager::AddSound(const char* filePath, const char* name)
{
	auto it = m_Sounds.find(name);
	if (it != m_Sounds.end())
	{
		std::cout << "Sound " << name << " has already been loaded.\n";
	}

	sf::SoundBuffer sound;
	if (!sound.loadFromFile(filePath))
	{
		throw std::runtime_error("Failed to load sound at " + std::string(filePath));
	}

	m_Sounds[name] = std::move(sound);
	return m_Sounds[name];
}

const sf::Texture& ChessAI::ResourceManager::GetTexture(const char* key) const
{
	auto it = m_Textures.find(key);
	if (it == m_Textures.end()) 
	{
		throw std::runtime_error("Font not found!");
	}
	return it->second;
}

const sf::Font& ChessAI::ResourceManager::GetFont(const char* key) const
{
	auto it = m_Fonts.find(key);
	if (it == m_Fonts.end()) 
	{
		throw std::runtime_error("Font not found!");
	}
	return it->second;
}

const sf::SoundBuffer& ChessAI::ResourceManager::GetSound(const char* key) const
{
	auto it = m_Sounds.find(key);
	if (it == m_Sounds.end()) 
	{
		throw std::runtime_error("Sound not found!");
	}
	return it->second;
}

ChessAI::ResourceManager::ResourceManager()
{
	SetPieceIDToResource();

	LoadAssets();
}

void ChessAI::ResourceManager::SetPieceIDToResource()
{
	std::string pieceColor[] = { "white", "black" };
	std::string pieceType[] = { "pawn", "knight", "bishop", "rook", "queen", "king" };
	int i = 0;
	for (auto& color : pieceColor)
	{
		for (auto& type : pieceType)
		{
			std::string name = color + "-" + type;
			m_PieceIDToResource[PieceType(i)] = name;
			
			i++;
		}
	}
}
