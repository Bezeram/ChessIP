#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include <utility>

#include "../Utils.h"

class Board;
class BasePiece;

typedef std::vector<std::vector<std::unique_ptr<BasePiece>>> BoardMatrix;

class BasePiece
{
public:
	BasePiece(std::shared_ptr<Board> board, PieceColor color)
		: m_Board(std::move(board)), m_Color(color)
	{
	}

	// Every piece must implement this function to get the valid moves
	virtual void GetLegalMoves(PiecePosition piecePosition, std::vector<ActionMove>& legalMoves) = 0;
	// By default it moves the piece to the corepondent square
	// Assumes the move is valid
	virtual void ExecuteMove(BoardMatrix& board, PiecePosition piecePosition, ActionMove move)
	{
		PiecePosition targetSquare = move.TargetSquare;
		// Move piece to the new square
		board[targetSquare.y][targetSquare.x] = std::move(board[piecePosition.y][piecePosition.x]);
		// Clear old position
		board[piecePosition.y][piecePosition.x] = nullptr;
	}

	// For a given attack move, checks if move is among valid ones
	ActionMove BasePiece::IsLegalMove(PieceMove move)
	{
		std::vector<ActionMove> legalMoves;
		GetLegalMoves(move.StartSquare, legalMoves);
		for (const auto& legalMove : legalMoves)
		{
			if (legalMove.TargetSquare == move.TargetSquare)
			{
				return legalMove;
			}
		}

		return GlobalConstants::NullActionMove;
	}
	
	virtual PieceType GetPieceType() = 0;
	PieceColor GetPieceColor() const
	{
		return m_Color;
	}
protected:
	std::vector<Effect> m_Effects;
	std::shared_ptr<Board> m_Board;
	PieceColor m_Color;
};

#include "../Board.h"

