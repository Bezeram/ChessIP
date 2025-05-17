#include "Trebuchet.h"
#include <algorithm>

Trebuchet::Trebuchet(std::shared_ptr<Board> board, PieceColor color, uint32_t upgradeLevel)
    : BasePiece(board, color, upgradeLevel) {}

void Trebuchet::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
    const BoardMatrix& boardMatrix = m_Board->GetBoard();

    // 🔹 Mișcare: doar în cruce la 1 pătrat
    for (auto dir : std::vector<sf::Vector2i>{ {1, 0}, {-1, 0}, {0, 1}, {0, -1} })
    {
        PiecePosition target = piecePosition + dir;
        if (IsCellInBounds(target, m_Board->GetSize()))
        {
            const auto& targetPiece = boardMatrix[target.y][target.x];
            if (targetPiece == nullptr || !m_Board->IsTargetFriendly(PieceMove(piecePosition, target)))
            {
                legalMoves.push_back(ActionMove(target, MoveType::Move));
            }
        }
    }

    //  Atac: în funcție de level, folosește offseturi predefinite
    AddPredefinedAttackOffsets(legalMoves, piecePosition, m_UpgradeLevel);
}

bool Trebuchet::ExecuteMove(BoardMatrix& board, PiecePosition piecePosition, ActionMove move)
{
    if (move.MoveType == MoveType::Move)
    {
        PiecePosition target = move.TargetSquare;
        board[target.y][target.x] = std::move(board[piecePosition.y][piecePosition.x]);
        board[piecePosition.y][piecePosition.x] = nullptr;
        return true;
    }
    else if (move.MoveType == MoveType::Action)
    {
        auto& targetPiece = board[move.TargetSquare.y][move.TargetSquare.x];

        //  Nu atacăm dacă nu e nicio piesă
        if (targetPiece == nullptr)
            return false;

        // ❌ Nu atacăm piesă proprie
        BasePiece* rawTarget = targetPiece.get();
        if (::GetPieceColor(rawTarget->GetPieceType()) == m_Color)
            return false;

        //  Atac valid – distruge ținta, fără să se mute
        board[move.TargetSquare.y][move.TargetSquare.x].reset();
        return true;
    }

    return false;
}


PieceType Trebuchet::GetPieceType()
{
    return (m_Color == PieceColor::White) ? PieceType::White_Trebuchet : PieceType::Black_Trebuchet;
}

// 🔽 Offseturi de atac predefinite pentru fiecare level (exact ca în imagine)
void Trebuchet::AddPredefinedAttackOffsets(std::vector<ActionMove>& legalMoves, PiecePosition center, int upgradeLevel)
{
    static const std::vector<sf::Vector2i> level1 = {
        {  2,  0 }, { -2,  0 },
        {  2, -1 }, { -2, -1 },
        {  2,  1 }, { -2,  1 },
        {  0,  2 }, {  0, -2 },
        {  1,  2 }, {  1, -2 },
        { -1,  2 }, { -1, -2 }
    };

    static const std::vector<sf::Vector2i> level2 = {
        {  3,  0 }, { -3,  0 },
        {  3, -1 }, { -3, -1 },
        {  3,  1 }, { -3,  1 },
        {  0,  3 }, {  0, -3 },
        {  1,  3 }, {  1, -3 },
        { -1,  3 }, { -1, -3 }
    };

    static const std::vector<sf::Vector2i> level3 = {
        {  4,  0 }, { -4,  0 },
        {  4, -1 }, { -4, -1 },
        {  4,  1 }, { -4,  1 },
        {  0,  4 }, {  0, -4 },
        {  1,  4 }, {  1, -4 },
        { -1,  4 }, { -1, -4 }
    };

    // Always add level1
    for (const auto& offset : level1)
    {
        PiecePosition pos = center + offset;
        if (IsCellInBounds(pos, m_Board->GetSize()))
            legalMoves.push_back(ActionMove(pos, MoveType::Action));
    }

    // Add level2 if upgradeLevel >= 2
    if (upgradeLevel >= 2)
    {
        for (const auto& offset : level2)
        {
            PiecePosition pos = center + offset;
            if (IsCellInBounds(pos, m_Board->GetSize()))
                legalMoves.push_back(ActionMove(pos, MoveType::Action));
        }
    }

    // Add level3 if upgradeLevel == 3
    if (upgradeLevel >= 3)
    {
        for (const auto& offset : level3)
        {
            PiecePosition pos = center + offset;
            if (IsCellInBounds(pos, m_Board->GetSize()))
                legalMoves.push_back(ActionMove(pos, MoveType::Action));
        }
    }
}
