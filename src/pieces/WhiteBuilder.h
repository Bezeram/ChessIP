#pragma once

#include "BasePiece.h"
#include "../Utils.h"

class WhiteBuilder : public BasePiece
{
public:
	void GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves) override;
	void GetRange(PiecePosition piecePosition, std::vector<ActionMove>& legalMoves) override;
	PieceType GetPieceType();

private:
	// TODO: Implement ExecuteMove() if necessary
};