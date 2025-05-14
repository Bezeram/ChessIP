#include "BlackPlagueDoctor.h"

void BlackPlagueDoctor::GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves)
{
	// TODO: calculate valid moves/attacks
}

 PieceType BlackPlagueDoctor::GetPieceType()
{
	return  PieceType::Black_PlagueDoctor;
}