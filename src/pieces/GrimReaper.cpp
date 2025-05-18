#include "GrimReaper.h"

GrimReaper::GrimReaper(std::shared_ptr<Board> board, PieceColor color, uint32_t upgradeLevel)
    : BasePiece(board, color, upgradeLevel)
{
}

int GrimReaper::GetCurseDelayTurns() const
{
    switch (m_UpgradeLevel)
    {
    case 1: return 3;
    case 2: return 3;
    case 3: return -2;
    default: return 3;
    }
}

void GrimReaper::GetRange(PiecePosition piecePosition, std::vector<ActionMove>& legalMoves)
{
    int range = (m_UpgradeLevel == 1) ? 1 : 2;

    for (int dy = -range; dy <= range; dy++)
    {
        for (int dx = -range; dx <= range; dx++)
        {
            if (dx == 0 && dy == 0) continue;

            PiecePosition target = piecePosition + sf::Vector2i(dx, dy);
            if (IsCellInBounds(target, m_Board->GetSize()))
            {
                legalMoves.push_back(ActionMove(target, MoveType::Move));
                legalMoves.push_back(ActionMove(target, MoveType::Action));
            }
        }
    }
}

void GrimReaper::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
    const BoardMatrix& boardMatrix = m_Board->GetBoard();
    int range = (m_UpgradeLevel == 1) ? 1 : 2;

    for (int dy = -range; dy <= range; dy++)
    {
        for (int dx = -range; dx <= range; dx++)
        {
            if (dx == 0 && dy == 0) continue;

            PiecePosition target = piecePosition + sf::Vector2i(dx, dy);
            if (!IsCellInBounds(target, m_Board->GetSize()))
                continue;

            const auto& targetPiece = boardMatrix[target.y][target.x];

            if (targetPiece == nullptr)
            {
                legalMoves.push_back(ActionMove(target, MoveType::Move));
            }
            else if (!m_Board->IsTargetFriendly(PieceMove(piecePosition, target)))
            {
                legalMoves.push_back(ActionMove(target, MoveType::Action));
            }
        }
    }
}

void GrimReaper::ExecuteMove(BoardMatrix& board, PiecePosition piecePosition, ActionMove move)
{
    if (move.MoveType == MoveType::Move)
    {
        PiecePosition target = move.TargetSquare;
        board[target.y][target.x] = std::move(board[piecePosition.y][piecePosition.x]);
        board[piecePosition.y][piecePosition.x] = nullptr;
    }
    else if (move.MoveType == MoveType::Action)
    {
        auto& targetPiece = board[move.TargetSquare.y][move.TargetSquare.x];

        if (targetPiece != nullptr && !m_Board->IsTargetFriendly(PieceMove(piecePosition, move.TargetSquare)))
        {
            // Aplicã efectul "curse" (capturare întârziatã)
            int turnsToDie = GetCurseDelayTurns();
            targetPiece->AddEffect(Effect::Curse, GetCurseDelayTurns());
        }
    }
}

PieceType GrimReaper::GetPieceType()
{
    return (m_Color == PieceColor::White) ? PieceType::White_GrimReaper : PieceType::Black_GrimReaper;
}
