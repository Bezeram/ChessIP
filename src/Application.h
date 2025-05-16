#pragma once

#include <SFML/Graphics.hpp>

#include "Renderer.h"
#include "Board.h"
#include "SoundPlayer.h"

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
		
	PieceMove m_PreviousMove = { GlobalConstants::NullPosition, GlobalConstants::NullPosition };
	PiecePosition m_SelectedSquare = GlobalConstants::NullPosition;
	MoveType m_MoveType = MoveType::None;

	std::shared_ptr<Board> m_Board;
	Renderer m_Renderer;
};
