#pragma once

#include "BasePiece.h"
#include "../Utils.h"

class Rogue : public BasePiece
{
public:
    Rogue(std::shared_ptr<Board> board, PieceColor color, uint32_t upgradeLevel = 1);
    void GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves) override;
    void GetRange(PiecePosition piecePosition, std::vector<ActionMove>& legalMoves) override;
    void ExecuteMove(BoardMatrix& board, PiecePosition piecePosition, ActionMove move) override;
    PieceType GetPieceType() override;

private:
    bool HasEffect(Effect effect) const;
};
