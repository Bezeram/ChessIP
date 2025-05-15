#include "BlackArcher.h"

void BlackArcher::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	// TODO: calculate valid moves/attacks
}

PieceType BlackArcher::GetPieceType()
{
	return  PieceType::Black_King;
}