#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

#include "Renderer.h"
#include "Board.h"
#include "SoundPlayer.h"
#include "Inventory.h"

class Application
{
public:
	Application(WindowSettings windowSettings);
	~Application();

	void Run();

	std::shared_ptr<Board>& GetBoard() { return m_Board; }

private:
	void EventHandler();
private:

	sf::RenderWindow m_Window;
	sf::View m_Viewport;
	bool m_IsRunning = true;
		
	PieceMove m_PreviousMove = { Constants::NullPosition, Constants::NullPosition };
	PiecePosition m_SelectedSquare = Constants::NullPosition;
	MoveType m_MoveType = MoveType::None;
	sf::Vector2i m_SelectedInventorySlot = Constants::NullPosition;

	void MoveHandler_MousePressed(const sf::Event::MouseButtonPressed* buttonPressed);
	void MoveHandler_MouseReleased(const sf::Event::MouseButtonReleased* buttonReleased);

	void InventoryHandler_MousePressed(const sf::Event::MouseButtonPressed* buttonPressed);

	std::shared_ptr<Board> m_Board;
	Renderer m_Renderer;

	PieceType m_DefaultDeck[Inventory::s_DeckSize] =
	{
		PieceType::White_Alchemist,
		PieceType::White_Archer,
		PieceType::White_Builder,
		PieceType::White_GrimReaper,
		PieceType::White_PlagueDoctor,
		PieceType::White_Rogue,
		PieceType::White_Trebuchet,
		PieceType::White_Witch,
	};
	Inventory m_Inventory;
	sf::Time m_EffectAnimationTimer = sf::Time::Zero;
};
