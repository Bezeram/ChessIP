#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>

typedef sf::Vector2i PiecePosition;
typedef std::string TextureString;
typedef std::string SoundString;
typedef std::string FontString;
typedef std::string PieceName;

enum class PieceColor
{
    White = 0,
    Black,
};

enum class Effect
{
    None = 0,
	// TODO: Add buffs and debuffs
};

// TODO: update according to the effects added
static bool IsEffectBuff(Effect effect)
{
	assert(effect != Effect::None);
    return false;
}


struct ActionMove
{
    ActionMove(PiecePosition targetSquare) : TargetSquare(targetSquare) {}

    bool operator==(const ActionMove& other) const
    {
        return TargetSquare == other.TargetSquare && Effect == other.Effect;
    }
    bool operator!=(const ActionMove& other) const
    {
        return !(*this == other);
    }

    PiecePosition TargetSquare;
    Effect Effect = Effect::None;
};


struct PieceMove
{
    PieceMove(PiecePosition startSquare, PiecePosition targetSquare) : StartSquare(startSquare), TargetSquare(targetSquare) {}
    PieceMove(const PieceMove&) = default;
    PiecePosition StartSquare;
    PiecePosition TargetSquare;
};

enum class PieceType
{
    None = 0,
    White_King,
    White_Witch,
    White_Knight,
    White_Alchemist,
    White_Archer,
    White_Builder,
    White_GrimReaper,
    White_PlagueDoctor,
    White_Rogue,
    White_Trebuchet,
    White_Warlord,
    White_Samurai,
    White_Dragon,
    Black_King,
    Black_Witch,
    Black_Knight,
    Black_Alchemist,
    Black_Archer,
    Black_Builder,
    Black_GrimReaper,
    Black_PlagueDoctor,
    Black_Rogue,
    Black_Trebuchet,
    Black_Warlord,
    Black_Samurai,
    Black_Dragon,
};

// Global vars
namespace GlobalConstants
{
    const inline static PiecePosition NullPosition = { -1, -1 };
    const inline static ActionMove NullActionMove = ActionMove(PiecePosition(-1, -1));
}

namespace Textures
{
    const inline static std::unordered_map<PieceType, PieceName> PieceTypeToString =
    {
        { PieceType::None, "None" },
	    { PieceType::White_King,        "White_King" },         { PieceType::Black_King,        "Black_King" },
	    { PieceType::White_Witch,       "White_Witch" },        { PieceType::Black_Witch,       "Black_Witch" },
	    { PieceType::White_Knight,      "White_Knight" },       { PieceType::Black_Knight,      "Black_Knight" },
	    { PieceType::White_Alchemist,   "White_Alchemist" },    { PieceType::Black_Alchemist,   "Black_Alchemist" },
	    { PieceType::White_Archer,      "White_Archer" },       { PieceType::Black_Archer,      "Black_Archer" },
	    { PieceType::White_Builder,     "White_Builder" },      { PieceType::Black_Builder,     "Black_Builder" },
	    { PieceType::White_GrimReaper,  "White_GrimReaper" },   { PieceType::Black_GrimReaper,  "Black_GrimReaper" },
	    { PieceType::White_PlagueDoctor,"White_PlagueDoctor" }, { PieceType::Black_PlagueDoctor,"Black_PlagueDoctor" },
	    { PieceType::White_Rogue,       "White_Rogue" },        { PieceType::Black_Rogue,       "Black_Rogue" },
	    { PieceType::White_Trebuchet,   "White_Trebuchet" },    { PieceType::Black_Trebuchet,   "Black_Trebuchet" },
	    { PieceType::White_Warlord,     "White_Warlord" },      { PieceType::Black_Warlord,     "Black_Warlord" },
	    { PieceType::White_Samurai,     "White_Samurai" },      { PieceType::Black_Samurai,     "Black_Samurai" },
	    { PieceType::White_Dragon,      "White_Dragon" },       { PieceType::Black_Dragon,      "Black_Dragon" }
    };
    const inline static std::string Null = "Null.png";
    const inline static std::string Board = "Board.jpg";

