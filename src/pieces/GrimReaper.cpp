#include "GrimReaper.h"

void GrimReaper::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	// TODO: calculate valid moves/attacks
}

PieceType GrimReaper::GetPieceType()
{
	return  PieceType::White_GrimReaper;
}