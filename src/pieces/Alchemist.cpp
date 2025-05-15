#include "Alchemist.h"

Alchemist::Alchemist(std::shared_ptr<Board> board, PieceColor color, uint32_t upgradeLevel)
	: BasePiece(board, color, upgradeLevel)
{
}

void Alchemist::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	const BoardMatrix& boardMatrix = m_Board->GetBoard();

	switch (Alchemist::m_UpgradeLevel) 
	{
		case 1:
			for (int dy = -1; dy <= 1; dy++)
				for (int dx = -1; dx <= 1; dx++)
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
		default:
			for (int dy = -2; dy <= 2; dy++)
				for (int dx = -2; dx <= 2; dx++)
					if (abs(dx) + abs(dy) <= 2)
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

void Alchemist::ExecuteMove(BoardMatrix& board, PiecePosition piecePosition, ActionMove move)
{
	if (move.TargetSquare == piecePosition)
	{
		// Ability used

		std::vector <BasePiece*> piecesToCleanse;
		// Add all nearby pieces, including self, to a list
		switch (Alchemist::m_UpgradeLevel)
		{
			case 1:
				for (int dy = -1; dy <= 1; dy++)
					for (int dx = -1; dx <= 1; dx++)
						if (board[piecePosition.y + dy][piecePosition.x + dx].get()->GetPieceType() != PieceType::None)
						{
							piecesToCleanse.push_back(board[piecePosition.y + dy][piecePosition.x + dx].get());
						}
				break;
			default:
				for (int dy = -2; dy <= 2; dy++)
					for (int dx = -2; dx <= 2; dx++)
						if (board[piecePosition.y + dy][piecePosition.x + dx].get()->GetPieceType() != PieceType::None
							&& (abs(dx) + abs(dy) <= 2))
						{
							piecesToCleanse.push_back(board[piecePosition.y + dy][piecePosition.x + dx].get());
						}
				break;
		}

		// Cleanse the negative effects off all pieces on the list
		for (BasePiece* piece : piecesToCleanse)
		{
			// If any other piece uses the cleanse mechanic, it should be modularized into a function
			for (Effect effect : piece->GetEffects())
			{
				// If the effect is negative, remove it
				if (!IsEffectBuff(effect))
				{
					piece->RemoveEffect(effect);
				}
			}

			// Level 3 alchemist adds 1-round shield to the piece
			if (Alchemist::m_UpgradeLevel == 3)
			{
				piece->AddEffect(Effect::Alchemist_Shield);
			}
		}

	}
	else
	{
		// Normal move
		PiecePosition targetSquare = move.TargetSquare;
		// Move piece to the new square
		board[targetSquare.y][targetSquare.x] = std::move(board[piecePosition.y][piecePosition.x]);
		// Clear old position
		board[piecePosition.y][piecePosition.x] = nullptr;
	}
}

 PieceType Alchemist::GetPieceType()
{
	return (m_Color == PieceColor::White) ? PieceType::White_Alchemist : PieceType::Black_Alchemist;
}