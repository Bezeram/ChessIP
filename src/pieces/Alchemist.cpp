#include "Alchemist.h"

Alchemist::Alchemist(std::shared_ptr<Board> board, PieceColor color, uint32_t upgradeLevel)
	: BasePiece(board, color, upgradeLevel)
{
}

void Alchemist::GetRange(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	legalMoves.push_back(ActionMove(piecePosition, MoveType::Action));

	int range = (m_UpgradeLevel == 1) ? 1 : 2;
	for (int dy = -range; dy <= range; dy++)
		for (int dx = -range; dx <= range; dx++)
		{
			PiecePosition targetSquare = piecePosition + sf::Vector2i(dx, dy);
			if (IsCellInBounds(targetSquare, m_Board->GetSize()))
			{
				legalMoves.push_back(ActionMove(targetSquare, MoveType::Move));
			}
		}
}

void Alchemist::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	const BoardMatrix& boardMatrix = m_Board->GetBoard();
	int range = (m_UpgradeLevel == 1) ? 1 : 2;

	legalMoves.push_back(ActionMove(piecePosition, MoveType::Action));

	for (int dy = -range; dy <= range; dy++)
		for (int dx = -range; dx <= range; dx++)
		{
			PiecePosition targetSquare = piecePosition + sf::Vector2i(dx, dy);
			if (IsCellInBounds(targetSquare, m_Board->GetSize()))
			{
				const auto& targetPiece = boardMatrix[targetSquare.y][targetSquare.x];
				if (targetPiece.get() == nullptr || !m_Board->IsTargetFriendly(PieceMove(piecePosition, targetSquare)))
				{
					// Check if the target square is not occupied by a friendly piece
					legalMoves.push_back(ActionMove(targetSquare, MoveType::Move));
				}
			}
		}
}

void Alchemist::ExecuteMove(BoardMatrix& board, PiecePosition piecePosition, ActionMove move)
{
	// Move action
	if (move.MoveType == MoveType::Move)
	{
		PiecePosition targetSquare = move.TargetSquare;
		// Move piece to the new square
		board[targetSquare.y][targetSquare.x] = std::move(board[piecePosition.y][piecePosition.x]);
		// Clear old position
		board[piecePosition.y][piecePosition.x] = nullptr;
	}
	else
	{
		// Ability used
		std::vector <BasePiece*> piecesToCleanse;
		// Add all nearby pieces, including self, to a list
		int range = (m_UpgradeLevel == 1) ? 1 : 2;

		for (int dy = -range; dy <= range; dy++)
		{
			for (int dx = -range; dx <= range; dx++)
			{
				PiecePosition targetPosition = { piecePosition.x + dx, piecePosition.y + dy };
				if (IsCellInBounds(targetPosition, m_Board->GetSize()))
				{
					BasePiece* piece = board[targetPosition.y][targetPosition.x].get();
					if (piece != nullptr && piece->GetPieceType() != PieceType::None)
					{
						piecesToCleanse.push_back(piece);
					}
				}
			}
		}

		// Cleanse the negative effects off all pieces on the list
		for (BasePiece* piece : piecesToCleanse)
		{
			// If any other piece uses the cleanse mechanic, it should be modularized into a function
			for (const auto& effect : piece->GetEffects())
			{
				// If the effect is negative, remove it
				if (!IsEffectBuff(std::get<0>(effect)))
				{
					piece->RemoveEffect(std::get<0>(effect));
				}
			}

			// Level 3 alchemist adds 1-round shield to the piece
			if (Alchemist::m_UpgradeLevel == 3)
			{
				piece->AddEffect(Effect::Alchemist_Shield, 1);
			}
		}
	}
}

 PieceType Alchemist::GetPieceType()
{
	return (m_Color == PieceColor::White) ? PieceType::White_Alchemist : PieceType::Black_Alchemist;
}