#pragma once

#include <SFML/Graphics.hpp>

#include "Renderer.h"
#include "Board.h"

class Application
{
public:
	Application();
	~Application();

	void Run();


	

private:
	void EventHandler();
private:

	sf::RenderWindow m_Window;
	sf::View m_Viewport;
	bool m_IsRunning = true;
		
	PieceMove m_PreviousMove = { GlobalConstants::NullPosition, GlobalConstants::NullPosition };
	PiecePosition m_SelectedSquare = GlobalConstants::NullPosition;

	Board m_Board;
	Renderer m_Renderer;
};
