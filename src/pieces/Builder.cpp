#include "Builder.h"

Builder::Builder(std::shared_ptr<Board> board, PieceColor color, uint32_t upgradeLevel)
	: BasePiece(board, color, upgradeLevel)
{
}

void Builder::GetRange(PiecePosition piecePosition, std::vector<ActionMove>& legalMoves)
{
	for (int dy = -2; dy <= 2; dy++)
		for (int dx = -2; dx <= 2; dx++)
			if ((dx == 0 || dy == 0) && (dx != 0 || dy != 0))
			{
				PiecePosition targetSquare = piecePosition + sf::Vector2i(dx, dy);
				if (IsCellInBounds(targetSquare, m_Board->GetSize()))
				{
					legalMoves.push_back(ActionMove(targetSquare, MoveType::Move));
				}
			}

	std::vector<PiecePosition> targetSquareVector;


	// Junk so compiler doesn't scream at me
	PiecePosition targetSquare1;
	PiecePosition targetSquare2;
	PiecePosition targetSquare3;
	PiecePosition targetSquareMiddle;

	switch (Builder::m_UpgradeLevel)
	{
	case 1:
		// black builder should be able to place walls only downwards, not upwards
		if (Builder::GetPieceColor() == PieceColor::Black)
		{
			targetSquareVector = {
				piecePosition + sf::Vector2i(-1, -1),
				piecePosition + sf::Vector2i(0, -1),
				piecePosition + sf::Vector2i(1, -1)
			};
		}
		else
		{
			targetSquareVector = {
				piecePosition + sf::Vector2i(-1, 1),
				piecePosition + sf::Vector2i(0, 1),
				piecePosition + sf::Vector2i(1, 1)
			};
		}
		for (PiecePosition target : targetSquareVector)
		{
			if (IsCellInBounds(target, m_Board->GetSize()))
			{
				legalMoves.push_back(ActionMove(target, MoveType::Move));
				legalMoves.push_back(ActionMove(target, MoveType::Action));
			}
		}


		break;
	case 2:

		if (Builder::GetPieceColor() == PieceColor::Black)
		{
			targetSquare1 = piecePosition + sf::Vector2i(-1, -1);
			targetSquare2 = piecePosition + sf::Vector2i(1, -1);
			targetSquareMiddle = piecePosition + sf::Vector2i(0, -1);
		}
		else
		{
			targetSquare1 = piecePosition + sf::Vector2i(-1, 1);
			targetSquare2 = piecePosition + sf::Vector2i(1, 1);
			targetSquareMiddle = piecePosition + sf::Vector2i(0, 1);
		}


		if (IsCellInBounds(targetSquare1, m_Board->GetSize()))
		{
			legalMoves.push_back(ActionMove(targetSquare1, MoveType::Move));
			legalMoves.push_back(ActionMove(targetSquare1, MoveType::Action));
		}
		if (IsCellInBounds(targetSquare2, m_Board->GetSize()))
		{
			legalMoves.push_back(ActionMove(targetSquare2, MoveType::Move));
			legalMoves.push_back(ActionMove(targetSquare2, MoveType::Action));
		}
		break;
	case 3:

		if (Builder::GetPieceColor() == PieceColor::Black)
		{
			targetSquare1 = piecePosition + sf::Vector2i(-1, -1);
			targetSquare2 = piecePosition + sf::Vector2i(0, -1);
			targetSquare3 = piecePosition + sf::Vector2i(1, -1);
		}
		else
		{
			targetSquare1 = piecePosition + sf::Vector2i(-1, 1);
			targetSquare2 = piecePosition + sf::Vector2i(0, 1);
			targetSquare3 = piecePosition + sf::Vector2i(1, 1);
		}

		if (IsCellInBounds(targetSquare1 + sf::Vector2i(-1, 0), m_Board->GetSize()))
		{
			legalMoves.push_back(ActionMove(targetSquare1, MoveType::Move));
			legalMoves.push_back(ActionMove(targetSquare1, MoveType::Action));
		}

		if (IsCellInBounds(targetSquare2 + sf::Vector2i(-1, 0), m_Board->GetSize())
			&& IsCellInBounds(targetSquare2 + sf::Vector2i(1, 0), m_Board->GetSize()))
		{
			legalMoves.push_back(ActionMove(targetSquare2, MoveType::Move));
			legalMoves.push_back(ActionMove(targetSquare2, MoveType::Action));
		}

		if (IsCellInBounds(targetSquare3 + sf::Vector2i(1, 0), m_Board->GetSize()))
		{
			legalMoves.push_back(ActionMove(targetSquare3, MoveType::Move));
			legalMoves.push_back(ActionMove(targetSquare3, MoveType::Action));
		}

	}
}

