#include "BlackKing.h"

void BlackKing::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	// TODO: calculate valid moves/attacks
}

PieceType BlackKing::GetPieceType()
{
	return PieceType::Black_King;
}