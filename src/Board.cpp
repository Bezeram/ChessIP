#include "Board.h"

using namespace ChessIP;

Board::Board(GameType gameType)
{
	int boardSize;
	switch (gameType)
	{
	case GameType::OneVOne:
		boardSize = 9;
		break;
	case GameType::TwoVTwo:
		boardSize = 10;
		break;
	}

	// First square starts from the top left, from white's perspective (a8)
	// Pieces default positions
	int King = boardSize / 2;

	{
		// Place pieces
		int blackKingRow = 0;
		m_BlackKing = King + 8 * (blackKingRow - 1);
	}

	{
		// White pieces
		int whiteKingRow = boardSize - 1;
		m_WhiteKing = King + 8 * (whiteKingRow + 1);
	}

	// Map positions to all pieces
	auto mapPositions = [&](std::vector<PiecePos>* data, int size, PieceType type)
		{
			for (int i = 0; i < size; i++)
			{
				// Read as vector
				PiecePos pos = (*data)[i];
				PieceLocation loc = PieceLocation(data, i, type);
				m_PositionToPiece.emplace(pos, loc);
			}
		};

	// Kings are exceptions because there cannot be more than one of them
	{
		mapPositions(&m_WhitePawns, m_WhitePawns.size(), PieceType::White_Pawn);
		mapPositions(&m_WhiteKnights, m_WhiteKnights.size(), PieceType::White_Knight);
		mapPositions(&m_WhiteBishops, m_WhiteBishops.size(), PieceType::White_Bishop);
		mapPositions(&m_WhiteRooks, m_WhiteRooks.size(), PieceType::White_Rook);
		mapPositions(&m_WhiteQueen, m_WhiteQueen.size(), PieceType::White_Queen);
		// White King
		PiecePos pos = m_WhiteKing;
		PieceLocation loc = PieceLocation::WhiteKing(&m_WhiteKing);
		m_PositionToPiece.emplace(pos, loc);
	}

	{
		mapPositions(&m_BlackPawns, m_BlackPawns.size(), PieceType::Black_Pawn);
		mapPositions(&m_BlackKnights, m_BlackKnights.size(), PieceType::Black_Knight);
		mapPositions(&m_BlackBishops, m_BlackBishops.size(), PieceType::Black_Bishop);
		mapPositions(&m_BlackRooks, m_BlackRooks.size(), PieceType::Black_Rook);
		mapPositions(&m_BlackQueen, m_WhiteQueen.size(), PieceType::Black_Queen);
		// Black King
		PiecePos pos = m_BlackKing;
		PieceLocation loc = PieceLocation::BlackKing(&m_BlackKing);
		m_PositionToPiece.emplace(pos, loc);
	}
}

Board::Board(GameType gameType, const char* FEN)
{
	// TODO:
	// Load board position via FEN string
}

void Board::GetBoard(std::vector<Piece>& outBoard) const
{
	// Put all the pieces in the output vector
	auto putPieces = [&outBoard](const std::vector<PiecePos>& const data, int size, PieceType pieceType)
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
	assert(move.StartSquare >= 0 && move.StartSquare <= 63 && move.TargetSquare >= 0 && move.TargetSquare <= 63);

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
	assert(position >= 0 && position <= 63);

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
	}
	// Place piece at target position in container
	selected.SetPosition(targetSquare);

	// Update mapper (erase start position, add the new target position)
	m_PositionToPiece.erase(startSquare);
	m_PositionToPiece.emplace(targetSquare, selected);

	m_IsWhitesTurn = !m_IsWhitesTurn;
	return true;
}
