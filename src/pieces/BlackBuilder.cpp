#include "BlackBuilder.h"

void BlackBuilder::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

PieceType BlackBuilder::GetPieceType()
{
	return PieceType::Black_Builder;
}