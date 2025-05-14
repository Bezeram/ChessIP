#pragma once

#include "BasePiece.h"
#include "../Utils.h"

class BlackArcher : public BasePiece
{
public:
	void GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& validMoves) override;
	PieceType GetPieceType();
private:
	// TODO: Implement ExecuteMove() if necessary
};