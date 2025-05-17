#include "Wall.h"

Wall::Wall(std::shared_ptr<Board> board, PieceColor color, uint32_t upgradeLevel)
	: BasePiece(board, color, upgradeLevel)
{
}

void Wall::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	legalMoves.clear();
}


PieceType Wall::GetPieceType()
{
	return (m_Color == PieceColor::White) ? PieceType::White_Wall : PieceType::Black_Wall;
}