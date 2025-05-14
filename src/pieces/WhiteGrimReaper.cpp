#include "WhiteGrimReaper.h"

void WhiteGrimReaper::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType WhiteGrimReaper::GetPieceType()
{
	return  PieceType::White_GrimReaper;
}