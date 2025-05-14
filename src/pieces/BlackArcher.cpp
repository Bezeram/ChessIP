#include "BlackArcher.h"

void BlackArcher::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

PieceType BlackArcher::GetPieceType()
{
	return  PieceType::Black_King;
}