#include "BlackSamurai.h"

void BlackSamurai::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType BlackSamurai::GetPieceType()
{
	return  PieceType::Black_Samurai;
}