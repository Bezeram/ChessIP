#pragma once

#include <vector>
#include <cmath>
#include <array>
#include <unordered_map>

#include "Utils.h"

namespace ChessAI
{
    /**
    * Every piece type is stored in its own container
    * To check if a piece is in a given position, a map is used to locate
    the piece type to a corresponding container and index
    */
    class Board
    {
    public:
        typedef int PiecePos;

        Board();
        Board(const char* FEN);

        void GetBoard(std::vector<Piece>& outBoard) const;

        void MakeMove(int startSquare, int targetSquare);
        void MakeMove(const Move& move);
    private:
        bool m_WhitesTurn = 1;

        struct PieceLocation
        {
            PieceLocation() = default;
            PieceLocation(PiecePos* container, int index, PieceType type) : Container(container), Type(type), Index(index) {}
            PiecePos* Container = nullptr;
            PieceType Type;
            int Index = -1;
        };

        std::unordered_map<PiecePos, const PieceLocation> m_PositionToPiece;

        std::array<PiecePos, 8> m_WhitePawns;
        std::array<PiecePos, 2> m_WhiteKnights;
        std::array<PiecePos, 2> m_WhiteBishops;
        std::array<PiecePos, 2> m_WhiteRooks;
        PiecePos m_WhiteQueen;
        PiecePos m_WhiteKing;

        std::array<PiecePos, 8> m_BlackPawns;
        std::array<PiecePos, 2> m_BlackKnights;
        std::array<PiecePos, 2> m_BlackBishops;
        std::array<PiecePos, 2> m_BlackRooks;
        PiecePos m_BlackQueen;
        PiecePos m_BlackKing;
    };
}

