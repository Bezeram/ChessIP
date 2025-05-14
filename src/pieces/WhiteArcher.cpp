#include "WhiteArcher.h"

void WhiteArcher::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType WhiteArcher::GetPieceType()
{
	return  PieceType::White_Archer;
}