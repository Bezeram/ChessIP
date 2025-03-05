#pragma once

#include <iostream>

namespace ChessIP
{
    typedef int PiecePos;

    enum class PieceColor
    {
        White = 0,
        Black,
    };

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

    enum class GameType
    {
        OneVOne,
        TwoVTwo,
    };

    struct Move
    {
        Move(int startSquare, int targetSquare) : StartSquare(startSquare), TargetSquare(targetSquare) {};
        Move(const Move&) = default;
        int StartSquare;
        int TargetSquare;
    };

    struct Piece
    {
        Piece(int position, PieceType type) : Position(position), Type(type) {}
        int Position;
        PieceType Type;
    };

    class PieceLocation
    {
    public:
        PieceLocation(std::vector<PiecePos>* container, int index, PieceType type) : m_Container(container), m_Type(type), m_Index(index) {}
        static PieceLocation EmptySquare() { return PieceLocation(nullptr, -1, PieceType::White_Pawn); }
        static PieceLocation WhiteKing(PiecePos* king) { return PieceLocation(king, PieceType::White_King); }
        static PieceLocation BlackKing(PiecePos* king) { return PieceLocation(king, PieceType::Black_King); }

        PieceType GetType() const { return m_Type; }
        bool IsEmptySquare() const 
        { 
            // TODO:
            // Empty squares should be based on an additional NULL piece type, not based on index to some container
            return m_Index == -1; 
        };
        bool IsKing() const
        {
            return m_King != nullptr;
        }
        PiecePos GetPosition() const
        {
            if (!IsEmptySquare())
            {
                if (IsKing())
                    return *m_King;
                return m_Container->at(m_Index);
            }
            return -1;
        }
        void SetPosition(PiecePos position) 
        {
            if (!IsEmptySquare())
            {
                if (IsKing())
                    *m_King = position;
                else
                    m_Container->at(m_Index) = position;
            }
        }
        void Delete(std::unordered_map<PiecePos, PieceLocation>& positionToPieceLocation)
        { 
            if (!IsEmptySquare())
            {
                if (IsKing())
                {
                    *m_King = -1;
                    m_King = nullptr;
                }
                else
                {
                    // All of the items AFTER the current index in the container (vector) will correspond to entries
                    // in the map which point to incorrect places back in the vector. 
                    // Every index must be lowered by 1 to account for the current element being erased from the container
                    for (int i = m_Index + 1; i < m_Container->size(); i++)
                    {
                        PiecePos position = m_Container->at(i);
                        PieceLocation& pieceLoc = positionToPieceLocation.at(position);
                        pieceLoc.m_Index--;
                        i = i + 0;
                    }

                    m_Container->erase(m_Container->begin() + m_Index);
                    m_Index = -1;
                }
            }
        }

    private:
        PieceLocation(PiecePos* king, PieceType type) : m_King(king), m_Type(type), m_Index(0) {}

        std::vector<PiecePos>* m_Container = nullptr;
        PiecePos* m_King = nullptr;
        PieceType m_Type = PieceType::White_Pawn;
        int m_Index = -1;
    };

    inline bool IsWhitePiece(PieceType type)
    {
        return int(type) < 6;
    }

    inline bool IsBlackPiece(PieceType type)
    {
        return int(type) >= 6;
    }

    inline PieceColor GetPieceColor(PieceType type)
    {
        if (IsWhitePiece(type))
            return PieceColor::White;
        return PieceColor::Black;
    }

    // Global vars
    inline float g_AdjustableK = 0.1f;
}
