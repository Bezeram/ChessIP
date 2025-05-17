#include "Archer.h"

Archer::Archer(std::shared_ptr<Board> board, PieceColor color, uint32_t upgradeLevel)
    : BasePiece(board, color, upgradeLevel)
{
}

// Cast ray to check if the path is clear for the move
bool CastRay(const BoardMatrix& board, PiecePosition piecePosition, ActionMove move) {
    PiecePosition targetSquare = move.TargetSquare;
    int dx = targetSquare.x - piecePosition.x;
    int dy = targetSquare.y - piecePosition.y;
    int stepX = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
    int stepY = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;
    for (int x = piecePosition.x + stepX, y = piecePosition.y + stepY;
        x != targetSquare.x || y != targetSquare.y; x += stepX, y += stepY) {
        if (board[y][x] != nullptr) {
            return false; // Path is blocked
        }
    }
    return true; // Path is clear
}

void Archer::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
    const BoardMatrix& boardMatrix = m_Board->GetBoard();

    // Mutări normale în jurul piesei
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dy != 0 || dx != 0) {
                PiecePosition targetSquare = piecePosition + sf::Vector2i(dx, dy);
                if (IsCellInBounds(targetSquare, m_Board->GetSize())) {
                    const auto& targetPiece = boardMatrix[targetSquare.y][targetSquare.x];
                    if (targetPiece == nullptr || !m_Board->IsTargetFriendly(PieceMove(piecePosition, targetSquare))) {
                        legalMoves.push_back(ActionMove(targetSquare, MoveType::Move));
                    }
                }
            }
        }
    }

    // Atacuri în funcție de nivel (vizualizează tot range-ul)
    int maxRange = (m_UpgradeLevel == 1) ? 2 : (m_UpgradeLevel == 2) ? 3 : 4;

    for (int dy = -maxRange; dy <= maxRange; dy++) {
        for (int dx = -maxRange; dx <= maxRange; dx++) {
            if ((dx == 0 && dy == 0) || !(dx == 0 || dy == 0 || abs(dx) == abs(dy)))
                continue;

            PiecePosition targetSquare = piecePosition + sf::Vector2i(dx, dy);
            if (!IsCellInBounds(targetSquare, m_Board->GetSize()))
                continue;

            if (CastRay(boardMatrix, piecePosition, ActionMove(targetSquare))) {
                legalMoves.push_back(ActionMove(targetSquare, MoveType::Action));  // colorăm toată raza
            }
        }
    }
}

bool Archer::ExecuteMove(BoardMatrix& board, PiecePosition piecePosition, ActionMove move)
{
    if (move.MoveType == MoveType::Move) {
        PiecePosition target = move.TargetSquare;
        board[target.y][target.x] = std::move(board[piecePosition.y][piecePosition.x]);
        board[piecePosition.y][piecePosition.x] = nullptr;
        return true;
    }
    else if (move.MoveType == MoveType::Action) {
        auto& targetPiece = board[move.TargetSquare.y][move.TargetSquare.x];

        // 1. Nu ataca dacă e pătrat gol
        if (targetPiece == nullptr)
            return false;

        // 2. Nu ataca dacă e piesă prietenă
        BasePiece* rawTarget = targetPiece.get();
        if (::GetPieceColor(rawTarget->GetPieceType()) == m_Color)
            return false;

        // 3. Atac valid – mută Archer un pătrat în direcția țintei
        int dx = move.TargetSquare.x - piecePosition.x;
        int dy = move.TargetSquare.y - piecePosition.y;
        int stepX = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
        int stepY = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;
        PiecePosition newPosition = piecePosition + sf::Vector2i(stepX, stepY);

        board[newPosition.y][newPosition.x] = std::move(board[piecePosition.y][piecePosition.x]);
        board[piecePosition.y][piecePosition.x] = nullptr;
        board[move.TargetSquare.y][move.TargetSquare.x] = nullptr;

        return true;
    }

    return false;
}

PieceType Archer::GetPieceType()
{
    return (m_Color == PieceColor::White) ? PieceType::White_Archer : PieceType::Black_Archer;
}