void Builder::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	const BoardMatrix& boardMatrix = m_Board->GetBoard();

	// Move commands
	for (int dy = -2; dy<= 2; dy++)
		for (int dx = -2; dx <= 2; dx++)
			if ((dx == 0 || dy == 0) && (dx != 0 || dy != 0))
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
							legalMoves.push_back(ActionMove(targetSquare, MoveType::Move));
						}
					}
				}
			}

	std::vector<PiecePosition> targetSquareVector;


	// Junk so compiler doesn't scream at me
	PiecePosition targetSquare1;
	PiecePosition targetSquare2;
	PiecePosition targetSquare3;
	PiecePosition targetSquareMiddle;

	switch (Builder::m_UpgradeLevel)
	{
	case 1:
		// black builder should be able to place walls only downwards, not upwards
		if (Builder::GetPieceColor() == PieceColor::Black)
		{
			targetSquareVector = { 
				piecePosition + sf::Vector2i(-1, -1),
				piecePosition + sf::Vector2i(0, -1),
				piecePosition + sf::Vector2i(1, -1) 
			};
		}
		else
		{
			targetSquareVector = {
				piecePosition + sf::Vector2i(-1, 1),
				piecePosition + sf::Vector2i(0, 1),
				piecePosition + sf::Vector2i(1, 1)
			};
		}
		for (PiecePosition target : targetSquareVector)
		{
			if (IsCellInBounds(target, m_Board->GetSize()))
			{
				const auto& targetPiece = boardMatrix[target.y][target.x];
				if (targetPiece.get() == nullptr)
				{
					legalMoves.push_back(ActionMove(target, MoveType::Action));
				}
			}
		}


		break;
	case 2:

		if (Builder::GetPieceColor() == PieceColor::Black)
		{
			targetSquare1 = piecePosition + sf::Vector2i(-1, -1);
			targetSquare2 = piecePosition + sf::Vector2i(1, -1);
			targetSquareMiddle = piecePosition + sf::Vector2i(0, -1);
		}
		else
		{
			targetSquare1 = piecePosition + sf::Vector2i(-1, 1);
			targetSquare2 = piecePosition + sf::Vector2i(1, 1);
			targetSquareMiddle = piecePosition + sf::Vector2i(0, 1);
		}


		if (IsCellInBounds(targetSquare1, m_Board->GetSize()))
		{
			const auto& targetPiece = boardMatrix[targetSquare1.y][targetSquare1.x];
			if (targetPiece.get() == nullptr && boardMatrix[targetSquareMiddle.y][targetSquareMiddle.x].get() == nullptr)
			{
				legalMoves.push_back(ActionMove(targetSquare1, MoveType::Action));
			}
		}
		if (IsCellInBounds(targetSquare2, m_Board->GetSize()))
		{
			const auto& targetPiece = boardMatrix[targetSquare2.y][targetSquare2.x];
			if (targetPiece.get() == nullptr && boardMatrix[targetSquareMiddle.y][targetSquareMiddle.x].get() == nullptr)
			{
				legalMoves.push_back(ActionMove(targetSquare1, MoveType::Action));
			}
		}
		break;
	case 3:

		if (Builder::GetPieceColor() == PieceColor::Black)
		{
			targetSquare1 = piecePosition + sf::Vector2i(-1, -1);
			targetSquare2 = piecePosition + sf::Vector2i(0, -1);
			targetSquare3 = piecePosition + sf::Vector2i(1, -1);
		}
		else
		{
			 targetSquare1 = piecePosition + sf::Vector2i(-1, 1);
			 targetSquare2 = piecePosition + sf::Vector2i(0, 1);
			 targetSquare3 = piecePosition + sf::Vector2i(1, 1);
		}

		if (IsCellInBounds(targetSquare1 + sf::Vector2i(-1, 0), m_Board->GetSize()))
		{
			const auto& targetPieceLeft = boardMatrix[targetSquare1.y][targetSquare1.x - 1];
			const auto& targetPieceMiddle = boardMatrix[targetSquare1.y][targetSquare1.x];
			const auto& targetPieceRight = boardMatrix[targetSquare1.y][targetSquare1.x + 1];
			if (targetPieceLeft.get() == nullptr && targetPieceMiddle.get() == nullptr && targetPieceRight.get() == nullptr)
			{
				legalMoves.push_back(ActionMove(targetSquare1, MoveType::Action));
			}
		}

		if (IsCellInBounds(targetSquare2 + sf::Vector2i(-1, 0), m_Board->GetSize())
			&& IsCellInBounds(targetSquare2 + sf::Vector2i(1, 0), m_Board->GetSize()))
		{
			const auto& targetPieceLeft = boardMatrix[targetSquare2.y][targetSquare2.x - 1];
			const auto& targetPieceMiddle = boardMatrix[targetSquare2.y][targetSquare2.x];
			const auto& targetPieceRight = boardMatrix[targetSquare2.y][targetSquare2.x + 1];
			if (targetPieceLeft.get() == nullptr && targetPieceMiddle.get() == nullptr && targetPieceRight.get() == nullptr)
			{
				legalMoves.push_back(ActionMove(targetSquare2, MoveType::Action));
			}
		}

		if (IsCellInBounds(targetSquare3 + sf::Vector2i(1, 0), m_Board->GetSize()))
		{
			const auto& targetPieceLeft = boardMatrix[targetSquare3.y][targetSquare3.x - 1];
			const auto& targetPieceMiddle = boardMatrix[targetSquare3.y][targetSquare3.x];
			const auto& targetPieceRight = boardMatrix[targetSquare3.y][targetSquare3.x + 1];
			if (targetPieceLeft.get() == nullptr && targetPieceMiddle.get() == nullptr && targetPieceRight.get() == nullptr)
			{
				legalMoves.push_back(ActionMove(targetSquare3, MoveType::Action));
			}
		}

	}
}

