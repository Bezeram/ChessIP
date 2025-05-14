#pragma once

#include <vector>
#include <cmath>
#include <unordered_map>
#include <cassert>

#include <SFML/Audio.hpp>
#include <SFML/Audio/Sound.hpp>

#include "ResourceManager.h"
#include "Utils.h"
#include "SoundPlayer.h"

namespace ChessIP
{
    /**
    * Every piece type is stored in its own container
    * To check if a piece is in a given position, a map is used to locate
    the piece type to a corresponding container and index
    * First square starts from the top left, from white's perspective
    */
    class Board
    {
    public:
        Board(GameType gameType);

        void GetBoard(std::vector<Piece>& outBoard) const;
        
        int GetSize() const;
        PieceLocation GetPieceAt(PiecePos position) const;
        void DeletePieceAt(PiecePos position);

		int GetWhiteFlux() const { return m_WhiteFlux; }
		int GetWhiteGold() const { return m_WhiteGold; }

        bool IsTargetFriendly(const Move& move) const;
        bool IsValidPieceByTurn(PiecePos position) const;
        bool IsWhitesMove() const;
        bool MakeMove(const Move& move);
    private:
        void MapPositions(std::vector<PiecePos>* data, int size, PieceType type);
        void Init1v1Game();
    private:
        int m_Size = 2;
        bool m_IsWhitesTurn = 1;

        int m_WhiteFlux = 0;
        int m_WhiteGold = 0;

		int m_BlackFlux = 0;
		int m_BlackGold = 0;

        std::unordered_map<PiecePos, PieceLocation> m_PositionToPiece;

        std::vector<PiecePos> m_WhitePawns;
        std::vector<PiecePos> m_WhiteKnights;
        std::vector<PiecePos> m_WhiteBishops;
        std::vector<PiecePos> m_WhiteRooks;
        std::vector<PiecePos> m_WhiteQueen;
        PiecePos m_WhiteKing;

        std::vector<PiecePos> m_BlackPawns;
        std::vector<PiecePos> m_BlackKnights;
        std::vector<PiecePos> m_BlackBishops;
        std::vector<PiecePos> m_BlackRooks;
        std::vector<PiecePos> m_BlackQueen;
        PiecePos m_BlackKing;

    };
}

