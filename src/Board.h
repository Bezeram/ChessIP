#pragma once

#include <vector>
#include <cmath>
#include <array>
#include <unordered_map>

namespace ChessAI
{
    enum class PieceType
    {
        White_Pawn = 0,
        White_Knight,
        White_Bishop,
        White_Rook,
        White_Queen,
        White_King,
        Black_Pawn,
        Black_Knight,
        Black_Bishop,
        Black_Rook,
        Black_Queen,
        Black_King,
    };

    struct Move
    {
        Move(int startSquare, int targetSquare) : StartSquare(startSquare), TargetSquare(targetSquare) {};
        const int StartSquare;
        const int TargetSquare;
    };

    struct Piece
    {
        Piece(int position, PieceType type) : Position(position), Type(type) {}
        int Position;
        PieceType Type;
    };

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

        void GetBoard(std::vector<Piece>& outBoard);

        void MakeMove(int startSquare, int targetSquare);
        void MakeMove(const Move& move);
    private:
        struct PieceLocation
        {
            PieceLocation() = default;
            PieceLocation(PiecePos* container, int index) : Container(container), Index(index) {}
            PiecePos* Container = nullptr;
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

