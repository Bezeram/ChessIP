#pragma once

#include <vector>
#include <cmath>
#include <unordered_map>
#include <cassert>

#include "Utils.h"

namespace ChessIP
{
    /**
    * Every piece type is stored in its own container
    * To check if a piece is in a given position, a map is used to locate
    the piece type to a corresponding container and index
    */
    class Board
    {
    public:

        Board(GameType gameType);
        Board(GameType gameType, const char* FEN);

        void GetBoard(std::vector<Piece>& outBoard) const;
        
        PieceLocation GetPieceAt(PiecePos position) const;
        void DeletePieceAt(PiecePos position);

        bool IsTargetFriendly(const Move& move) const;
        bool IsValidPieceByTurn(PiecePos position) const;
        bool IsWhitesMove() const;
        bool MakeMove(const Move& move);
        
    private:
        bool m_IsWhitesTurn = 1;

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

