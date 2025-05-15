#include "WhiteRogue.h"

void WhiteRogue::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType WhiteRogue::GetPieceType()
{
	return  PieceType::White_Rogue;
}