#include "WhiteKnight.h"

void WhiteKnight::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType WhiteKnight::GetPieceType()
{
	return  PieceType::White_Knight;
}