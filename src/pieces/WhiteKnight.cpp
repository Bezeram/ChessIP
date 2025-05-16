#include "WhiteKnight.h"

void WhiteKnight::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType WhiteKnight::GetPieceType()
{
	return  PieceType::White_Knight;
}