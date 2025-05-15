#include "BlackSamurai.h"

void BlackSamurai::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType BlackSamurai::GetPieceType()
{
	return  PieceType::Black_Samurai;
}