    const inline static std::string Gold_Bar_0 = "Gold_Bar_0.jpg";
    const inline static std::string Gold_Bar_1 = "Gold_Bar_1.jpg";
    const inline static std::string Gold_Bar_2 = "Gold_Bar_2.jpg";
    const inline static std::string Gold_Bar_3 = "Gold_Bar_3.jpg";
    const inline static std::string Gold_Bar_4 = "Gold_Bar_4.jpg";
    const inline static std::string Gold_Bar_5 = "Gold_Bar_5.jpg";
    const inline static std::string Gold_Bar_6 = "Gold_Bar_6.jpg";
    const inline static std::string Gold_Bar_7 = "Gold_Bar_7.jpg";
    const inline static std::string Gold_Bar_8 = "Gold_Bar_8.jpg";
    const inline static std::string Gold_Bar_9 = "Gold_Bar_9.jpg";

    const inline static std::string Flux_Bar_0 = "Gold_Bar_0.jpg";
    const inline static std::string Flux_Bar_1 = "Gold_Bar_1.jpg";
    const inline static std::string Flux_Bar_2 = "Gold_Bar_2.jpg";
    const inline static std::string Flux_Bar_3 = "Gold_Bar_3.jpg";
    const inline static std::string Flux_Bar_4 = "Gold_Bar_4.jpg";
    const inline static std::string Flux_Bar_5 = "Gold_Bar_5.jpg";
    const inline static std::string Flux_Bar_6 = "Gold_Bar_6.jpg";
    const inline static std::string Flux_Bar_7 = "Gold_Bar_7.jpg";
    const inline static std::string Flux_Bar_8 = "Gold_Bar_8.jpg";
    const inline static std::string Flux_Bar_9 = "Gold_Bar_9.jpg";
}

namespace Paths
{
    const inline static std::string Root = "../";
    const inline static std::string Assets = Root + "assets/";
    const inline static std::string Textures = Assets + "textures/";
    const inline static std::string Pieces = Textures + "pieces/";
    const inline static std::string Sounds = Assets + "sounds/";
    const inline static std::string Fonts = Assets + "fonts/";
	const inline static std::string Config = Root + "config/";
    const inline static std::string WindowConfig = Config + "window.txt";
}

inline bool IsWhitePiece(PieceType type)
{
    return int(type) <= int(PieceType::White_Dragon);
}

inline bool IsBlackPiece(PieceType type)
{
    return int(type) > int(PieceType::White_Dragon);
}

inline PieceColor GetPieceColor(PieceType type)
{
	if (IsWhitePiece(type))
		return PieceColor::White;
	return PieceColor::Black;
}

enum class GameType
{
    OneVOne,
    TwoVTwo,
};

template<typename T>
inline T Lerp(T start, T end, float t)
{
    return start * (1 - t) + end * t;
}

template<>
inline sf::Color Lerp(sf::Color start, sf::Color end, float t)
{
    uint8_t r = start.r * (1 - t) + end.r * t;
    uint8_t g = start.g * (1 - t) + end.g * t;
    uint8_t b = start.b * (1 - t) + end.b * t;
    return sf::Color(r, g, b);
}

template<typename T>
inline T Clamp(T value, T min, T max)
{
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}

template<typename T>
inline T ClampMax(T value, T max)
{
    if (value > max)
        return max;
    return value;
}

template<typename T>
inline T ClampMin(T value, T min)
{
    if (value < min)
        return min;
    return value;
}

inline bool IsInBounds(int x, int size)
{
    return x >= 0 && x < size;
}

inline bool IsCellInBounds(PiecePosition cellIndex, int boardSize)
{
    return IsInBounds(cellIndex.x, boardSize) && IsInBounds(cellIndex.y, boardSize);
}

namespace Global
{
    inline float AdjustableK = 0.1f;
}

struct WindowSettings
{
	WindowSettings() = default;
    sf::State State = sf::State::Fullscreen;
    sf::Vector2u Resolution = { 1920, 1080 };
};

inline WindowSettings ParseWindowConfig(const std::string& filePath)
{
    WindowSettings settings{};
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        // If file is not present, assume default options
        return settings;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.rfind("fullscreen=", 0) == 0)
        {
            std::string value = line.substr(11);
            settings.State = (value == "1") ? sf::State::Fullscreen : sf::State::Windowed;
        }
        else if (line.rfind("resolution=", 0) == 0)
        {
            std::string value = line.substr(11);
            auto commaPos = value.find('x');
            if (commaPos != std::string::npos)
            {
                unsigned int width = std::stoul(value.substr(0, commaPos));
                unsigned int height = std::stoul(value.substr(commaPos + 1));
                settings.Resolution = sf::Vector2u(width, height);
            }
        }
        // Ignore unrecognized lines or comments
    }

    return settings;
}


