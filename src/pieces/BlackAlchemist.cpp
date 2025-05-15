#include "BlackAlchemist.h"

void BlackAlchemist::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	// TODO: calculate valid moves/attacks
}

PieceType BlackAlchemist::GetPieceType()
{
	return PieceType::Black_King;
}