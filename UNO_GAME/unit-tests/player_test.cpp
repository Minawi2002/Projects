//
// Created by UNOrganized on 11.12.2023.
//

#include "gtest/gtest.h"
#include "../src/common/game_state/player/player.h"
#include "../src/common/game_state/cards/card.h"
#include "../src/common/game_state/player/hand.h"
class PlayerTest : public ::testing::Test {
protected:
    void SetUp() override {
        player1 = new player("TestPlayer");
    }

    void TearDown() override {
        delete player1;
    }

    player* player1;
    std::string err;
};

TEST_F(PlayerTest, ConstructorTest) {
    EXPECT_EQ(player1->get_player_name(), "TestPlayer");
    EXPECT_EQ(player1->get_score(), 0);
    EXPECT_FALSE(player1->has_called_uno());
}

TEST_F(PlayerTest, ScoreTest) {
    player1->set_score(100);
    EXPECT_EQ(player1->get_score(), 100);
}

TEST_F(PlayerTest, NumberOfCardsTest) {
    EXPECT_EQ(player1->get_nof_cards(), 0);
}

TEST_F(PlayerTest, HandScoreTest) {
    EXPECT_EQ(player1->get_hand_score(), 0);
}

// Testing if the set/reset function works properly by setting/reseting uno
TEST_F(PlayerTest, Call_reset_UNO) {
    EXPECT_TRUE(player1->set_called_uno(false,err));
    EXPECT_TRUE(player1->set_called_uno(true,err));
    EXPECT_FALSE(player1->set_called_uno(true,err));
    EXPECT_EQ(err,"You already called uno.");
    EXPECT_TRUE(player1->set_called_uno(false,err));
    EXPECT_TRUE(player1->set_called_uno(true,err));
}

TEST_F(PlayerTest, AddCardTest) {
    // Add a card to the player's hand
    card* c_1_red= new card(1,2);
    player1->add_card(c_1_red,err);

    // Check if the card was added successfully
    EXPECT_EQ(player1->get_nof_cards(), 1);
    EXPECT_EQ(player1->get_hand_score(), 1);
}

TEST_F(PlayerTest, AddMultipleCardsTest) {
    // Add multiple cards to the player's hand
    card* c_1_red= new card(1,2);
    card* c_2_blue= new card(2, 1);
    card* c_3_green= new card(3, 3);
    card* reverse_green= new card(10,3);
    player1->add_card(c_1_red,err);
    player1->add_card(c_2_blue,err);
    player1->add_card(c_3_green,err);
    player1->add_card(reverse_green,err);

    EXPECT_EQ(player1->get_nof_cards(), 4);
    EXPECT_EQ(player1->get_hand_score(), 26);
}

TEST_F(PlayerTest, RemoveCardTest) {
    card* c_1_red = new card(1,2);
    player1->add_card(c_1_red, err);
    EXPECT_EQ(player1->get_nof_cards(), 1);
    std::string card_id = "1";
    player1->remove_card(c_1_red->get_id(),c_1_red, err);
    EXPECT_EQ(player1->get_nof_cards(), 0);
}

TEST_F(PlayerTest, SetupRound) {
    card* c_1_red= new card(1,2);
    card* c_2_blue= new card(2, 1);
    card* c_3_green= new card(3, 3);
    card* reverse_green= new card(10,3);
    player1->add_card(c_1_red,err);
    player1->add_card(c_2_blue,err);
    player1->add_card(c_3_green,err);
    player1->add_card(reverse_green,err);
    //set uno for player
    EXPECT_TRUE(player1->set_called_uno(true,err));
    player1->setup_round(err);
    //test if it resetted called uno and hand
    EXPECT_TRUE(player1->set_called_uno(true,err));
    EXPECT_EQ(player1->get_nof_cards(), 0);

}

TEST_F(PlayerTest, GetHandTest) {
    // Add a few cards to the player's hand
    card* c_1_red = new card(1,2);
    card* c_2_blue = new card(2,1);
    player1->add_card(c_1_red, err);
    player1->add_card(c_2_blue, err);

    // Get the player's  hand
    const hand* hand1 = player1->get_hand();
    // Check if the hand contains the correct cards
    EXPECT_EQ(hand1->get_cards()[0]->get_index(), 1);
    EXPECT_EQ(hand1->get_cards()[0]->get_color(), 2);
    EXPECT_EQ(hand1->get_cards()[1]->get_index(), 2);
    EXPECT_EQ(hand1->get_cards()[1]->get_color(), 1);
}

