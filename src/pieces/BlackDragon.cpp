#include "BlackDragon.h"

void BlackDragon::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

PieceType BlackDragon::GetPieceType()
{
	return PieceType::Black_Dragon;
}