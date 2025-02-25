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

		void Run();

	private:
		sf::RenderWindow m_Window;

		enum class GameType
		{
			PLAYER_VS_PLAYER = 0,
		} m_GameType = GameType::PLAYER_VS_PLAYER;

		int m_SelectedSquare = -1;

		ChessAI::Renderer m_Renderer;
		ChessAI::Board m_Board;
	};
}
