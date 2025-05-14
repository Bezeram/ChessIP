#include "WhitePlagueDoctor.h"

void WhitePlagueDoctor::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType WhitePlagueDoctor::GetPieceType()
{
	return  PieceType::White_PlagueDoctor;
}