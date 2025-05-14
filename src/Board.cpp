#include "Board.h"

using namespace ChessIP;

Board::Board(GameType gameType)
{
	// Default for OneVOne
	m_Size = 9;
	switch (gameType)
	{
	case GameType::OneVOne:
		// boardSize = 9
		Init1v1Game();
		break;
	case GameType::TwoVTwo:
		m_Size = 10;
		// TODO:
		// 2v2 init
		break;
	}
}

void Board::MapPositions(std::vector<PiecePos>* data, int size, PieceType type)
{
	for (int i = 0; i < size; i++)
	{
		// Read as vector
		PiecePos pos = (*data)[i];
		PieceLocation loc = PieceLocation(data, i, type);
		m_PositionToPiece.emplace(pos, loc);
	}
}

void Board::Init1v1Game()
{
	// Pieces default positions
	int King = m_Size / 2;

	// Place pieces
	// Black
	{
		int blackKingRow = 0;
		m_BlackKing = King + m_Size * blackKingRow;
	}

	// White
	{
		int whiteKingRow = m_Size - 1;
		m_WhiteKing = King + m_Size * whiteKingRow;
	}

	// White King
	{
		PiecePos pos = m_WhiteKing;
		PieceLocation loc = PieceLocation::WhiteKing(&m_WhiteKing);
		m_PositionToPiece.emplace(pos, loc);
	}

	// Black King
	{
		PiecePos pos = m_BlackKing;
		PieceLocation loc = PieceLocation::BlackKing(&m_BlackKing);
		m_PositionToPiece.emplace(pos, loc);
	}
}

void Board::GetBoard(std::vector<Piece>& outBoard) const
{
	// Put all the pieces in the output vector
	auto putPieces = [&outBoard](const std::vector<PiecePos>& data, int size, PieceType pieceType)
		{
			for (int i = 0; i < size; i++)
			{
				PiecePos pos = data[i];

				outBoard.emplace_back(pos, pieceType);
			}
		};

	outBoard.reserve(16);
	putPieces(m_WhitePawns, m_WhitePawns.size(), PieceType::White_Pawn);
	putPieces(m_WhiteKnights, m_WhiteKnights.size(), PieceType::White_Knight);
	putPieces(m_WhiteBishops, m_WhiteBishops.size(), PieceType::White_Bishop);
	putPieces(m_WhiteRooks, m_WhiteRooks.size(), PieceType::White_Rook);
	putPieces(m_WhiteQueen, m_WhiteQueen.size(), PieceType::White_Queen);
	outBoard.emplace_back(m_WhiteKing, PieceType::White_King);

	putPieces(m_BlackPawns, m_BlackPawns.size(), PieceType::Black_Pawn);
	putPieces(m_BlackKnights, m_BlackKnights.size(), PieceType::Black_Knight);
	putPieces(m_BlackBishops, m_BlackBishops.size(), PieceType::Black_Bishop);
	putPieces(m_BlackRooks, m_BlackRooks.size(), PieceType::Black_Rook);
	putPieces(m_BlackQueen, m_BlackQueen.size(), PieceType::Black_Queen);
	outBoard.emplace_back(m_BlackKing, PieceType::Black_King);
}

int Board::GetSize() const
{
	return m_Size;
}

PieceLocation Board::GetPieceAt(PiecePos position) const
{
	auto it = m_PositionToPiece.find(position);
	if (it == m_PositionToPiece.end())
		return PieceLocation::EmptySquare();
	return it->second;
}

void Board::DeletePieceAt(PiecePos position)
{
	auto it = m_PositionToPiece.find(position);
	if (it == m_PositionToPiece.end())
		return;
	PieceLocation pieceLoc = m_PositionToPiece.at(position);
	// Erase the container it is pointing to and modify the others affected
	// using the mapper
	pieceLoc.Delete(m_PositionToPiece);
	// Erase it from the location lookup
	m_PositionToPiece.erase(position);
}

bool Board::IsTargetFriendly(const Move& move) const
{
	// Board bounds
	assert(move.StartSquare >= 0 && move.StartSquare <= m_Size * m_Size - 1 && move.TargetSquare >= 0 && move.TargetSquare <= m_Size * m_Size - 1);

	// Check if the target piece is 
	PieceLocation selected = GetPieceAt(move.StartSquare);
	PieceLocation target = GetPieceAt(move.TargetSquare);
	if (!target.IsEmptySquare())
		return GetPieceColor(selected.GetType()) == GetPieceColor(target.GetType());
	return false;
}

bool Board::IsValidPieceByTurn(PiecePos position) const
{
	// Board bounds
	assert(position >= 0 && position <= m_Size * m_Size - 1);

	PieceLocation pieceLoc = GetPieceAt(position);
	if (pieceLoc.IsEmptySquare())
		return false;

	return IsWhitePiece(pieceLoc.GetType()) == IsWhitesMove();
}

bool Board::IsWhitesMove() const
{
	return m_IsWhitesTurn;
}

bool Board::MakeMove(const Move& move)
{
	int startSquare = move.StartSquare;
	int targetSquare = move.TargetSquare;


	// Check for empty square and if target is friendly
	PieceLocation selected = m_PositionToPiece.at(startSquare);
	if (selected.IsEmptySquare() || IsTargetFriendly(move))
		return false;

	PieceLocation target = GetPieceAt(targetSquare);
	// Check for capture
	if (!target.IsEmptySquare())
	{
		// Delete the captured piece
		DeletePieceAt(targetSquare);

		SoundPlayer::GetInstance().PlaySound("capture1");
	}
	else
	{
		SoundPlayer::GetInstance().PlaySound("move1");
	}
	// Place piece at target position in container
	selected.SetPosition(targetSquare);

	// Update mapper (erase start position, add the new target position)
	m_PositionToPiece.erase(startSquare);
	m_PositionToPiece.emplace(targetSquare, selected);

	// Update flux and gold
	if (m_IsWhitesTurn)
	{
		m_WhiteFlux++;
		if (m_WhiteFlux > 9)
			m_WhiteFlux = 9;
		m_WhiteGold += m_Size - m_WhiteKing / m_Size;
		if (m_WhiteGold > 9)
			m_WhiteGold = 9;
		std::cout << m_WhiteKing / m_Size << std::endl;
	}
	else
	{
		m_BlackFlux++;
		m_BlackGold += m_BlackKing / m_Size;
	}

	m_IsWhitesTurn = !m_IsWhitesTurn;

	return true;
}


