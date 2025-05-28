#pragma once

#include "Utils.h"
#include "pieces/AllPiecesHeader.h"

class PieceFactory
{
public:
	inline static BasePiece* Create(PieceType type, PieceColor color, std::shared_ptr<Board> boardRef)
	{
		switch (type)
		{
        case PieceType::White_King:
            return new King(boardRef, PieceColor::White);
        case PieceType::White_Witch:
            return new Witch(boardRef, PieceColor::White);
        case PieceType::White_Alchemist:
            return new Alchemist(boardRef, PieceColor::White);
        case PieceType::White_Archer:
            return new Archer(boardRef, PieceColor::White);
        case PieceType::White_GrimReaper:
            return new GrimReaper(boardRef, PieceColor::White);
        case PieceType::White_PlagueDoctor:
            return new PlagueDoctor(boardRef, PieceColor::White);
        case PieceType::White_Rogue:
            return new Rogue(boardRef, PieceColor::White);
        case PieceType::White_Trebuchet:
            return new Trebuchet(boardRef, PieceColor::White);
        case PieceType::White_Builder:
            return new Builder(boardRef, PieceColor::White);
        case PieceType::Black_King:
            return new King(boardRef, PieceColor::Black);
        case PieceType::Black_Witch:
            return new Witch(boardRef, PieceColor::Black);
        case PieceType::Black_Knight:
            return new Archer(boardRef, PieceColor::Black);
        case PieceType::Black_Alchemist:
            return new Alchemist(boardRef, PieceColor::Black);
        case PieceType::Black_Archer:
            return new Archer(boardRef, PieceColor::Black);
        case PieceType::Black_GrimReaper:
            return new GrimReaper(boardRef, PieceColor::Black);
        case PieceType::Black_PlagueDoctor:
            return new PlagueDoctor(boardRef, PieceColor::Black);
        case PieceType::Black_Rogue:
            return new Rogue(boardRef, PieceColor::Black);
        case PieceType::Black_Trebuchet:
            return new Trebuchet(boardRef, PieceColor::Black);
        case PieceType::Black_Builder:
            return new Builder(boardRef, PieceColor::Black);
        default:
            throw "[PieceFactory] Bruh i don't know that piece.";
		}
	}
};