#pragma once

#include <vector>
#include <cmath>
#include <unordered_map>
#include <cassert>
#include <memory>

#include <SFML/Audio.hpp>
#include <SFML/Audio/Sound.hpp>

#include "ResourceManager.h"
#include "Utils.h"
#include "pieces/AllPiecesHeader.h"
#include "SoundPlayer.h"

/**
* First square starts from the bottom left, going to right and up
*/
class Board
{
public:
    Board(GameType gameType);
	~Board();

    const BoardMatrix& GetBoard() const;
        
    int GetSize() const;
    void DeletePieceAt(PiecePosition position);

    // Get the square at a coordinate
    std::unique_ptr<BasePiece>& operator[](PiecePosition position);
    const std::unique_ptr<BasePiece>& operator[](PiecePosition position) const;

	int GetFlux() const { return m_Flux; }
	int GetGold() const { return m_Gold; }
    // Negative values work
    void IncreaseFlux(int flux) 
    { 
        m_Flux += flux;
        m_Flux = Clamp(m_Flux, 0, 9);
    }
    // Negative values work
    void IncreaseGold(int gold)
    {
        m_Gold += gold;
        m_Gold = Clamp(m_Gold, 0, 9);
    }

    bool IsTargetFriendly(const PieceMove& move) const;
    bool IsValidPieceByTurn(PiecePosition position) const;
    bool IsWhitesMove() const;
    bool MakeMove(PiecePosition piecePosition, ActionMove actionMove);

    void Update();

	PiecePosition GetWhiteKingPosition() const;
	PiecePosition GetBlackKingPosition() const;

    void Init1v1Game(std::shared_ptr<Board>& boardRef);
private:
    void UpdateResources();
private:
    bool m_IsWhitesTurn = 1;

    int m_Flux = 0;
    int m_Gold = 0;

	int m_BlackFlux = 0;
	int m_BlackGold = 0;

    int m_Size = 0;
    int m_BoardSize = 0;
    std::vector<std::vector<std::unique_ptr<BasePiece>>> m_Board;
};

