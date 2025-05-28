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

// Follow this rule:
// All positive effects (buffs) are even numbers (2, 4, 6, ...)
// All negative effects (debuffs) are odd numbers (1, 3, 5, ...)
enum class Effect
{
    None = 0,
    Stun = 1,
    Alchemist_Shield = 2,
    Hex = 3,
    Curse = 5
};


static bool IsEffectBuff(Effect effect)
{
	return int(effect) % 2 == 0;
}

enum class MoveType 
{
    None = 0,
    Move,
    Action,
    Any
};

struct ActionMove
{
    ActionMove(PiecePosition targetSquare, MoveType moveType = MoveType::Move) 
        : TargetSquare(targetSquare), MoveType(moveType) {}

    bool operator==(const ActionMove& other) const
    {
        return TargetSquare == other.TargetSquare 
            //&& Effect == other.Effect
            ;
    }
    bool operator!=(const ActionMove& other) const
    {
        return !(*this == other);
    }

    PiecePosition TargetSquare;
    Effect Effect = Effect::None;
    MoveType MoveType;
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
	White_Wall,
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
	Black_Wall,
    Black_GrimReaper,
    Black_PlagueDoctor,
    Black_Rogue,
    Black_Trebuchet,
    Black_Warlord,
    Black_Samurai,
    Black_Dragon,
};

// Global vars
namespace Constants
{
    const inline static PiecePosition NullPosition = { -1, -1 };
    const inline static ActionMove NullActionMove = ActionMove(PiecePosition(-1, -1));
    const inline static std::unordered_map<PieceType, int> PieceTypeToFluxCost =
    {
        { PieceType::None, 0 },
        { PieceType::White_King,        999 },  { PieceType::Black_King,        999 },
        { PieceType::White_Witch,       4 },    { PieceType::Black_Witch,       4 },
        { PieceType::White_Knight,      3 },    { PieceType::Black_Knight,      3 },
        { PieceType::White_Alchemist,   4 },    { PieceType::Black_Alchemist,   4 },
        { PieceType::White_Archer,      3 },    { PieceType::Black_Archer,      3 },
        { PieceType::White_Builder,     2 },    { PieceType::Black_Builder,     2 },
        { PieceType::White_GrimReaper,  3 },    { PieceType::Black_GrimReaper,  3 },
        { PieceType::White_PlagueDoctor,3 },    { PieceType::Black_PlagueDoctor,3 },
        { PieceType::White_Rogue,       3 },    { PieceType::Black_Rogue,       3 },
        { PieceType::White_Trebuchet,   3 },    { PieceType::Black_Trebuchet,   3 },
        { PieceType::White_Warlord,     5 },    { PieceType::Black_Warlord,     5 },
        { PieceType::White_Samurai,     4 },    { PieceType::Black_Samurai,     4 },
        { PieceType::White_Dragon,      6 },    { PieceType::Black_Dragon,      6 }
    };
    const inline static std::unordered_map<PieceType, int> PieceTypeToGoldCost =
    {
        { PieceType::None, 0 },
        { PieceType::White_King,        999 },  { PieceType::Black_King,        999 },
        { PieceType::White_Witch,       4 },    { PieceType::Black_Witch,       4 },
        { PieceType::White_Knight,      3 },    { PieceType::Black_Knight,      3 },
        { PieceType::White_Alchemist,   4 },    { PieceType::Black_Alchemist,   4 },
        { PieceType::White_Archer,      3 },    { PieceType::Black_Archer,      3 },
        { PieceType::White_Builder,     2 },    { PieceType::Black_Builder,     2 },
        { PieceType::White_Wall,        0 },    { PieceType::Black_Wall,        0 },
        { PieceType::White_GrimReaper,  3 },    { PieceType::Black_GrimReaper,  3 },
        { PieceType::White_PlagueDoctor,3 },    { PieceType::Black_PlagueDoctor,3 },
        { PieceType::White_Rogue,       3 },    { PieceType::Black_Rogue,       3 },
        { PieceType::White_Trebuchet,   3 },    { PieceType::Black_Trebuchet,   3 },
        { PieceType::White_Warlord,     5 },    { PieceType::Black_Warlord,     5 },
        { PieceType::White_Samurai,     4 },    { PieceType::Black_Samurai,     4 },
        { PieceType::White_Dragon,      6 },    { PieceType::Black_Dragon,      6 }
    };
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
        { PieceType::White_Wall,        "White_Wall" },         { PieceType::Black_Wall,        "Black_Wall" },
	    { PieceType::White_GrimReaper,  "White_GrimReaper" },   { PieceType::Black_GrimReaper,  "Black_GrimReaper" },
	    { PieceType::White_PlagueDoctor,"White_PlagueDoctor" }, { PieceType::Black_PlagueDoctor,"Black_PlagueDoctor" },
	    { PieceType::White_Rogue,       "White_Rogue" },        { PieceType::Black_Rogue,       "Black_Rogue" },
	    { PieceType::White_Trebuchet,   "White_Trebuchet" },    { PieceType::Black_Trebuchet,   "Black_Trebuchet" },
	    { PieceType::White_Warlord,     "White_Warlord" },      { PieceType::Black_Warlord,     "Black_Warlord" },
	    { PieceType::White_Samurai,     "White_Samurai" },      { PieceType::Black_Samurai,     "Black_Samurai" },
	    { PieceType::White_Dragon,      "White_Dragon" },       { PieceType::Black_Dragon,      "Black_Dragon" }
    };
	const inline static std::string Placeholder = "Placeholder.png";
    const inline static std::string Null = "Null.png";
    const inline static std::string Board = "Board.jpg";

    const inline static std::string Gold_Bar_ = "Gold_Bar_";
    const inline static std::string Flux_Bar_ = "Flux_Bar_";

    const inline static std::string Gold_Bar_0 = "Gold_Bar_0";
    const inline static std::string Gold_Bar_1 = "Gold_Bar_1";
    const inline static std::string Gold_Bar_2 = "Gold_Bar_2";
    const inline static std::string Gold_Bar_3 = "Gold_Bar_3";
    const inline static std::string Gold_Bar_4 = "Gold_Bar_4";
    const inline static std::string Gold_Bar_5 = "Gold_Bar_5";
    const inline static std::string Gold_Bar_6 = "Gold_Bar_6";
    const inline static std::string Gold_Bar_7 = "Gold_Bar_7";
    const inline static std::string Gold_Bar_8 = "Gold_Bar_8";
    const inline static std::string Gold_Bar_9 = "Gold_Bar_9";

    const inline static std::string Flux_Bar_0 = "Flux_Bar_0";
    const inline static std::string Flux_Bar_1 = "Flux_Bar_1";
    const inline static std::string Flux_Bar_2 = "Flux_Bar_2";
    const inline static std::string Flux_Bar_3 = "Flux_Bar_3";
    const inline static std::string Flux_Bar_4 = "Flux_Bar_4";
    const inline static std::string Flux_Bar_5 = "Flux_Bar_5";
    const inline static std::string Flux_Bar_6 = "Flux_Bar_6";
    const inline static std::string Flux_Bar_7 = "Flux_Bar_7";
    const inline static std::string Flux_Bar_8 = "Flux_Bar_8";
    const inline static std::string Flux_Bar_9 = "Flux_Bar_9";
}

