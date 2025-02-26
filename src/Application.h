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
		sf::RenderWindow m_Window;

		enum class GameType
		{
			PLAYER_VS_PLAYER = 0,
		} m_GameType = GameType::PLAYER_VS_PLAYER;

		int m_SelectedSquare = -1;

		Renderer m_Renderer;
		Board m_Board;
	};
}
