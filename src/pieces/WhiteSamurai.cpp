#include "WhiteSamurai.h"

void WhiteSamurai::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType WhiteSamurai::GetPieceType()
{
	return  PieceType::White_Samurai;
}