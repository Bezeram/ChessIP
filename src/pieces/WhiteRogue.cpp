#include "WhiteRogue.h"

void WhiteRogue::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType WhiteRogue::GetPieceType()
{
	return  PieceType::White_Rogue;
}