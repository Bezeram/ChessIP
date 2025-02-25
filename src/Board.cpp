#include "Board.h"

ChessAI::Board::Board()
{
	// First square starts from the bottom left, from white's perspective (a1)
	// White pieces
	for (int i = 0; i < m_WhitePawns.size(); i++)
		m_WhitePawns[i] = 8 + i;
	m_WhiteKnights[0] = 1;
	m_WhiteKnights[1] = 6;

	m_WhiteBishops[0] = 2;
	m_WhiteBishops[1] = 5;

	m_WhiteRooks[0] = 0;
	m_WhiteRooks[1] = 7;

	m_WhiteQueen = 3;
	m_WhiteKing = 4;

	// Black pieces
	int blackPawnRow = 6; // 0-7
	for (int i = 0; i < m_WhitePawns.size(); i++)
		m_BlackPawns[i] = 8 * blackPawnRow + i;
	m_BlackKnights[0] = 1 + 8 * (blackPawnRow + 1);
	m_BlackKnights[1] = 6 + 8 * (blackPawnRow + 1);

	m_BlackBishops[0] = 2 + 8 * (blackPawnRow + 1);
	m_BlackBishops[1] = 5 + 8 * (blackPawnRow + 1);

	m_BlackRooks[0] = 0 + 8 * (blackPawnRow + 1);
	m_BlackRooks[1] = 7 + 8 * (blackPawnRow + 1);

	m_BlackQueen = 3 + 8 * (blackPawnRow + 1);
	m_BlackKing = 4 + 8 * (blackPawnRow + 1);

	// Map positions to all pieces
	auto mapPositions = [&](PiecePos* data, int size, PieceType type)
		{
			for (int i = 0; i < size; i++)
			{
				PiecePos pos = data[i];
				PieceLocation loc = PieceLocation(data, i, type);
				m_PositionToPiece.insert(std::pair(pos, loc));
			}
		};

	mapPositions(m_WhitePawns.data(), m_WhitePawns.size(), PieceType::White_Pawn);
	mapPositions(m_WhiteKnights.data(), m_WhiteKnights.size(), PieceType::White_Knight);
	mapPositions(m_WhiteBishops.data(), m_WhiteBishops.size(), PieceType::White_Bishop);
	mapPositions(m_WhiteRooks.data(), m_WhiteRooks.size(), PieceType::White_Rook);
	mapPositions(&m_WhiteQueen, 0, PieceType::White_Queen);
	mapPositions(&m_WhiteKing, 0, PieceType::White_King);

	mapPositions(m_BlackPawns.data(), m_BlackPawns.size(), PieceType::Black_Pawn);
	mapPositions(m_BlackKnights.data(), m_BlackKnights.size(), PieceType::Black_Knight);
	mapPositions(m_BlackBishops.data(), m_BlackBishops.size(), PieceType::Black_Bishop);
	mapPositions(m_BlackRooks.data(), m_BlackRooks.size(), PieceType::Black_Rook);
	mapPositions(&m_BlackQueen, 0, PieceType::Black_Queen);
	mapPositions(&m_BlackKing, 0, PieceType::Black_King);
}

ChessAI::Board::Board(const char* FEN)
{
	// TODO:
}

void ChessAI::Board::GetBoard(std::vector<Piece>& outBoard) const
{
	// Put all the pieces in the output vector
	auto putPieces = [&outBoard](const PiecePos* const data, int size, PieceType pieceType)
		{
			for (int i = 0; i < size; i++)
			{
				PiecePos pos = data[i];

				outBoard.emplace_back(pos, pieceType);
			}
		};

	outBoard.reserve(16);
	putPieces(m_WhitePawns.data(), m_WhitePawns.size(), PieceType::White_Pawn);
	putPieces(m_WhiteKnights.data(), m_WhiteKnights.size(), PieceType::White_Knight);
	putPieces(m_WhiteBishops.data(), m_WhiteBishops.size(), PieceType::White_Bishop);
	putPieces(m_WhiteRooks.data(), m_WhiteRooks.size(), PieceType::White_Rook);
	putPieces(&m_WhiteQueen, 1, PieceType::White_Queen);
	putPieces(&m_WhiteKing, 1, PieceType::White_King);

	putPieces(m_BlackPawns.data(), m_BlackPawns.size(), PieceType::Black_Pawn);
	putPieces(m_BlackKnights.data(), m_BlackKnights.size(), PieceType::Black_Knight);
	putPieces(m_BlackBishops.data(), m_BlackBishops.size(), PieceType::Black_Bishop);
	putPieces(m_BlackRooks.data(), m_BlackRooks.size(), PieceType::Black_Rook);
	putPieces(&m_BlackQueen, 1, PieceType::Black_Queen);
	putPieces(&m_BlackKing, 1, PieceType::Black_King);
}

void ChessAI::Board::MakeMove(int startSquare, int targetSquare)
{
	if (m_PositionToPiece.find(startSquare) == m_PositionToPiece.end())
		return;

	// Modify piece position in container
	PieceLocation loc = m_PositionToPiece[startSquare];
	loc.Container[loc.Index] = targetSquare;

	// Modify mapper
	m_PositionToPiece.erase(startSquare);
	m_PositionToPiece.insert(std::pair(targetSquare, loc));
}

void ChessAI::Board::MakeMove(const Move& move)
{
	int startSquare = move.StartSquare;
	int targetSquare = move.TargetSquare;
	if (m_PositionToPiece.find(startSquare) == m_PositionToPiece.end())
		return;

	// Modify piece position in container
	PieceLocation loc = m_PositionToPiece[startSquare];
	loc.Container[loc.Index] = targetSquare;

	// Check side turn
	bool correctPiece = int(loc.Type) <= 5 && m_WhitesTurn || int(loc.Type) > 5 && !m_WhitesTurn;
	if (!correctPiece)
		return;

	m_WhitesTurn = !m_WhitesTurn;

	// Modify mapper
	m_PositionToPiece.erase(startSquare);
	m_PositionToPiece.insert(std::pair(targetSquare, loc));
}
