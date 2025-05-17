#pragma once

#include "BasePiece.h"
#include "../Utils.h"

class Trebuchet : public BasePiece
{
public:
    Trebuchet(std::shared_ptr<Board> board, PieceColor color, uint32_t upgradeLevel = 1);

    void GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves) override;
    bool ExecuteMove(BoardMatrix& board, PiecePosition piecePosition, ActionMove move) override;
    PieceType GetPieceType() override;

private:
    void AddRadiusPattern(std::vector<ActionMove>& legalMoves, PiecePosition center, int radius);
    void AddPlusPattern(std::vector<ActionMove>& legalMoves, PiecePosition center, int range, bool exactDistance);
    void AddManhattanDistance2(std::vector<ActionMove>& legalMoves, PiecePosition center);
    void AddPredefinedAttackOffsets(std::vector<ActionMove>& legalMoves, PiecePosition center, int upgradeLevel);

};
