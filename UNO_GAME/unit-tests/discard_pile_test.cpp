//
// Created by UNOrganized on 11.12.2023.
//

#include <gtest/gtest.h>
#include "../src/common/game_state/cards/discard_pile.h"

class DiscardPileTest : public ::testing::Test {
protected:
    discard_pile discardPile;
};

TEST_F(DiscardPileTest, CanPlayOnEmptyPile) {
    // Test whether can_play returns true for an empty discard pile,
    // for all cards
    for (size_t i = 0; i < 16; i++){
        for (size_t color = 0; color < 5; color++){
            card* testCard = new card(i, color);
            EXPECT_TRUE(discardPile.can_play(testCard));
            delete testCard;  // Clean up.
        }   
    }
}

TEST_F(DiscardPileTest, SetupGame) {
    // Test whether get_top_card returns nullptr for an empty discard pile.
    std::string err;
    card* testCard = new card(10, 2);
    EXPECT_TRUE(discardPile.try_play(testCard, err)); // put a card on the pile
    discardPile.setup_game(err); // removes all cards from pile
    EXPECT_EQ(discardPile.get_top_card(), nullptr);
}

TEST_F(DiscardPileTest, CanPlayOnSameSymbolOrColor) {
    // Test whether can_play returns true for a card with the same symbol or color.
    std::string err;
    // first card cant have color = 4
    for (size_t i1 = 0; i1 < 16; i1++){
        for (size_t color1 = 0; color1 < 4; color1++){
            // removing old card
            discardPile.setup_game(err);

            card* existingCard = new card(i1, color1);

            discardPile.try_play(existingCard, err);  // Add existingCard to discardPile
            
            // second card can be any card
            for (size_t i2 = 0; i2 < 16; i2++){
                for (size_t color2 = 0; color2 < 5; color2++){
                    card* testCard = new card(i2, color2);
                    
                    bool playable = (i1 == i2 || color1 == color2 || i2 >=13);

                    EXPECT_EQ(discardPile.can_play(testCard), playable);

                    delete testCard;  // Clean up.
                }
            }
        }
    }
}

TEST_F(DiscardPileTest, GetTopCardOfEmptyPile) {
    // Test whether get_top_card returns nullptr for an empty discard pile.
    EXPECT_EQ(discardPile.get_top_card(), nullptr);
}

TEST_F(DiscardPileTest, GetTopCardOfNonEmptyPile) {
    // Test whether get_top_card returns the correct card for a non-empty discard pile.
    card* testCard = new card(2, 3);
    std::string err;

    discardPile.try_play(testCard, err);

    EXPECT_EQ(discardPile.get_top_card(), testCard);
}

TEST_F(DiscardPileTest, CheckSpecialCard) {
    // Test whether check_special_card returns the correct index of the top card.
    card* testCard = new card(11, 2);
    std::string err;

    discardPile.try_play(testCard, err);

    EXPECT_EQ(discardPile.check_special_card(), 11);
}

TEST_F(DiscardPileTest, ChangeColorValid) {
    // Test whether change_color updates the color of a wildcard card successfully.
    card* wildcardCard = new card(13, 4);
    std::string err;

    discardPile.try_play(wildcardCard, err);

    EXPECT_TRUE(discardPile.change_color(wildcardCard, 1, err));  // Change to color 1.

    EXPECT_EQ(wildcardCard->get_color(), 1);
}

TEST_F(DiscardPileTest, ChangeColorInvalidWildcard) {
    // Test whether change_color correctly handles a non-wildcard card.
    card* nonWildcardCard = new card(5, 2);
    std::string err;

    int initialColor = nonWildcardCard->get_color();

    EXPECT_FALSE(discardPile.change_color(nonWildcardCard, 1, err));  // Should fail for non-wildcard.

    // Ensure the color remains unchanged.
    EXPECT_EQ(nonWildcardCard->get_color(), initialColor);
}