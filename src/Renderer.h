#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "Board.h"
#include "Inventory.h"

class Renderer
{
public:
	Renderer(const sf::Vector2u& screenSize, int boardGridSize);

	/*
		Board is drawn from white's perspective, from the top left square, left to right;
	*/
	void Renderer::DrawBackground(sf::RenderWindow& window);
	void DrawBoard(sf::RenderWindow& window, const Board& board, PiecePosition selectedSquare, 
		MoveType moveType, const PieceMove& previousMove, sf::Time deltaTime);
	void CalculateBoardProperties(const sf::Vector2u& screenSize, int boardTileSize);
	void Renderer::DrawInventory(sf::RenderWindow& window, Inventory inventory, sf::Vector2i selectedSlotIndex, sf::Time deltaTime);
	void Renderer::DrawInfo(sf::RenderWindow& window);
	void DrawResourceBars(sf::RenderWindow& window, int flux, int gold) const;
	void DrawMenu(sf::RenderWindow& window);


	sf::Vector2f GetBoardPosition() const;
	float GetBoardSize() const;
	float GetBoardCellSize() const;
	sf::Vector2f CalculateInventoryPosition() const;
	sf::Vector2f CalculateInventoryScale() const;

	sf::Vector2f CalculateTilePosition(uint32_t windowHeight, PiecePosition position);

	sf::Vector2i MouseCellIndex(int windowHeight, const sf::Vector2f& mousePosition) const;

	sf::FloatRect GetPlayButtonBounds() const;

private:
	sf::Shader m_PieceShader;

	// Expressed as a percentage of the viewport size
	sf::Vector2f m_BoardPadding01 = { 0.05f, 0.05f };
	sf::Vector2f m_BoardPosition = { 0.f, 0.f };
	float m_BoardLength;
	float m_BoardCellSize;

	sf::Color m_ColorDarkSquare = { 0, 0, 0 };
	sf::Color m_ColorWhiteSquare = { 255, 255, 255 };
	sf::Color m_ColorSelectSquare = { 255, 122, 0, 127 };
	sf::Color m_ColorPreviousMove = { 255, 209, 0, 125 };

	sf::Color m_ColorIllegalMove = { 255, 30, 30, 127 };
	sf::Color m_ColorLegalMove = { 255, 174, 0, 127 };
	sf::Color m_ColorLegalAction = { 162, 0, 219, 127 };

	sf::Time m_InventoryHighlightTimer = sf::Time::Zero;
	sf::Time m_InventoryHighlightTotalTime = sf::seconds(2.5f);
	float m_InventoryHighlightThickness = 1.f;

	sf::Time m_EffectAnimationTimer = sf::Time::Zero;
	sf::Time m_EffectAnimationTotalTime = sf::seconds(6.f);

	sf::FloatRect m_PlayButtonBounds;
};
