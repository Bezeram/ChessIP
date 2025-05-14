#include "WhiteWitch.h"

void WhiteWitch::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType WhiteWitch::GetPieceType()
{
	return  PieceType::White_Witch;
}