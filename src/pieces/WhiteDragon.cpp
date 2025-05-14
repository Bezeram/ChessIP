#include "WhiteDragon.h"

void WhiteDragon::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType WhiteDragon::GetPieceType()
{
	return  PieceType::White_Dragon;
}