#include "Renderer.h"

#include <iostream>

Renderer::Renderer(const sf::Vector2u& screenSize, int boardTileSize)
{
	// Load shader for pieces
	constexpr std::string_view outlineFixFragShader = R"(
		uniform sampler2D texture;

		void main()
		{
			vec4 texColor = texture2D(texture, gl_TexCoord[0].xy);
			vec4 color = texColor * gl_Color;

			// Discard almost-transparent pixels to avoid white outlines
			if (color.a < 0.5)
				discard;

			gl_FragColor = color;
		}
	)";

	if (!m_PieceShader.loadFromMemory(outlineFixFragShader, sf::Shader::Type::Fragment))
		std::cout << "Shader outlineFix could not be loaded.\n";

	// Calculate rendered board properties
	CalculateBoardProperties(screenSize, boardTileSize);
}

void Renderer::DrawBackground(sf::RenderWindow& window)
{
	const auto& tex = ResourceManager::GetInstance().GetTexture("background");
	sf::Sprite sprite(tex);

	float scaleX = static_cast<float>(window.getSize().x) / tex.getSize().x;
	float scaleY = static_cast<float>(window.getSize().y) / tex.getSize().y;
	sprite.setScale(sf::Vector2f(scaleX, scaleY));
	sprite.setPosition(sf::Vector2f(0.f, 0.f));

	window.draw(sprite);
}

