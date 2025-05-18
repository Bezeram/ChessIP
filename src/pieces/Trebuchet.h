#pragma once

#include "BasePiece.h"
#include "../Utils.h"

class Trebuchet : public BasePiece
{
public:
	Trebuchet(std::shared_ptr<Board> board, PieceColor color, uint32_t upgradeLevel = 1);
	void GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves) override;
	void GetRange(PiecePosition piecePosition, std::vector<ActionMove>& legalMoves) override;
	void ExecuteMove(BoardMatrix& board, PiecePosition piecePosition, ActionMove move) override;
	PieceType GetPieceType() override;

private:
	void AddAttackOffsets(std::vector<ActionMove>& legalMoves, PiecePosition center, int level);
};
