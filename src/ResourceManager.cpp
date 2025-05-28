#include "ResourceManager.h"

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
	for (const auto& [type, name] : Textures::PieceTypeToString)
	{
		const std::string& path = Paths::Pieces + name + ".png";
		AddTexture(path, name, true);
	}

	// Board
	AddTexture(Paths::Textures + "tile_white.png", "tile_white");
	AddTexture(Paths::Textures + "tile_black.png", "tile_black");
	AddTexture(Paths::Textures + "board_border.png", "board_border");

	// Background
	AddTexture(Paths::Textures + "background.png", "background");

	// Play
	AddTexture(Paths::Textures + "play.png", "play", true);

	// Title
	AddTexture(Paths::Textures + "title.png", "title", true);

	// Info
	AddTexture(Paths::Textures + "info.png", "info", true);

	// Inventory
	AddTexture(Paths::Textures + "inventory.png", "inventory", true);

	// Resources bars
	for (int i = 0; i <= 9; i++)
	{
		AddTexture(Paths::Gold + Textures::Gold_Bar_ + std::to_string(i) + ".png", Textures::Gold_Bar_ + std::to_string(i), true);
		AddTexture(Paths::Flux + Textures::Flux_Bar_ + std::to_string(i) + ".png", Textures::Flux_Bar_ + std::to_string(i), true);
	}

	// Load font
	{
		std::string name = "JetBrainsMono_Bold";
		std::string path = Paths::Fonts + name + ".ttf";

		AddFont(path.c_str(), name.c_str());
	}
	
	// Load sounds
	{
		// load all sounds from path

		std::string path = Paths::Sounds;

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
	const std::string& key = Textures::PieceTypeToString.at(type);
	if (m_Textures.find(key) == m_Textures.end())
	{
		std::cout << "Texture " << key << " not found!\n";
		return m_Textures.at(Textures::Null);
	}
	return m_Textures.at(key);
}

const sf::Texture& ResourceManager::AddTexture(const std::string& filePath, const std::string& name, bool applyMask)
{
	if (!applyMask)
	{
		auto it = m_Textures.find(name);
		if (it != m_Textures.end())
		{
			std::cout << "Texture " << name << " has already been loaded.\n";
		}
		
		sf::Texture texture;
		if (!texture.loadFromFile(filePath))
		{
			return m_Textures[Textures::Null];
		}

		texture.setSmooth(true);
		texture.setRepeated(false);
		if (!texture.generateMipmap())
			std::cout << "Mipmaps unavailable for texture at " << filePath << std::endl;
		m_Textures[name] = std::move(texture);
		return m_Textures[name];
	}

	// Load to image, apply mask and then load to texture
	auto it = m_Textures.find(name);
	if (it != m_Textures.end())
	{
		std::cout << "Texture " << name << " has already been loaded.\n";
	}

	sf::Image image;
	if (!image.loadFromFile(filePath.c_str()))
	{
		return m_Textures[Textures::Null];
	}

	sf::Color maskPixelColor = image.getPixel({ 0, 0 });
	image.createMaskFromColor(maskPixelColor, 0);

	sf::Texture texture;
	if (!texture.loadFromImage(image))
	{
		return m_Textures[Textures::Null];
	}
	texture.setSmooth(true);
	texture.setRepeated(false);
	if (!texture.generateMipmap())
		std::cout << "Mipmaps unavailable for texture at " << filePath << std::endl;
	m_Textures[name] = std::move(texture);
	return m_Textures[name];
}

const sf::Font& ResourceManager::AddFont(const std::string& filePath, const std::string& name)
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

const sf::SoundBuffer& ResourceManager::AddSound(const std::string& filePath, const std::string& name)
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

const sf::Texture& ResourceManager::GetTexture(const std::string& key) const
{
	auto it = m_Textures.find(key);
	if (it == m_Textures.end()) 
	{
		return m_Textures.at(Textures::Null);
	}
	return it->second;
}

const sf::Font& ResourceManager::GetFont(const std::string& key) const
{
	auto it = m_Fonts.find(key);
	if (it == m_Fonts.end()) 
	{
		throw std::runtime_error("Font not found!");
	}
	return it->second;
}

const sf::SoundBuffer& ResourceManager::GetSound(const std::string& key) const
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
	AddTexture(Paths::Textures, Textures::Null);
	assert(m_Textures.find(Textures::Null) != m_Textures.end());

	LoadAssets();
}

