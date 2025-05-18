#include "Trebuchet.h"

Trebuchet::Trebuchet(std::shared_ptr<Board> board, PieceColor color, uint32_t upgradeLevel)
	: BasePiece(board, color, upgradeLevel) {}

void Trebuchet::GetRange(PiecePosition piecePosition, std::vector<ActionMove>& legalMoves)
{
	// Mișcare în + (sus/jos/stânga/dreapta), doar la distanță 1
	for (auto dir : std::vector<sf::Vector2i>{ {1, 0}, {-1, 0}, {0, 1}, {0, -1} })
	{
		PiecePosition target = piecePosition + dir;
		if (IsCellInBounds(target, m_Board->GetSize()))
		{
			legalMoves.push_back(ActionMove(target, MoveType::Move));
		}
	}

	// Atac pe offseturi predefinite
	AddAttackOffsets(legalMoves, piecePosition, m_UpgradeLevel);
}

void Trebuchet::GetLegalMoves(sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves)
{
	const BoardMatrix& boardMatrix = m_Board->GetBoard();

	// Mișcare simplă în cruce
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

	// Atac pe offseturi predefinite
	std::vector<sf::Vector2i> offsets;
	AddAttackOffsets(legalMoves, piecePosition, m_UpgradeLevel);
}

void Trebuchet::ExecuteMove(BoardMatrix& board, PiecePosition piecePosition, ActionMove move)
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
		if (targetPiece != nullptr)
		{
			if (!m_Board->IsTargetFriendly(PieceMove(piecePosition, move.TargetSquare)))
			{
				targetPiece.reset(); // Distruge piesa inamică
			}
		}
	}
}

PieceType Trebuchet::GetPieceType()
{
	return (m_Color == PieceColor::White) ? PieceType::White_Trebuchet : PieceType::Black_Trebuchet;
}

void Trebuchet::AddAttackOffsets(std::vector<ActionMove>& legalMoves, PiecePosition center, int level)
{
	std::vector<sf::Vector2i> offsets;

	if (level >= 1)
	{
		offsets = {
			{  2,  0 }, { -2,  0 }, {  0,  2 }, {  0, -2 },
			{  2,  1 }, { -2,  1 }, {  2, -1 }, { -2, -1 },
			{  1,  2 }, { -1,  2 }, {  1, -2 }, { -1, -2 }
		};
	}
	if (level >= 2)
	{
		offsets.insert(offsets.end(), {
			{  3,  0 }, { -3,  0 }, {  0,  3 }, {  0, -3 },
			{  3,  1 }, { -3,  1 }, {  3, -1 }, { -3, -1 },
			{  1,  3 }, { -1,  3 }, {  1, -3 }, { -1, -3 }
			});
	}
	if (level >= 3)
	{
		offsets.insert(offsets.end(), {
			{  4,  0 }, { -4,  0 }, {  0,  4 }, {  0, -4 },
			{  4,  1 }, { -4,  1 }, {  4, -1 }, { -4, -1 },
			{  1,  4 }, { -1,  4 }, {  1, -4 }, { -1, -4 }
			});
	}

	for (const auto& offset : offsets)
	{
		PiecePosition pos = center + offset;
		if (IsCellInBounds(pos, m_Board->GetSize()))
		{
			legalMoves.push_back(ActionMove(pos, MoveType::Action));
		}
	}
}
