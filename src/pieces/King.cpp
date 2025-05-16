#include "King.h"

King::King(std::shared_ptr<Board> board, PieceColor color)
	: BasePiece(board, color)
{
}

void King::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	const BoardMatrix& boardMatrix = m_Board->GetBoard();

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
}

PieceType King::GetPieceType()
{
	return (m_Color == PieceColor::White) ? PieceType::White_King : PieceType::Black_King;
}