#include "BlackTrebuchet.h"

void BlackTrebuchet::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType BlackTrebuchet::GetPieceType()
{
	return  PieceType::Black_Trebuchet;
}