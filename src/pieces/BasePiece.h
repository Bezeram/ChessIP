#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "../Utils.h"

class BasePiece;

typedef std::vector<std::vector<std::unique_ptr<BasePiece>>> BoardMatrix;

class BasePiece
{
public:
	// Every piece must implement this function to get the valid moves
	virtual void GetLegalMoves(const BoardMatrix& board, sf::Vector2i piecePosition, std::vector<ActionMove>& legalMoves) = 0;
	// By default it moves the piece to the corepondent square
	// Assumes the move is valid
	virtual void ExecuteMove(BoardMatrix& board, PiecePosition piecePosition, ActionMove move)
	{
		PiecePosition targetSquare = move.TargetSquare;
		// std::move piece to the new square
		//board[targetSquare.y][targetSquare.x] = std::move(board[piecePosition.y][piecePosition.x]);
		// Clear old position
		//board[piecePosition.y][piecePosition.x] = nullptr;
	}

	// For a given attack move, checks if move is among valid ones
	ActionMove BasePiece::IsLegalMove(const BoardMatrix& board, PieceMove move)
	{
		std::vector<ActionMove> legalMoves;
		GetLegalMoves(board, move.StartSquare, legalMoves);
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
protected:
	std::vector<Effect> m_Effects;
};

