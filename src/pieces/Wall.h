#pragma once

#include "BasePiece.h"
#include "../Utils.h"

class Wall : public BasePiece
{
public:
	Wall(std::shared_ptr<Board> board, PieceColor color, uint32_t upgradeLevel = 1);
	void GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves) override;
	void GetRange(PiecePosition piecePosition, std::vector<ActionMove>& legalMoves) override;
	PieceType GetPieceType();

private:
};