void Renderer::DrawBoard(sf::RenderWindow& window, const Board& board, PiecePosition selectedPiecePosition, 
	MoveType moveType, const PieceMove& previousMove, sf::Time deltaTime)
{
	CalculateBoardProperties(window.getSize(), board.GetSize());

	const auto& borderTexture = ResourceManager::GetInstance().GetTexture("board_border");

	sf::Sprite borderSprite(borderTexture);

	// Calculează scalare și poziție
	float scale = (m_BoardLength + m_BoardCellSize * 0.4f) / borderTexture.getSize().x;
	borderSprite.setScale(sf::Vector2f(scale, scale));

	// Pozitionare: deplasezi astfel încât tabla să fie centrată în interior
	sf::Vector2f offset(m_BoardCellSize * 0.2f, m_BoardCellSize * 0.2f);
	borderSprite.setPosition(m_BoardPosition - offset);

	window.draw(borderSprite); // desenează înainte de orice

	// Helper function for drawing a board tile
	sf::RectangleShape tile(sf::Vector2f(m_BoardCellSize, m_BoardCellSize));
	auto drawSquare = [&](PiecePosition position, sf::Color color)
		{
			sf::Vector2f drawPosition = CalculateTilePosition(window.getSize().y, position);

			tile.setFillColor(color);
			tile.setPosition(drawPosition);
			window.draw(tile);
		};

	/// Get legal moves
	std::vector<ActionMove> legalMoves;
	if (selectedPiecePosition != Constants::NullPosition)
	{
		const auto& selectedPiece = board[selectedPiecePosition];
		if (selectedPiece.get() != nullptr)
			selectedPiece->GetLegalMovesWrapper(selectedPiecePosition, legalMoves);
	}

	/// Draw grid board
	const auto& whiteTex = ResourceManager::GetInstance().GetTexture("tile_white");
	const auto& blackTex = ResourceManager::GetInstance().GetTexture("tile_black");

	for (int rank = 0; rank < board.GetSize(); rank++)
	{
		for (int file = 0; file < board.GetSize(); file++)
		{
			PiecePosition gridPosition = PiecePosition(file, rank);
			const sf::Texture& tex = ((rank + file) % 2 == 0) ? whiteTex : blackTex;

			sf::Sprite sprite(tex);
			sprite.setScale(sf::Vector2f(m_BoardCellSize / tex.getSize().x, m_BoardCellSize / tex.getSize().y));
			sprite.setPosition(CalculateTilePosition(window.getSize().y, gridPosition));
			window.draw(sprite);
		}
	}


	/// Draw legal moves, previous move and highlight selected square
	// 
	// Legal moves must be drawn on top of any other highlight
	bool skipPreviousMoveRender[2] = { false, false };
	bool skipSelectHighlight = false;
	// Highlight legal moves and selected square
	if (selectedPiecePosition != Constants::NullPosition)
	{
		// Legal moves
		for (const auto& move : legalMoves)
		{
			// Only consider the specified move type
			if (move.MoveType == moveType)
			{
				sf::Color color = (moveType == MoveType::Move) ? m_ColorLegalMove : m_ColorLegalAction;
				drawSquare(move.TargetSquare, color);

				// Register possible collisions with the previous move..
				if (move.TargetSquare == previousMove.StartSquare)
					skipPreviousMoveRender[0] = true;
				if (move.TargetSquare == previousMove.TargetSquare)
					skipPreviousMoveRender[1] = true;
				// ..and with the select position
				if (move.TargetSquare == selectedPiecePosition)
					skipSelectHighlight = true;
			}
		}

		// Selected piece square
		if (!skipSelectHighlight)
			drawSquare(selectedPiecePosition, m_ColorSelectSquare);
	}
	// Draw previous move
	if (previousMove.StartSquare != Constants::NullPosition && previousMove.TargetSquare != Constants::NullPosition)
	{
		// Highlight previous move if there were no collisions
		if (!skipPreviousMoveRender[0])
			drawSquare(previousMove.StartSquare, m_ColorPreviousMove);
		if (!skipPreviousMoveRender[1])
			drawSquare(previousMove.TargetSquare, m_ColorPreviousMove);
	}

	
	/// Draw pieces (from the top to bottom, left to right)
	// 
	// The position of a piece is counted from the bottom-left, going left to right and bottom-top
	{
		// Update effect animation timer
		// Calculate effect tint keyframe
		// Update timer
		m_EffectAnimationTimer += deltaTime;
		// Keep in range [0, animationTime]
		while (m_EffectAnimationTimer > m_EffectAnimationTotalTime)
			m_EffectAnimationTimer -= m_EffectAnimationTotalTime;

		// Calculate keyframe
		float animationTime = m_EffectAnimationTotalTime.asSeconds();
		float keyFrame = Animation::RiseAndFall(m_EffectAnimationTimer.asSeconds(), animationTime / 2);
		float tEffect = keyFrame / (animationTime / 2);
		tEffect = Animation::EaseOutCubic(tEffect);
		// tEffect is passed in the piece->RenderWrapper function below

		const ResourceManager& rm = ResourceManager::GetInstance();
		// Render piece inner function
		auto renderPiece = [&](PiecePosition position)
			{
				const auto& piece = board[position];
				bool isSelectedPiece = position == selectedPiecePosition;

				// Set texture and shader
				const auto& texture = rm.GetPieceTexture(piece->GetPieceType());
				m_PieceShader.setUniform(std::string("texture"), sf::Shader::CurrentTexture);

				sf::Sprite sprite(texture);
				// Set sprite position and scale on board tile
				sf::Vector2f drawPosition = CalculateTilePosition(window.getSize().y, position);
				float scale = m_BoardCellSize / texture.getSize().x;
				sprite.setPosition(drawPosition);
				sprite.setScale(sf::Vector2f(scale, scale));

				// Sprite is rendered using a wrapper, which calls an overriden function where the sprite is drawn
				piece->RenderWrapper(sprite, window, m_PieceShader, isSelectedPiece, tEffect);
			};

		for (int rank = 0; rank < board.GetSize(); rank++)
		{
			for (int file = 0; file < board.GetSize(); file++)
			{
				PiecePosition piecePosition = PiecePosition(file, rank);
				bool isSelectedPiece = piecePosition == selectedPiecePosition;
				const auto& piece = board[piecePosition];
				// Skip if empty tile, or if it's the selected piece
				// which must be drawn at the end, on top of everything else
				if (piece.get() == nullptr || isSelectedPiece)
					continue;

				renderPiece(piecePosition);
			}
		}
		// Render selected piece
		if (selectedPiecePosition != Constants::NullPosition)
			renderPiece(selectedPiecePosition);
	}
}

