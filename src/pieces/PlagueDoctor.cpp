#include "PlagueDoctor.h"

PlagueDoctor::PlagueDoctor(std::shared_ptr<Board> board, PieceColor color, uint32_t upgradeLevel)
    : BasePiece(board, color, upgradeLevel)
{
}

PieceType PlagueDoctor::GetPieceType()
{
    return (m_Color == PieceColor::White) ? PieceType::White_PlagueDoctor : PieceType::Black_PlagueDoctor;
}

void PlagueDoctor::GetRange(PiecePosition piecePosition, std::vector<ActionMove>& legalMoves)
{
    // Mișcare ca regele
    for (int dy = -1; dy <= 1; ++dy)
        for (int dx = -1; dx <= 1; ++dx)
            if (dx != 0 || dy != 0)
            {
                PiecePosition target = piecePosition + sf::Vector2i(dx, dy);
                if (IsCellInBounds(target, m_Board->GetSize()))
                    legalMoves.push_back(ActionMove(target, MoveType::Move));
            }

    // Range de atac
    for (const auto& offset : GetAttackOffsets())
    {
        PiecePosition target = piecePosition + offset;
        if (IsCellInBounds(target, m_Board->GetSize()))
            legalMoves.push_back(ActionMove(target, MoveType::Action));
    }
}

void PlagueDoctor::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
    const BoardMatrix& boardMatrix = m_Board->GetBoard();

    // Mișcare ca regele
    for (int dy = -1; dy <= 1; ++dy)
        for (int dx = -1; dx <= 1; ++dx)
        {
            if (dx == 0 && dy == 0) continue;
            PiecePosition target = piecePosition + sf::Vector2i(dx, dy);
            if (IsCellInBounds(target, m_Board->GetSize()))
            {
                const auto& targetPiece = boardMatrix[target.y][target.x];
                if (targetPiece == nullptr || !m_Board->IsTargetFriendly(PieceMove(piecePosition, target)))
                    legalMoves.push_back(ActionMove(target, MoveType::Move));
            }
        }

    // Atacuri în offseturi
    for (const auto& offset : GetAttackOffsets())
    {
        PiecePosition target = piecePosition + offset;
        if (!IsCellInBounds(target, m_Board->GetSize())) continue;

        const auto& targetPiece = boardMatrix[target.y][target.x];
        if (targetPiece != nullptr && !m_Board->IsTargetFriendly(PieceMove(piecePosition, target)))
        {
            legalMoves.push_back(ActionMove(target, MoveType::Action));
        }
    }
}

void PlagueDoctor::ExecuteMove(BoardMatrix& board, PiecePosition piecePosition, ActionMove move)
{
    if (move.MoveType == MoveType::Move)
    {
        board[move.TargetSquare.y][move.TargetSquare.x] = std::move(board[piecePosition.y][piecePosition.x]);
        board[piecePosition.y][piecePosition.x] = nullptr;
    }
    else if (move.MoveType == MoveType::Action)
    {
        PiecePosition target = move.TargetSquare;
        auto& targetPiece = board[target.y][target.x];
        if (targetPiece != nullptr && !m_Board->IsTargetFriendly(PieceMove(piecePosition, target)))
        {
            ApplyPlagueEffect(board, target);
        }
    }
}

std::vector<sf::Vector2i> PlagueDoctor::GetAttackOffsets() const
{
    std::vector<sf::Vector2i> offsets = {
        {  2,  0 }, { -2,  0 }, {  0,  2 }, {  0, -2 },
        {  2,  1 }, { -2,  1 }, {  2, -1 }, { -2, -1 },
        {  1,  2 }, { -1,  2 }, {  1, -2 }, { -1, -2 },
        { -1,  0 }, {  1,  0 }, {  0,  1 }, {  0, -1 },
        { -1, -1 }, {  1, -1 }, {  1,  1 }, { -1,  1 }
    };

    if (m_UpgradeLevel >= 2)
    {
        offsets.insert(offsets.end(), {
            {  3,  0 }, { -3,  0 }, {  0,  3 }, {  0, -3 },
            {  3,  1 }, { -3,  1 }, {  3, -1 }, { -3, -1 },
            {  1,  3 }, { -1,  3 }, {  1, -3 }, { -1, -3 }
            });
    }

    return offsets;
}

void PlagueDoctor::ApplyPlagueEffect(BoardMatrix& board, PiecePosition center)
{
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            PiecePosition target = center + sf::Vector2i(dx, dy);
            if (!IsCellInBounds(target, m_Board->GetSize()))
                continue;

            auto& targetPiece = board[target.y][target.x];

            if (targetPiece != nullptr && !m_Board->IsTargetFriendly(PieceMove(center, target)))
            {
                // La orice nivel: afectează piesele din jur (stun 1 tură)
                targetPiece->AddEffect(Effect::Stun, 1);
            }

            if (m_UpgradeLevel == 3 && targetPiece == nullptr)
            {
                // La level 3: marchează pătratele goale ca inaccesibile temporar (cu efect Hex pe celulă)
               
                // m_Board->AddCellEffect(target, Effect::Hex, 1);
            }
        }
    }
}