namespace Paths
{
    const inline static std::string Root = "../";
    const inline static std::string Assets = Root + "assets/";
    const inline static std::string Textures = Assets + "textures/";
    const inline static std::string Gold = Assets + "textures/bars/gold/";
    const inline static std::string Flux = Assets + "textures/bars/flux/";
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
    inline float AdjustableK = 1.f;
    inline bool MouseLeftPressed = false;
}

struct WindowSettings
{
	WindowSettings() = default;
    sf::State State = sf::State::Fullscreen;
    sf::Vector2u Resolution = { 0, 0 };
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

namespace Animation
{
    inline float EaseOutCubic(float t) {
        t = Clamp(t, 0.f, 1.f);
        return 1 - pow(1 - t, 3);
    }

    inline float EaseInCubic(float t) {
        t = Clamp(t, 0.f, 1.f);
        return t * t * t;
    }

    inline float EaseInQuadric(float t) {
        t = Clamp(t, 0.f, 1.f);
        return t * t;
    }

    inline float EaseInOutCubic(float t) {
        t = Clamp(t, 0.f, 1.f);
        return t < 0.5 ? 4 * t * t * t : 1 - std::pow(-2 * t + 2, 3) / 2;
    }

    // Function which increases linearly from 0 to the peak and then back down in the same fashion.
    // Peak is reached at x=peak.
    // Completes a cycle after the value peak*2.
    inline float RiseAndFall(float x, float peak)
    {
        return peak - std::abs(x - peak);
    }
}


