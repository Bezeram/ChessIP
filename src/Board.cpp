#include "Board.h"

Board::Board(GameType gameType)
{
	m_Board.resize(10);

	// For each row, resize the inner vector to 10 columns and initialize the unique_ptr<int>
	for (int i = 0; i < 10; ++i) {
		m_Board[i].resize(10); // Resize each row to 10 columns
		for (int j = 0; j < 10; ++j) {
			m_Board[i][j] = nullptr; // Initialize each element
		}
	}

	// Default for OneVOne
	switch (gameType)
	{
	case GameType::OneVOne:
		// boardSize = 9
		m_Size = 9;
		break;
	case GameType::TwoVTwo:
		m_Size = 10;
		// TODO:
		// 2v2 init
		break;
	}
}

Board::~Board()
{
}

void Board::Init1v1Game(std::shared_ptr<Board>& boardRef)
{
	int kingFile = m_Size / 2;
	int whiteRank = 0;
	int blackRank = m_Size - 1;

	// Pieces default positions
	// Place pieces
	// White
	{
		m_Board[whiteRank][kingFile] = std::make_unique<King>(boardRef, PieceColor::White);
	}
	{
		m_Board[whiteRank][kingFile + 1] = std::make_unique<Archer>(boardRef, PieceColor::White, 1);
	}
	{
		m_Board[whiteRank][kingFile + 2] = std::make_unique<Alchemist>(boardRef, PieceColor::White, 3);
	}
	{
		m_Board[whiteRank][kingFile + 3] = std::make_unique<Witch>(boardRef, PieceColor::White, 3);
	}
	{
		m_Board[whiteRank][kingFile - 1] = std::make_unique<Trebuchet>(boardRef, PieceColor::White, 3);
	}
	{
		m_Board[whiteRank][kingFile - 2] = std::make_unique<Rogue>(boardRef, PieceColor::White, 3);
	}
	{
		m_Board[whiteRank][kingFile - 3] = std::make_unique<GrimReaper>(boardRef, PieceColor::White, 3);
	}
	{
		m_Board[whiteRank][kingFile - 4] = std::make_unique<PlagueDoctor>(boardRef, PieceColor::White, 3);
	}

	// Black
	{
		m_Board[blackRank][kingFile] = std::make_unique<King>(boardRef, PieceColor::Black);
	}
	{
		m_Board[blackRank][kingFile + 1] = std::make_unique<Archer>(boardRef, PieceColor::Black, 3);
	}
	{
		m_Board[blackRank][kingFile + 2] = std::make_unique<Alchemist>(boardRef, PieceColor::Black, 3);
	}
	{
		m_Board[blackRank][kingFile + 3] = std::make_unique<Witch>(boardRef, PieceColor::Black, 3);
	}
	{
		m_Board[blackRank][kingFile - 1] = std::make_unique<Trebuchet>(boardRef, PieceColor::Black, 3);
	}
	{
		m_Board[blackRank][kingFile - 2] = std::make_unique<Rogue>(boardRef, PieceColor::Black, 2);
	}
	{
		m_Board[blackRank][kingFile - 3] = std::make_unique<GrimReaper>(boardRef, PieceColor::Black, 3);
	}
	{
		m_Board[blackRank][kingFile - 4] = std::make_unique<PlagueDoctor>(boardRef, PieceColor::Black, 3);
	}
}

const BoardMatrix& Board::GetBoard() const
{
	return m_Board;
}

int Board::GetSize() const
{
	return m_Size;
}

void Board::DeletePieceAt(PiecePosition position)
{
	assert(position.x >= 0 && position.x < m_Size && position.y >= 0 && position.y < m_Size);
	m_Board[position.y][position.x].reset();
}

