#include "Inventory.h"

Inventory::Inventory(PieceType deck[s_DeckSize])
{
	// Assign given deck
	for (int i = 0; i < s_DeckSize; i++)
		m_Deck[i] = deck[i];
}

PieceType Inventory::GetDeckPiece(int index) const
{
	assert(index >= 0 && index < s_DeckSize);

	return m_Deck[index];
}

void Inventory::SetDeckPiece(PieceType piece, int index)
{
	assert(index >= 0 && index < s_DeckSize);

	m_Deck[index] = piece;
}

const std::array<PieceType, Inventory::s_DeckSize>& Inventory::GetDeck() const
{
	return m_Deck;
}
