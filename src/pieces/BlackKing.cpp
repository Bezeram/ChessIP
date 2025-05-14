#include "BlackKing.h"

void BlackKing::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

PieceType BlackKing::GetPieceType()
{
	return PieceType::Black_King;
}