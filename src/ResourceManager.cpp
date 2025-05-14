#include "ResourceManager.h"

using namespace ChessIP;

void ResourceManager::Initialise()
{
	assert(s_ResourceManager == nullptr);

	s_ResourceManager = new ResourceManager();
}

const ResourceManager& ResourceManager::GetInstance()
{
	assert(s_ResourceManager != nullptr);

	return *s_ResourceManager;
}

void ResourceManager::Shutdown()
{
	assert(s_ResourceManager != nullptr);

	delete s_ResourceManager;
	s_ResourceManager = nullptr;
}

void ResourceManager::LoadAssets()
{
	// Load pieces
	for (int i = 0; i < 12; i++)
	{
		PieceType type = PieceType(i);
		std::string name = m_PieceIDToResource[type];
		std::string path = "./assets/textures/1024x1024/" + name + ".png";
		AddTexture(path.c_str(), name.c_str());
	}

	// Board
	std::string path = "./assets/textures/board.jpg";
	AddTexture(path.c_str(), "board");

	// Resources bars
	{
		std::string path = "./assets/textures/resources-bars.jpg";
		sf::Image resourceBarImg(path.c_str());
		// Mask background from the resources bar
		resourceBarImg.createMaskFromColor(sf::Color::Black, 0);
		sf::Texture texture;
		if (!texture.loadFromImage(resourceBarImg))
		{
			std::cout << "Could not load resources bar texture.\n";
		}
		else
		{
			m_Textures["resources_bars"] = std::move(texture);
			// rect size (545, 144)
		}
	}

	// Load font
	{
		std::string name = "JetBrainsMono-Bold";
		std::string path = "./assets/fonts/" + name + ".ttf";
		AddFont(path.c_str(), name.c_str());
	}
	
	// Load sounds
	{
		// load all sounds from path

		std::string path = "./assets/sounds/";

		for (const auto & entry : std::filesystem::directory_iterator(path))
		{
			std::string name = entry.path().filename().string();
			std::string ext = entry.path().extension().string();
			if (ext != ".ogg" && ext != ".wav" && ext != ".mp3")
				continue;
			name = name.substr(0, name.size() - ext.size());
            std::string soundPath = entry.path().string();
			AddSound(soundPath.c_str(), name.c_str());
		}
		
	}
}

const sf::Texture& ResourceManager::GetPieceTexture(PieceType type) const
{
	const std::string& key = m_PieceIDToResource.at(type);
	return m_Textures.at(key);
}

const sf::Texture& ResourceManager::AddTexture(const char* filePath, const char* name)
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

	texture.setSmooth(true);
	texture.setRepeated(false);
	if (!texture.generateMipmap())
		std::cout << "Mipmaps unavailable for texture at " << filePath << std::endl;
	m_Textures[name] = std::move(texture);
	return m_Textures[name];
}

const sf::Font& ResourceManager::AddFont(const char* filePath, const char* name)
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

const sf::SoundBuffer& ResourceManager::AddSound(const char* filePath, const char* name)
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

const sf::Texture& ResourceManager::GetTexture(const char* key) const
{
	auto it = m_Textures.find(key);
	if (it == m_Textures.end()) 
	{
		throw std::runtime_error("Font not found!");
	}
	return it->second;
}

const sf::Font& ResourceManager::GetFont(const char* key) const
{
	auto it = m_Fonts.find(key);
	if (it == m_Fonts.end()) 
	{
		throw std::runtime_error("Font not found!");
	}
	return it->second;
}

const sf::SoundBuffer& ResourceManager::GetSound(const char* key) const
{
	auto it = m_Sounds.find(key);
	if (it == m_Sounds.end()) 
	{
		return m_Sounds.at("move1");
	}
	return it->second;
}

ResourceManager::ResourceManager()
{
	SetPieceIDToResource();

	LoadAssets();
}

void ResourceManager::SetPieceIDToResource()
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
