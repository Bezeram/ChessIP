#include "BlackWarlord.h"

void BlackWarlord::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType BlackWarlord::GetPieceType()
{
	return  PieceType::Black_Warlord;
}