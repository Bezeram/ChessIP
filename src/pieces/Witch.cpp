#include "Witch.h"

Witch::Witch(std::shared_ptr<Board> board, PieceColor color, uint32_t upgradeLevel)
	: BasePiece(board, color, upgradeLevel)
{
}

void Witch::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	const BoardMatrix& boardMatrix = m_Board->GetBoard();

	switch (Witch::m_UpgradeLevel)
	{
	case 1:
		for (int dy = -2; dy <= 2; dy++)
			for (int dx = -2; dx <= 2; dx++)
				if ((dy != 0 || dx != 0) && (abs(dx) + abs(dy) <= 2))
				{
					PiecePosition targetSquare = piecePosition + sf::Vector2i(dx, dy);
					if (IsCellInBounds(targetSquare, m_Board->GetSize()))
					{
						const auto& targetPiece = boardMatrix[targetSquare.y][targetSquare.x];
						if (targetPiece.get() == nullptr)
						{
							legalMoves.push_back(ActionMove(targetSquare, MoveType::Move));
						}
						else
						{
							if (!m_Board->IsTargetFriendly(PieceMove(piecePosition, targetSquare)))
							{
								legalMoves.push_back(ActionMove(targetSquare, MoveType::Action));
							}
						}
					}
				}
		break;
	default:
		for (int dy = -3; dy <= 3; dy++)
			for (int dx = -3; dx <= 3; dx++)
				if ((dy != 0 || dx != 0) && (abs(dx) + abs(dy) <= 3))
				{
					PiecePosition targetSquare = piecePosition + sf::Vector2i(dx, dy);
					if (IsCellInBounds(targetSquare, m_Board->GetSize()))
					{
						// todo: Check if the target square is empty or occupied by a friendly piece
						const auto& targetPiece = boardMatrix[targetSquare.y][targetSquare.x];
						if (targetPiece.get() == nullptr)
						{
							legalMoves.push_back(ActionMove(targetSquare, MoveType::Move));
						}
						else
						{
							if (!m_Board->IsTargetFriendly(PieceMove(piecePosition, targetSquare)))
							{
								legalMoves.push_back(ActionMove(targetSquare, MoveType::Action));
							}
						}
					}
				}
		break;
	}
}

void Witch::ExecuteMove(BoardMatrix& board, PiecePosition piecePosition, ActionMove move)
{
	// Normal move
	if (move.MoveType == MoveType::Move)
	{
		PiecePosition targetSquare = move.TargetSquare;
		// Move piece to the new square
		board[targetSquare.y][targetSquare.x] = std::move(board[piecePosition.y][piecePosition.x]);
		// Clear old position
		board[piecePosition.y][piecePosition.x] = nullptr;
	}
	else
	// Action move
	{
		std::cout << "ACTION MOVE!" << std::endl;
		if (board[move.TargetSquare.y][move.TargetSquare.x] != nullptr)
		{
			PiecePosition targetSquare = move.TargetSquare;
			// Stun enemy in the square
			board[targetSquare.y][targetSquare.x]->AddEffect(Effect::Stun, 1);
			std::cout << "STUNNED!" << std::endl;

			if (Witch::m_UpgradeLevel == 3)
			{
				// For level 3, hex enemies with same x value
				for (int dx = 0; dx < m_Board->GetSize(); dx += 1)
				{
					PiecePosition hexSquare = sf::Vector2i(dx, targetSquare.y);
					if (IsCellInBounds(hexSquare, m_Board->GetSize()))
					{
						const auto& targetPiece = board[hexSquare.y][hexSquare.x];
						if (!m_Board->IsTargetFriendly(PieceMove(piecePosition, hexSquare))
							&& targetPiece != nullptr)
						{
							targetPiece->AddEffect(Effect::Hex, 1);
						}
					}
				}
			}
			else
			{
				// For level 1 and 2, hex enemies in two squares to left and to right
				for (int dx = -2; dx <= 2; dx += 1)
				{
					PiecePosition hexSquare = targetSquare + sf::Vector2i(dx, 0);
					if (IsCellInBounds(hexSquare, m_Board->GetSize()))
					{
						const auto& targetPiece = board[hexSquare.y][hexSquare.x];
						if (!m_Board->IsTargetFriendly(PieceMove(piecePosition, hexSquare))
							&& targetPiece != nullptr)
						{
							targetPiece->AddEffect(Effect::Hex, 1);
						}
					}
				}
			}

		}
	}
	
}

PieceType Witch::GetPieceType()
{
	return (m_Color == PieceColor::White) ? PieceType::White_Witch : PieceType::Black_Witch;
}