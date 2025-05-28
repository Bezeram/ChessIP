#include "Rogue.h"

Rogue::Rogue(std::shared_ptr<Board> board, PieceColor color, uint32_t upgradeLevel)
    : BasePiece(board, color, upgradeLevel)
{
}

void Rogue::GetRange(PiecePosition piecePosition, std::vector<ActionMove>& legalMoves)
{
    int maxRange = m_UpgradeLevel + 1; // level 1 = 2 range, level 2 = 3, level 3 = 4

    for (int i = 1; i <= maxRange; ++i)
    {
        for (auto dir : std::vector<sf::Vector2i>{ {i, 0}, {-i, 0}, {0, i}, {0, -i} })
        {
            PiecePosition target = piecePosition + dir;
            if (IsCellInBounds(target, m_Board->GetSize()))
            {
                legalMoves.push_back(ActionMove(target, MoveType::Move));
                legalMoves.push_back(ActionMove(target, MoveType::Action));
            }
        }
    }
}

void Rogue::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
    const BoardMatrix& boardMatrix = m_Board->GetBoard();

    if (HasEffect(Effect::Stun))
    {
        legalMoves.clear();
        return;
    }

    int maxRange = m_UpgradeLevel + 1;

    for (int i = 1; i <= maxRange; ++i)
    {
        for (auto dir : std::vector<sf::Vector2i>{ {i, 0}, {-i, 0}, {0, i}, {0, -i} })
        {
            PiecePosition target = piecePosition + dir;
            if (!IsCellInBounds(target, m_Board->GetSize()))
                continue;

            const auto& targetPiece = boardMatrix[target.y][target.x];

            if (targetPiece == nullptr)
            {
                legalMoves.push_back(ActionMove(target, MoveType::Move));
            }
            else if (!m_Board->IsTargetFriendly(PieceMove(piecePosition, target)))
            {
                // Poate ataca doar dacă NU are efectul Hex
                if (!HasEffect(Effect::Hex))
                    legalMoves.push_back(ActionMove(target, MoveType::Action));
            }
        }
    }
}

void Rogue::ExecuteMove(BoardMatrix& board, PiecePosition piecePosition, ActionMove move)
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
            // Ia ownership temporar
            std::unique_ptr<BasePiece> roguePiece = std::move(board[piecePosition.y][piecePosition.x]);

            // Elimină ținta
            targetPiece.reset();

            // Aplică efectele direct pe obiectul *roguePiece*
            roguePiece->AddEffect(Effect::Alchemist_Shield, 2);
            roguePiece->AddEffect(Effect::Stun, 2);

            // Pune Rogue la noua locație
            board[move.TargetSquare.y][move.TargetSquare.x] = std::move(roguePiece);
        }
    }
}

PieceType Rogue::GetPieceType()
{
    return (m_Color == PieceColor::White) ? PieceType::White_Rogue : PieceType::Black_Rogue;
}

bool Rogue::HasEffect(Effect effect) const
{
    for (const auto& ef : m_Effects)
        if (std::get<0>(ef) == effect)
            return true;
    return false;
}
