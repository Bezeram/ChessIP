#include "WhiteArcher.h"

void WhiteArcher::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType WhiteArcher::GetPieceType()
{
	return  PieceType::White_Archer;
}