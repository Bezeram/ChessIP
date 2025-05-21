#include "Archer.h"

Archer::Archer(std::shared_ptr<Board> board, PieceColor color, uint32_t upgradeLevel)
	: BasePiece(board, color, upgradeLevel)
{
}

// Cast ray to check if the path is clear for the move
void Archer::GetRange(PiecePosition piecePosition, std::vector<ActionMove>& legalMoves) {
	for (int dy = -1; dy <= 1; dy++)
		for (int dx = -1; dx <= 1; dx++)
			if (dy != 0 || dx != 0)
			{
				PiecePosition targetSquare = piecePosition + sf::Vector2i(dx, dy);
				if (IsCellInBounds(targetSquare, m_Board->GetSize()))
				{
					legalMoves.push_back(ActionMove(targetSquare, MoveType::Move));
				}
			}

	int range = m_UpgradeLevel + 1;
	for (int dy = -range; dy <= range; dy++)
		for (int dx = -range; dx <= range; dx++)
			if ((dx == 0 || dy == 0 || abs(dx) == abs(dy)) && (dx != 0 || dy != 0))
			{
				PiecePosition targetSquare = piecePosition + sf::Vector2i(dx, dy);
				if (IsCellInBounds(targetSquare, m_Board->GetSize()))
				{
					legalMoves.push_back(ActionMove(targetSquare, MoveType::Move));
					legalMoves.push_back(ActionMove(targetSquare, MoveType::Action));
				}
			}
}

void Archer::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	const BoardMatrix& boardMatrix = m_Board->GetBoard();


	// Logic for generating move commands
	for (int dy = -1; dy <= 1; dy++)
		for (int dx = -1; dx <= 1; dx++)
			if (dy != 0 || dx != 0)
			{
				PiecePosition targetSquare = piecePosition + sf::Vector2i(dx, dy);
				if (IsCellInBounds(targetSquare, m_Board->GetSize()))
				{
					const auto& targetPiece = boardMatrix[targetSquare.y][targetSquare.x];
					if (targetPiece.get() == nullptr || !m_Board->IsTargetFriendly(PieceMove(piecePosition, targetSquare)))
					{
						// Check if the target square is not occupied by a friendly piece
						legalMoves.push_back(ActionMove(targetSquare));
					}
				}
			}

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
						if (targetPiece.get() != nullptr)
						{
							// Check if the target square is not occupied by a friendly piece
							if (!m_Board->IsTargetFriendly(PieceMove(piecePosition, targetSquare)))
							{
								if (CastRay(boardMatrix, piecePosition, ActionMove(targetSquare))) // Check if the path is clear
								{
									legalMoves.push_back(ActionMove(targetSquare));
								}
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
						if (targetPiece.get() != nullptr)
						{
							// Check if the target square is not occupied by a friendly piece
							if (!m_Board->IsTargetFriendly(PieceMove(piecePosition, targetSquare)))
							{
								if (CastRay(boardMatrix, piecePosition, ActionMove(targetSquare))) // Check if the path is clear
								{
									legalMoves.push_back(ActionMove(targetSquare));
								}
							}
						}
					}
				}
		break;
	case 3:
		for (int dy = -4; dy <= 4; dy++)
			for (int dx = -4; dx <= 4; dx++)
				if ((dx == 0 || dy == 0 || abs(dx) == abs(dy)) && (dx != 0 || dy != 0))
				{
					PiecePosition targetSquare = piecePosition + sf::Vector2i(dx, dy);
					if (IsCellInBounds(targetSquare, m_Board->GetSize()))
					{
						const auto& targetPiece = boardMatrix[targetSquare.y][targetSquare.x];
						if (targetPiece.get() != nullptr)
						{
							// Check if the target square is not occupied by a friendly piece
							if (!m_Board->IsTargetFriendly(PieceMove(piecePosition, targetSquare)))
							{
								if (CastRay(boardMatrix, piecePosition, ActionMove(targetSquare))) // Check if the path is clear
								{
									legalMoves.push_back(ActionMove(targetSquare));
								}
							}
						}
					}
				}
		break;
	}
}

void Archer::ExecuteMove(BoardMatrix& board, PiecePosition piecePosition, ActionMove move)
{
	// Move move
	if (board[move.TargetSquare.y][move.TargetSquare.x] == nullptr)
	{
		// Normal move
		PiecePosition targetSquare = move.TargetSquare;
		// Move piece to the new square
		board[targetSquare.y][targetSquare.x] = std::move(board[piecePosition.y][piecePosition.x]);
		// Clear old position
		board[piecePosition.y][piecePosition.x] = nullptr;
	}
	else
		// Attack move
	{
		PiecePosition targetSquare = move.TargetSquare;
		// Attack the piece on the target square
		// Move piece one space towards the target square
		// Move piece one space towards the target square
		int moveX = targetSquare.x - piecePosition.x;
		int moveY = targetSquare.y - piecePosition.y;
		int stepX = (moveX > 0) ? 1 : (moveX < 0) ? -1 : 0;
		int stepY = (moveY > 0) ? 1 : (moveY < 0) ? -1 : 0;
		PiecePosition newPosition = piecePosition + sf::Vector2i(stepX, stepY);
		board[newPosition.y][newPosition.x] = std::move(board[piecePosition.y][piecePosition.x]);

		board[piecePosition.y][piecePosition.x] = nullptr; // Clear old position

		// Remove the attacked piece from the board
		board[targetSquare.y][targetSquare.x] = nullptr;
	}
}



PieceType Archer::GetPieceType()
{
	return (m_Color == PieceColor::White) ? PieceType::White_Archer : PieceType::Black_Archer;
}