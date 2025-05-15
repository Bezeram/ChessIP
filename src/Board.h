#pragma once

#include <vector>
#include <cmath>
#include <unordered_map>
#include <cassert>
#include <memory>

#include "Utils.h"
#include "pieces/AllPiecesHeader.h"

/**
* First square starts from the bottom left, going to right and up
*/
class Board
{
public:
    Board(GameType gameType);

    const BoardMatrix& GetBoard() const;
        
    int GetSize() const;
    void DeletePieceAt(PiecePosition position);

    // Get the square at a coordinate
    //std::unique_ptr<BasePiece>& operator[](PiecePosition position);
    const std::unique_ptr<BasePiece>& operator[](PiecePosition position) const;

	int GetWhiteFlux() const { return m_WhiteFlux; }
	int GetWhiteGold() const { return m_WhiteGold; }

    bool IsTargetFriendly(const PieceMove& move) const;
    bool IsValidPieceByTurn(PiecePosition position) const;
    bool IsWhitesMove() const;
    bool MakeMove(PiecePosition piecePosition, PieceMove move);

	PiecePosition GetWhiteKingPosition() const;
	PiecePosition GetBlackKingPosition() const;

    bool IsCellOnBoard(const sf::Vector2i& cellIndex) const;

private:
    void Init1v1Game();
    void UpdateResources();
private:
    bool m_IsWhitesTurn = 1;

    int m_WhiteFlux = 0;
    int m_WhiteGold = 0;

	int m_BlackFlux = 0;
	int m_BlackGold = 0;

    int m_Size = 0;
    int m_BoardSize = 0;
    std::vector<std::vector<std::unique_ptr<BasePiece>>> m_Board;
};

