#pragma once

#include <SFML/Graphics.hpp>

#include "Renderer.h"
#include "Board.h"

namespace ChessIP
{
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
		
		Move m_PreviousMove = { -1, -1 };
		int m_SelectedSquare = -1;

		Board m_Board;
		Renderer m_Renderer;
	};
}
