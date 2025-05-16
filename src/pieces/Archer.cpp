#include "Archer.h"

Archer::Archer(std::shared_ptr<Board> board, PieceColor color, uint32_t upgradeLevel)
	: BasePiece(board, color, upgradeLevel)
{
}

void Archer::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	const BoardMatrix& boardMatrix = m_Board->GetBoard();

	switch (Archer::m_UpgradeLevel)
	{
	case 1:
		for (int dy = -2; dy <= 2; dy++)
			for (int dx = -2; dx <= 2; dx++)
				if ((dx == 0 || dy == 0 || abs(dx) == abs(dy)) && (dx != 0 || dy != 0))
				{
					PiecePosition targetSquare = piecePosition + sf::Vector2i(dx, dy);
					if (IsCellInBounds(targetSquare, m_Board->GetSize()))
					{
						const auto& targetPiece = boardMatrix[targetSquare.y][targetSquare.x];
						if (targetPiece.get() == nullptr)
						{
							// Check if the target square is not occupied by a friendly piece
							if (!m_Board->IsTargetFriendly(PieceMove(piecePosition, targetSquare)))
							{
								legalMoves.push_back(ActionMove(targetSquare));
							}
						}
					}
				}
		break;
	case 2:
		for (int dy = -3; dy <= 3; dy++)
			for (int dx = -3; dx <= 3; dx++)
				if ((dx == 0 || dy == 0 || abs(dx) == abs(dy)) && (dx != 0 || dy != 0))
				{
					PiecePosition targetSquare = piecePosition + sf::Vector2i(dx, dy);
					if (IsCellInBounds(targetSquare, m_Board->GetSize()))
					{
						const auto& targetPiece = boardMatrix[targetSquare.y][targetSquare.x];
						if (targetPiece.get() == nullptr)
						{
							// Check if the target square is not occupied by a friendly piece
							if (!m_Board->IsTargetFriendly(PieceMove(piecePosition, targetSquare)))
							{
								legalMoves.push_back(ActionMove(targetSquare));
							}
						}
					}
				}
		break;
	case 3:
		for (int dy = -3; dy <= 3; dy++)
			for (int dx = -3; dx <= 3; dx++)
				if ((dx == 0 || dy == 0 || abs(dx) == abs(dy)) && (dx != 0 || dy != 0))
				{
					PiecePosition targetSquare = piecePosition + sf::Vector2i(dx, dy);
					if (IsCellInBounds(targetSquare, m_Board->GetSize()))
					{
						const auto& targetPiece = boardMatrix[targetSquare.y][targetSquare.x];
						if (targetPiece.get() == nullptr)
						{
							// Check if the target square is not occupied by a friendly piece
							if (!m_Board->IsTargetFriendly(PieceMove(piecePosition, targetSquare)))
							{
								legalMoves.push_back(ActionMove(targetSquare));
							}
						}
					}
				}
		break;
	}
}

//void Archer::ExecuteMove(BoardMatrix& board, PiecePosition piecePosition, ActionMove move)
//{
//	
//	{
//		// Normal move
//		PiecePosition targetSquare = move.TargetSquare;
//		// Move piece to the new square
//		board[targetSquare.y][targetSquare.x] = std::move(board[piecePosition.y][piecePosition.x]);
//		// Clear old position
//		board[piecePosition.y][piecePosition.x] = nullptr;
//	}
//}

PieceType Archer::GetPieceType()
{
	return (m_Color == PieceColor::White) ? PieceType::White_Archer : PieceType::Black_Archer;
}