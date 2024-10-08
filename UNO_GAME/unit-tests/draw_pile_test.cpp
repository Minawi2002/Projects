//
// Created by UNOrganized on 11.12.2023.
//

#include <gtest/gtest.h>
#include "../src/common/game_state/cards/draw_pile.h"


// Test fixture for draw_pile class
class DrawPileTest : public ::testing::Test {
protected:
    draw_pile drawPile;
};


// Test the is_empty method
TEST_F(DrawPileTest, IsEmptyTest) {
    // Initially, the draw pile should be empty
    EXPECT_TRUE(drawPile.is_empty());

    std::vector<card*> cards = {new card(1, 2), new card(3, 1)};
    draw_pile nonEmptyDrawPile(cards, nullptr);
    EXPECT_FALSE(nonEmptyDrawPile.is_empty());
}

// Test the get_nof_cards method
TEST_F(DrawPileTest, GetNofCardsTest) {
    // Initially, the draw pile should have zero cards
    EXPECT_EQ(drawPile.get_nof_cards(), 0);

    // Add cards to the draw pile and check the count
    std::vector<card*> cards = {new card(1, 2), new card(3, 1)};
    draw_pile nonEmptyDrawPile(cards, nullptr);
    EXPECT_EQ(nonEmptyDrawPile.get_nof_cards(), 2);
}

TEST_F(DrawPileTest, SetupGame) {
    draw_pile pile;

    // Setup the game
    std::string err;
    pile.setup_game(err);

    // Check if the number of cards is correct
    EXPECT_EQ(pile.get_nof_cards(), 108);

    // Check if the pile is not empty
    EXPECT_FALSE(pile.is_empty());
}

// Test when the draw pile is empty, and the setup_game method is called to refill it.
TEST_F(DrawPileTest, DrawEmpty) {
    draw_pile pile;
    player* player1 = new player("testplayer");
    std::string err;

    // Setup the game
    pile.setup_game(err);

    // Draw a card
    card* drawn_card = nullptr;
    bool draw_result = pile.draw(player1, drawn_card, err);

    // Check if the draw was successful
    EXPECT_TRUE(draw_result);
    EXPECT_NE(drawn_card, nullptr);
    EXPECT_EQ(player1->get_nof_cards(), 1);

    delete player1;
}

// Draw pile not empty and has cards to draw from
TEST_F(DrawPileTest, Draw) {
    std::vector<card*> cards = {new card(1, 2), new card(3, 1)};
    draw_pile pile(cards, nullptr);
    player* player1 = new player("testplayer");
    std::string err;

    // Draw a card (which will make the draw pile empty)
    card* drawn_card = nullptr;
    bool draw_result = pile.draw(player1, drawn_card, err);

    // Check if the draw was unsuccessful due to an empty draw pile
    EXPECT_TRUE(draw_result);
    EXPECT_EQ(player1->get_nof_cards(), 1);

    delete player1;
}

// Test when the draw pile is empty, and the setup_game method is called to refill it.
TEST_F(DrawPileTest, DistributeCardEmpty) {
    draw_pile pile;
    player* player1 = new player("testplayer");
    std::string err;

    // Setup the game
    pile.setup_game(err);

    // Distribute a card
    pile.distribute_card(1, player1, err);

    // Check if the card was distributed
    EXPECT_EQ(player1->get_nof_cards(), 1);

    delete player1;
}

// Test if loop is entered 0 times
TEST_F(DrawPileTest, DistributeCard0) {
    draw_pile pile;
    player* player1 = new player("testplayer");
    std::string err;

    // Setup the game
    pile.setup_game(err);

    // Distribute a card
    pile.distribute_card(0, player1, err);

    // Check if the card was distributed
    EXPECT_EQ(player1->get_nof_cards(), 0);

    delete player1;
}

// Test if loop is entered more than 1 times
TEST_F(DrawPileTest, DistributeCard2) {
    draw_pile pile;
    player* player1 = new player("testplayer");
    std::string err;

    // Setup the game
    pile.setup_game(err);

    // Distribute a card
    pile.distribute_card(2, player1, err);

    // Check if the card was distributed
    EXPECT_EQ(player1->get_nof_cards(), 2);

    delete player1;
}

// testing the remove of the top card of a fresh set
TEST_F(DrawPileTest, RemoveTop) {
    draw_pile pile;
    std::string err;

    // Setup the game
    pile.setup_game(err);

    // Remove the top card
    card* drawn_card = pile.remove_top(err);

    // Check if the card was removed
    EXPECT_NE(drawn_card, nullptr);
    EXPECT_EQ(pile.get_nof_cards(), 107);
}

TEST_F(DrawPileTest, RemoveTop_onecard) {
    std::vector<card*> cards = {new card(1, 2), new card(3, 1)};
    draw_pile pile(cards, nullptr);
    std::string err;

    // Remove the top card
    card* drawn_card = pile.remove_top(err);

    // Check if the card was removed
    EXPECT_NE(drawn_card, nullptr);
    EXPECT_EQ(pile.get_nof_cards(), 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
