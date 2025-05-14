#include "BlackRogue.h"

void BlackRogue::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType BlackRogue::GetPieceType()
{
	return  PieceType::Black_Rogue;
}