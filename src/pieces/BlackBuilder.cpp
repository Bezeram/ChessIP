#include "BlackBuilder.h"

void BlackBuilder::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	// TODO: calculate valid moves/attacks
}

PieceType BlackBuilder::GetPieceType()
{
	return PieceType::Black_Builder;
}