#include "BlackKnight.h"

void BlackKnight::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	// TODO: calculate valid moves/attacks
}

PieceType BlackKnight::GetPieceType()
{
	return  PieceType::Black_Knight;
}