void Renderer::CalculateBoardProperties(const sf::Vector2u& screenSize, int boardGridSize)
{
	sf::Vector2u resolution = screenSize;
	sf::Vector2f boardPadding = { resolution.x * m_BoardPadding01.x, resolution.y * m_BoardPadding01.y };
	m_BoardLength = resolution.y - boardPadding.y * 2.f;
	m_BoardCellSize = m_BoardLength / (float)boardGridSize;
	// Calculate board position (centralised according to screen, drawn from the bottom left)
	float boardPositionX = (resolution.x - m_BoardLength) / (float)boardGridSize;
	float boardPositionY = resolution.y * m_BoardPadding01.y;
	m_BoardPosition = sf::Vector2f(boardPositionX, boardPositionY);
}

void Renderer::DrawInventory(sf::RenderWindow& window, Inventory inventory, sf::Vector2i selectedSlotIndex, sf::Time deltaTime)
{
	const auto& inventoryTexture = ResourceManager::GetInstance().GetTexture("inventory");

	sf::Sprite inventorySprite(inventoryTexture);
	sf::Vector2f scale = CalculateInventoryScale();
	sf::Vector2f position = CalculateInventoryPosition();

	inventorySprite.setScale(scale);
	inventorySprite.setPosition(position);

	window.draw(inventorySprite);

	// Set animation time in between 0 and total time
	m_InventoryHighlightTimer += deltaTime;
	while (m_InventoryHighlightTimer > m_InventoryHighlightTotalTime)
	{
		m_InventoryHighlightTimer -= m_InventoryHighlightTotalTime;
	}

	// Draw pieces
	const auto& deck = inventory.GetDeck();
	for (int i = 0; i < deck.size(); i++)
	{
		PieceType pieceType = deck[i];

		const auto& texture = ResourceManager::GetInstance().GetPieceTexture(pieceType);
		float spriteScale = Inventory::s_SlotSize.x * scale.x / texture.getSize().x;
		sf::Sprite piece(texture);
		// Calculate inventory slot position
		int rank = i / 4;
		int file = i % 4;
		sf::Vector2f spritePosition = Inventory::CalculateSlotPosition(position, scale, sf::Vector2i(file, rank));

		piece.setPosition(spritePosition);
		piece.setScale(sf::Vector2f(spriteScale, spriteScale));
		window.draw(piece);
	}

	// Highlight selected piece
	if (selectedSlotIndex != Constants::NullPosition)
	{
		float outlineThickness = m_InventoryHighlightThickness * scale.x / scale.y;
		sf::RectangleShape highlightTile(
			Inventory::s_SlotSize.componentWiseMul(scale) - sf::Vector2f(outlineThickness, outlineThickness));

		sf::Vector2f tilePosition = Inventory::CalculateSlotPosition(position, scale, selectedSlotIndex);
		// Add thickness offset
		tilePosition += sf::Vector2f(outlineThickness, outlineThickness);

		float totalTimeSeconds = m_InventoryHighlightTotalTime.asSeconds();
		// The first half of the animation the transparency increases,
		// whilst on the second half it decreases
		float keyFrame = Animation::RiseAndFall(m_InventoryHighlightTimer.asSeconds(), totalTimeSeconds / 2);
		// Normalize in range [0, 1]
		float tHighlight = keyFrame / (totalTimeSeconds / 2);
		// Use ease out animation
		tHighlight = Animation::EaseInQuadric(tHighlight);

		// Lerp the transparency
		sf::Color fillColor = m_ColorPreviousMove;
		fillColor.a = Lerp(30, 150, tHighlight);
		sf::Color outlineColor = m_ColorSelectSquare;
		outlineColor.a = Lerp(30, 255, tHighlight);

		highlightTile.setFillColor(fillColor);
		highlightTile.setOutlineColor(m_ColorSelectSquare);
		highlightTile.setOutlineThickness(outlineThickness);
		highlightTile.setPosition(tilePosition);

		window.draw(highlightTile);
	}
}