void Builder::ExecuteMove(BoardMatrix& board, PiecePosition piecePosition, ActionMove move)
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
		PiecePosition targetSquare;
		PiecePosition targetSquare1;
		PiecePosition targetSquare2;
		PiecePosition targetSquare3;
		PiecePosition targetSquare4;
		PiecePosition targetSquare5;
		// Ability used
		switch (Builder::m_UpgradeLevel) 
		{
			case 1:
				// one wall
				targetSquare = move.TargetSquare;
				board[targetSquare.y][targetSquare.x] = std::make_unique<Wall>(m_Board, Builder::GetPieceColor(), 1);
				break;

			case 2:
				// two walls
				targetSquare1 = move.TargetSquare;
				if (Builder::GetPieceColor() == PieceColor::Black)
				{
					targetSquare2 = piecePosition + sf::Vector2i(0, -1);
				}
				else
				{
					targetSquare2 = piecePosition + sf::Vector2i(0, 1);
				}
				board[targetSquare1.y][targetSquare1.x] = std::make_unique<Wall>(m_Board, Builder::GetPieceColor(), 1);
				board[targetSquare2.y][targetSquare2.x] = std::make_unique<Wall>(m_Board, Builder::GetPieceColor(), 1);
				break;
			case 3:
				// three walls
				targetSquare3 = move.TargetSquare;
				targetSquare4 = targetSquare3 + sf::Vector2i(1, 0);
				targetSquare5 = targetSquare3 + sf::Vector2i(-1, 0);
				board[targetSquare3.y][targetSquare3.x] = std::make_unique<Wall>(m_Board, Builder::GetPieceColor(), 1);
				board[targetSquare4.y][targetSquare4.x] = std::make_unique<Wall>(m_Board, Builder::GetPieceColor(), 1);
				board[targetSquare5.y][targetSquare5.x] = std::make_unique<Wall>(m_Board, Builder::GetPieceColor(), 1);
				break;
		}
	}
}

PieceType Builder::GetPieceType()
{
	return (m_Color == PieceColor::White) ? PieceType::White_Builder : PieceType::Black_Builder;
}