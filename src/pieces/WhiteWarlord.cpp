#include "WhiteWarlord.h"

void WhiteWarlord::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType WhiteWarlord::GetPieceType()
{
	return  PieceType::White_Warlord;
}