void Renderer::DrawResourceBars(sf::RenderWindow& window, int flux, int gold) const
{
	const auto& fluxTex = ResourceManager::GetInstance().GetTexture(Textures::Flux_Bar_ + std::to_string(flux));
	const auto& goldTex = ResourceManager::GetInstance().GetTexture(Textures::Gold_Bar_ + std::to_string(gold));

	sf::Sprite goldSprite(goldTex);
	sf::Sprite fluxSprite(fluxTex);

	// Dimensiuni dorite: înălțimea să fie 1/10 din tabla
	float targetHeight = m_BoardLength / 8.f;

	float goldScale = targetHeight * 1.2f / static_cast<float>(goldTex.getSize().y);
	float fluxScale = targetHeight * 1.2f / static_cast<float>(fluxTex.getSize().y);

	goldSprite.setScale(sf::Vector2f(goldScale, goldScale));
	fluxSprite.setScale(sf::Vector2f(fluxScale, fluxScale));

	// Poziții: dreapta tablei, sus
	float margin = m_BoardCellSize * 0.75f;
	float posX = m_BoardPosition.x + m_BoardLength + margin;

	goldSprite.setPosition(sf::Vector2f(posX, margin * 0.3f));
	fluxSprite.setPosition(sf::Vector2f(posX, goldSprite.getGlobalBounds().size.y + margin * 0.2f));

	window.draw(goldSprite);
	window.draw(fluxSprite);
}

sf::Vector2f Renderer::GetBoardPosition() const
{
	return m_BoardPosition;
}

float Renderer::GetBoardSize() const
{
	return m_BoardLength;
}

float Renderer::GetBoardCellSize() const
{
	return m_BoardCellSize;
}

sf::Vector2f Renderer::CalculateInventoryPosition() const
{
	float targetHeight = m_BoardLength / 3.f;

	sf::Vector2f offset(m_BoardCellSize * 0.95f, 0.f);
	float posX = m_BoardPosition.x + m_BoardLength + offset.x;
	float posY = m_BoardPosition.y + m_BoardLength - targetHeight;
	return { posX, posY };
}

sf::Vector2f Renderer::CalculateInventoryScale() const
{
	const auto& inventoryTexture = ResourceManager::GetInstance().GetTexture("inventory");

	sf::Sprite inventorySprite(inventoryTexture);

	// Dorim:
	// - înălțimea = 1/3 din m_BoardLength
	// - lățimea = 2/3 din m_BoardLength
	float targetHeight = m_BoardLength / 3.f;
	float targetWidth = m_BoardLength * (2.f / 3.f);

	// Obține dimensiunile originale ale texturii
	float texWidth = static_cast<float>(inventoryTexture.getSize().x);
	float texHeight = static_cast<float>(inventoryTexture.getSize().y);

	// Calculează scaling pe x și y separat
	float scaleX = targetWidth / texWidth;
	float scaleY = targetHeight / texHeight;

	return { scaleX, scaleY };
}

sf::Vector2f Renderer::CalculateTilePosition(uint32_t windowHeight, PiecePosition position)
{
	int file = position.x;
	int rank = position.y;
	return sf::Vector2f(m_BoardPosition.x + file * m_BoardCellSize,
		windowHeight - m_BoardPosition.y - (rank + 1) * m_BoardCellSize);
}

sf::Vector2i Renderer::MouseCellIndex(int windowHeight, const sf::Vector2f& mousePosition) const
{
	return sf::Vector2i(
		int((std::floor((mousePosition.x - m_BoardPosition.x) / m_BoardCellSize))),
		int((windowHeight - (mousePosition.y - m_BoardPosition.y)) / m_BoardCellSize) - 1);
}
