#pragma once

#include <SFML/Graphics.hpp>

#include "Renderer.h"
#include "Board.h"

namespace ChessAI
{
	class Application
	{
	public:
		Application();
		~Application();

		void run();

	private:
		sf::RenderWindow m_Window;

		ChessAI::Renderer m_Renderer;
		ChessAI::Board m_Board;
	};
}
