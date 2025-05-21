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

	virtual void GetRange(PiecePosition piecePosition, std::vector<ActionMove>& legalMoves) = 0;

	// Wrapper function to get the legal moves for a piece, made so that effects can be applied to available moves
	// Maybe do this more elegantly
	void GetLegalMovesWrapper(PiecePosition piecePosition, std::vector<ActionMove>& legalMoves)
	{
		if (HasEffect(Effect::Stun))
		{
			// No legal moves
			return;
		}

		GetLegalMoves(piecePosition, legalMoves);
	}

	static bool CastRay(const BoardMatrix& board, PiecePosition piecePosition, ActionMove move) {
		PiecePosition targetSquare = move.TargetSquare;
		int dx = targetSquare.x - piecePosition.x;
		int dy = targetSquare.y - piecePosition.y;
		int stepX = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
		int stepY = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;
		for (int x = piecePosition.x + stepX, y = piecePosition.y + stepY; x != targetSquare.x || y != targetSquare.y; x += stepX, y += stepY) {
			if (board[y][x] != nullptr) {
				return false; // Path is blocked
			}
		}
		return true; // Path is clear
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
	
	// Handles all of the render steps common for all pieces
	void RenderWrapper(sf::Sprite& sprite, sf::RenderWindow& window, const sf::Shader& pieceShader, bool isSelectedPiece, float tEffect)
	{
		EffectTint(sprite, tEffect);

		// Handle being picked up with the mouse
		if (isSelectedPiece && Global::MouseLeftPressed)
		{
			// Piece is dragged with mouse
			sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
			sprite.setPosition(sf::Vector2f(mousePosition.x, mousePosition.y));

			const auto& texture = sprite.getTexture();
			sprite.setOrigin(sf::Vector2f(texture.getSize().x / 2.f, texture.getSize().y / 2.f));
		}

		Render(sprite, window, pieceShader, isSelectedPiece);
	}

	// Add extra effects to sprite, like changing color, adding another sprite overlayed etc.
	// Board position and scale are automatically calculated into the sprite beforehand
	virtual void Render(sf::Sprite& sprite, sf::RenderWindow& window, const sf::Shader& pieceShader, bool isSelectedPiece)
	{
		window.draw(sprite, &pieceShader);
	}

	virtual void EffectTint(sf::Sprite& sprite, float tEffect)
	{
		sf::Color color;

		// Dont know if unaffected pieces should have the effect "None" or should have an empty vector, could cause a bug.
		if (m_Effects.empty())
		{
			return;
		}

		for (auto& effect : m_Effects)
		{
			switch (std::get<0>(effect))
			{
			case Effect::Alchemist_Shield:
				// Green color with transparency
				color = sf::Color(0, 255, 0, 64);
				color.g = Lerp(150, 230, tEffect);
				break;
			case Effect::Stun:
				// Black color with transparency
				color = sf::Color(0, 0, 0); 
				color.a = Lerp(70, 150, tEffect);
				break;
			case Effect::Hex:
				color.b = 0;
				// Transition from red to green
				if (tEffect < 0.5)
					color.r = Lerp(100, 200, tEffect * 2);
				else
					color.g = Lerp(150, 100, (tEffect - 0.5) * 2);
				// Transparency
				color.a = Lerp(70, 150, tEffect); 
				break;
			case Effect::Curse:
			{
				// Purple to violet semitransparent
				const sf::Color purple = sf::Color(135, 5, 189, 100);
				const sf::Color violet = sf::Color(224, 3, 172, 100);

				color = Lerp(purple, violet, tEffect); 
				break;
			}
			default:
				break;
			}
		}

		sprite.setColor(color);
	}
	
	void UpdateEffects() {
		for (auto& effect : m_Effects)
		{
			if (std::get<1>(effect) > 0)
			{
				std::get<1>(effect)--;
			}
			else
			{
				RemoveEffect(std::get<0>(effect));
			}

			switch (std::get<0>(effect))
			{
			// Should be first effect in the effects vector maybe
			case Effect::Alchemist_Shield:
				for (auto debuff : m_Effects)
					if (!IsEffectBuff(std::get<0>(debuff)))
					{
						RemoveEffect(std::get<0>(debuff));
					}
				break;
			case Effect::Hex:
				if (m_UpgradeLevel > 1)
				{
					m_UpgradeLevel--;
				}
				break;
			case Effect::Curse:
				if (m_Board && std::get<1>(effect) <= 0) {
					// Mark piece as dead (will be deleted externally)
					m_MarkedForDeletion = true;
				}
				break;
			default:
				break;
			}
		}
	}

	// For a given attack move, checks if move is among valid ones
	ActionMove BasePiece::IsLegalMove(PieceMove move, MoveType moveType)
	{
		std::vector<ActionMove> legalMoves;
		GetLegalMovesWrapper(move.StartSquare, legalMoves);
		for (const auto& legalMove : legalMoves)
		{
			if (legalMove.TargetSquare == move.TargetSquare && legalMove.MoveType == moveType)
			{
				return legalMove;
			}
		}

		return Constants::NullActionMove;
	}
	
	virtual PieceType GetPieceType() = 0;
	PieceColor GetPieceColor() const
	{
		return m_Color;
	}

	virtual std::vector<std::tuple<Effect, int>> GetEffects()
	{
		return m_Effects;
	}

	bool IsMarkedForDeletion() const { return m_MarkedForDeletion; }
	void ClearDeletionMark() { m_MarkedForDeletion = false; }

	void AddEffect(Effect effect, int lifetime)
	{
		m_Effects.push_back(std::make_tuple(effect, lifetime));
	}

	// Copy pasted from llm, dont understand it
	void RemoveEffect(Effect effect)
	{
		// Use remove_if with a custom predicate to find tuples where the first element matches 'effect'
		auto it = std::remove_if(m_Effects.begin(), m_Effects.end(),
			[effect](const std::tuple<Effect, int>& item) {
				return std::get<0>(item) == effect;
			});

		// Erase the removed elements
		if (it != m_Effects.end())
		{
			m_Effects.erase(it, m_Effects.end());
		}
	}

	bool HasEffect(Effect effect)
	{
		for (auto& it = m_Effects.begin(); it != m_Effects.end(); it++)
			if (std::get<0>(*it) == effect)
				return true;
		return false;
	}

protected:
	std::vector<std::tuple<Effect, int>> m_Effects;
	std::shared_ptr<Board> m_Board;
	uint32_t m_UpgradeLevel;
	PieceColor m_Color;
	bool m_MarkedForDeletion = false;
};

#include "../Board.h"

