#pragma once

#include "Utils.h"
#include <array>

class Inventory
{
public:
	const inline static int s_DeckSize = 8;
	const inline static sf::Vector2f s_SlotGap = sf::Vector2f(50, 40);
	const inline static sf::Vector2f s_SlotSize = sf::Vector2f(164, 154);
	const inline static sf::Vector2f s_TopLeftSlotPosition = sf::Vector2f(63, 44);
	const inline static sf::Vector2f s_InventorySize = sf::Vector2f(929, 438);
public:
	inline static sf::Vector2f CalculateSlotPosition(sf::Vector2f inventoryPosition, sf::Vector2f inventoryScale, sf::Vector2i selectedSlotIndex)
	{
		sf::Vector2f fullSlotSize = (Inventory::s_SlotGap + Inventory::s_SlotSize).componentWiseMul(inventoryScale);
		sf::Vector2f topLeftSlotPosition = inventoryPosition + Inventory::s_TopLeftSlotPosition.componentWiseMul(inventoryScale);
		sf::Vector2f slotOffset = sf::Vector2f(selectedSlotIndex).componentWiseMul(fullSlotSize);
		return topLeftSlotPosition + slotOffset;
	}

public:
	Inventory(PieceType deck[s_DeckSize]);

	PieceType GetDeckPiece(int index) const;
	void SetDeckPiece(PieceType piece, int index);

	const std::array<PieceType, s_DeckSize>& GetDeck() const;

private:
	std::array<PieceType, s_DeckSize> m_Deck;
};