#pragma once

#include "BasePiece.h"
#include "../Utils.h"

class Archer : public BasePiece
{
public:
    Archer(std::shared_ptr<Board> board, PieceColor color, uint32_t upgradeLevel = 1);
    void GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves) override;
    bool ExecuteMove(BoardMatrix& board, PiecePosition piecePosition, ActionMove move) override;
    PieceType GetPieceType();

private:
};
