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
	BasePiece(std::shared_ptr<Board> board, PieceColor color, uint32_t upgradeLevel = 1)
		: m_Board(std::move(board)), m_Color(color), m_UpgradeLevel(upgradeLevel)
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
	// Add extra effects to sprite, like changing color, adding another sprite overlayed etc.
	// Board position and scale are automatically calculated into the sprite beforehand
	virtual void Render(sf::Sprite& sprite, sf::RenderWindow& window, const sf::Shader& pieceShader, bool isSelectedPiece)
	{
		if (isSelectedPiece && Global::MouseLeftPressed)
		{
			// Piece is dragged with mouse
			sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
			sprite.setPosition(sf::Vector2f(mousePosition.x, mousePosition.y));

			const auto& texture = sprite.getTexture();
			sprite.setOrigin(sf::Vector2f(texture.getSize().x / 2.f, texture.getSize().y / 2.f));
		}

		window.draw(sprite, &pieceShader);
	}

	// For a given attack move, checks if move is among valid ones
	ActionMove BasePiece::IsLegalMove(PieceMove move, MoveType moveType)
	{
		std::vector<ActionMove> legalMoves;
		GetLegalMoves(move.StartSquare, legalMoves);
		for (const auto& legalMove : legalMoves)
		{
			if (legalMove.TargetSquare == move.TargetSquare && legalMove.MoveType == moveType)
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

	virtual std::vector<Effect> GetEffects()
	{
		return m_Effects;
	}

	virtual void AddEffect(Effect effect)
	{
		m_Effects.push_back(effect);
	}

	virtual void RemoveEffect(Effect effect)
	{
		auto it = std::remove(m_Effects.begin(), m_Effects.end(), effect);
		if (it != m_Effects.end())
		{
			m_Effects.erase(it, m_Effects.end());
		}
	}

protected:
	std::vector<Effect> m_Effects;
	std::shared_ptr<Board> m_Board;
	uint32_t m_UpgradeLevel;
	PieceColor m_Color;
};

#include "../Board.h"

