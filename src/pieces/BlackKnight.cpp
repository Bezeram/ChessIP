#include "BlackKnight.h"

void BlackKnight::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

PieceType BlackKnight::GetPieceType()
{
	return  PieceType::Black_Knight;
}