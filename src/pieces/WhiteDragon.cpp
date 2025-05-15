#include "WhiteDragon.h"

void WhiteDragon::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType WhiteDragon::GetPieceType()
{
	return  PieceType::White_Dragon;
}