#include "BlackWarlord.h"

void BlackWarlord::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType BlackWarlord::GetPieceType()
{
	return  PieceType::Black_Warlord;
}