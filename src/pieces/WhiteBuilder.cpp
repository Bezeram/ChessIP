#include "WhiteBuilder.h"

void WhiteBuilder::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType WhiteBuilder::GetPieceType()
{
	return  PieceType::White_Builder;
}