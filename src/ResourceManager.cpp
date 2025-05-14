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
	for (auto& type : Textures::PieceTypeToString)
	{
		const std::string& name = type.second;
		const std::string& path = Paths::Pieces + name + ".jpg";

		AddTexture(path, name, true);
	}

	// Board
	AddTexture(Paths::Textures + Textures::Board, Textures::Board);

	// Resources bars
	for (int i = 0; i <= 9; i++)
	{
		AddTexture(Paths::Textures + Textures::Gold_Bar_0, Textures::Gold_Bar_0 + std::to_string(i), true);
		AddTexture(Paths::Textures + Textures::Flux_Bar_0, Textures::Flux_Bar_0 + std::to_string(i), true);
	}

	// Load font
	{
		std::string name = "JetBrainsMono_Bold";
		std::string path = "../../../../src/assets/fonts/" + name + ".ttf";
		AddFont(path.c_str(), name.c_str());
	}
}

const sf::Texture& ResourceManager::GetPieceTexture(PieceType type) const
{
	const std::string& key = Textures::PieceTypeToString.at(type);
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
		throw std::runtime_error("Sound not found!");
	}
	return it->second;
}

ResourceManager::ResourceManager()
{
	AddTexture(Paths::Textures, Textures::Null);
	assert(m_Textures.find(Textures::Null) != m_Textures.end());

	LoadAssets();
}

