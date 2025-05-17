#include "WhiteBuilder.h"

void WhiteBuilder::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType WhiteBuilder::GetPieceType()
{
	return  PieceType::White_Builder;
}