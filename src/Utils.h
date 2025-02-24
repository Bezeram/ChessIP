#pragma once

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
}
