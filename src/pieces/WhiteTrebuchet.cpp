#include "WhiteTrebuchet.h"

void WhiteTrebuchet::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType WhiteTrebuchet::GetPieceType()
{
	return  PieceType::White_Trebuchet;
}