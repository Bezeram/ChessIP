#include "WhiteWarlord.h"

void WhiteWarlord::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType WhiteWarlord::GetPieceType()
{
	return  PieceType::White_Warlord;
}