bool Board::IsTargetFriendly(const PieceMove& move) const
{
	// Board bounds
	assert(move.StartSquare.x >= 0 && move.StartSquare.x < m_Size &&
		move.StartSquare.y >= 0 && move.StartSquare.y < m_Size &&
		move.TargetSquare.x >= 0 && move.TargetSquare.x < m_Size &&
		move.TargetSquare.y >= 0 && move.TargetSquare.y < m_Size);

	// Check if the target piece is 
	const auto& selectedPiece = (*this)[move.StartSquare];
	const auto& targetPiece = (*this)[move.TargetSquare];
	if (targetPiece.get() != nullptr)
		return GetPieceColor(selectedPiece->GetPieceType()) == GetPieceColor(targetPiece->GetPieceType());
	return false;
}

std::unique_ptr<BasePiece>& Board::operator[](PiecePosition position)
{
	assert(position.x >= 0 && position.x < m_Size && position.y >= 0 && position.y < m_Size);
	return m_Board[position.y][position.x];
}

const std::unique_ptr<BasePiece>& Board::operator[](PiecePosition position) const
{
	assert(position.x >= 0 && position.x < m_Size && position.y >= 0 && position.y < m_Size);
	return m_Board[position.y][position.x];
}

bool Board::IsValidPieceByTurn(PiecePosition position) const
{
	// Board bounds
	assert(position.x >= 0 && position.x < m_Size &&
		position.y >= 0 && position.y < m_Size);

	const auto& piece = (*this)[position];
	if (piece.get() == nullptr)
		return false;

	return IsWhitePiece(piece->GetPieceType()) == IsWhitesMove();
}

bool Board::IsWhitesMove() const
{
	return m_IsWhitesTurn;
}

bool Board::MakeMove(PiecePosition piecePosition, ActionMove actionMove)
{
	const auto& selectedPiece = (*this)[piecePosition];
	selectedPiece->ExecuteMove(m_Board, piecePosition, actionMove);

	// Update flux and gold
	UpdateResources();
	// Update board (effects)
	Update();

	// Șterge piesele afectate de Curse
	for (int y = 0; y < m_Size; ++y) {
		for (int x = 0; x < m_Size; ++x) {
			auto& piece = m_Board[y][x];
			if (piece && piece->IsMarkedForDeletion()) {
				piece.reset(); // elimină piesa
			}
		}
	}

	// Flip turn
	m_IsWhitesTurn = !m_IsWhitesTurn;
	return true;
}

void Board::Update()
{
	// Update effects
	for (int rank = 0; rank < m_Size; rank++)
		for (int file = 0; file < m_Size; file++)
		{
			if (m_Board[rank][file] != nullptr)
			{
				m_Board[rank][file]->UpdateEffects();
			}
		}
}

PiecePosition Board::GetWhiteKingPosition() const
{
	for (int rank = 0; rank < m_Size; rank++)
	{
		for (int file = 0; file < m_Size; file++)
		{
			if (m_Board[rank][file].get() != nullptr && m_Board[rank][file]->GetPieceType() == PieceType::White_King)
			{
				return PiecePosition(file, rank);
			}
		}
	}

	return Constants::NullPosition;
}

PiecePosition Board::GetBlackKingPosition() const
{
	for (int rank = 0; rank < m_Size; rank++)
	{
		for (int file = 0; file < m_Size; file++)
		{
			if (m_Board[rank][file].get() != nullptr && m_Board[rank][file]->GetPieceType() == PieceType::Black_King)
			{
				return PiecePosition(file, rank);
			}
		}
	}

	return Constants::NullPosition;
}

void Board::UpdateResources()
{
	PiecePosition whiteKingPosition = GetWhiteKingPosition();
	PiecePosition blackKingPosition = GetBlackKingPosition();

	if (whiteKingPosition == Constants::NullPosition || blackKingPosition == Constants::NullPosition)
		return;

	if (m_IsWhitesTurn)
	{
		m_Flux++;
		if (m_Flux > 9)
			m_Flux = 9;
		m_Gold += whiteKingPosition.y;
		if (m_Gold > 9)
			m_Gold = 9;
	}
	else
	{
		m_BlackFlux++;
		if (m_BlackFlux > 9)
			m_BlackFlux = 9;
		m_BlackGold += blackKingPosition.y;
		if (m_BlackGold > 9)
			m_BlackGold = 9;
	}
}


