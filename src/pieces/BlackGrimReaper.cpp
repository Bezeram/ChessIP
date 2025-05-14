#include "BlackGrimReaper.h"

void BlackGrimReaper::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

PieceType BlackGrimReaper::GetPieceType()
{
	return PieceType::Black_GrimReaper;
}