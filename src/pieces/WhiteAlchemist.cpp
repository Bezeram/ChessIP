#include "WhiteAlchemist.h"

void WhiteAlchemist::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType WhiteAlchemist::GetPieceType()
{
	return  PieceType::White_Alchemist;
}