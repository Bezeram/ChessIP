#include "BlackAlchemist.h"

void BlackAlchemist::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

PieceType BlackAlchemist::GetPieceType()
{
	return PieceType::Black_King;
}