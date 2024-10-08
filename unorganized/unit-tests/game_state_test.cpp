//
// Created by UNOrganized on 08.12.2023.
//

#include "gtest/gtest.h"

#define private public

#include "../src/common/exceptions/UnoException.h"
#include "../src/common/game_state/game_state.h"
#include "../src/common/serialization/json_utils.h"

//test function wrap_up_round for game not finished
TEST(GameStateTest, wrap_up_round1)
{

    game_state gameState;
    //add 4 players to the game state
    std::string err;
    player p1("ich");
    player p2("du");
    player p3("er");
    player p4("sie");
    gameState.add_player(&p1, err);
    gameState.add_player(&p2, err);
    gameState.add_player(&p3, err);
    gameState.add_player(&p4, err);

    //p3 wins and has no cards
    //p1 has a reverse and a 3 left (23points)
    card revcard(REVERSE_DIRECTION,2);
    card green3(3,3);
    p1.add_card(&revcard, err);
    p1.add_card(&green3, err);
    //p2 has a red5 (5 points)
    card r5(5, 1);
    p2.add_card(&r5, err);
    
    //player 4 has a wildcard (50 points)
    card wildcard(WILDCARD, 4);
    p4.add_card(&wildcard, err);

    EXPECT_EQ(p3.get_nof_cards(), 0);
    //do wrap up round and check if player 3 has 78 points and all others 0 points    
    gameState.wrap_up_round(err);
    EXPECT_EQ(p3.get_score(), 78);
    EXPECT_EQ(p1.get_score(), 0);
    EXPECT_EQ(p2.get_score(), 0);
    EXPECT_EQ(p4.get_score(), 0);
}

//test function wrap_up_round for game finished
TEST(GameStateTest, wrap_up_round2)
{

    game_state gameState;
    //add 4 players to the game state
    std::string err;
    player p1("ich");
    player p2("du");
    player p3("er");
    player p4("sie");
    gameState.add_player(&p1, err);
    gameState.add_player(&p2, err);
    gameState.add_player(&p3, err);
    gameState.add_player(&p4, err);
    
    //player 4 has a wildcard (50 points)
    card wildcard(WILDCARD, 4);
    p4.add_card(&wildcard, err);

    EXPECT_EQ(p3.get_nof_cards(), 0);

    //test if it stops when the max points of 500 is reached
    for(int i=0; i<5; i++)
    {//add a lot of points (500) to the round with wild cards
        card wildcard1(WILDCARD, 4);
        card wildcard2(WILDCARD, 4);
        p2.add_card(&wildcard1, err);
        p1.add_card(&wildcard2, err);
    }
    gameState.wrap_up_round(err);
    //the game should now be finished
    EXPECT_TRUE(gameState._is_finished->get_value());
    //are the points correct?
    EXPECT_EQ(p3.get_score(), 550);
    EXPECT_EQ(p1.get_score(), 0);
    EXPECT_EQ(p2.get_score(), 0);
    EXPECT_EQ(p4.get_score(), 0);
}

//test function questionmarkcard
TEST(GameStateTest, questionmarkcard_test){

    game_state gameState;
    std::string err;

    //enable qmc in gamestate
    gameState._play_with_questionmark->set_value(true);

    //add 4 players to the game state, start the game
    player p1("player1");
    player p2("player2");
    player p3("player3");
    player p4("player4");
    gameState.add_player(&p1, err);
    gameState.add_player(&p2, err);
    gameState.add_player(&p3, err);
    gameState.add_player(&p4, err);
    gameState.start_game(err);

    //set up round
    gameState.setup_round(err);


    //remove a random number of cards from each player between 0 and 6 and print it
    for(int i=0; i<4; i++){
        int random_number = rand() % 7;
        for(int j=0; j<random_number; j++){
            gameState._players[i]->_hand->_cards.pop_back();
        }
    }
    
    //give player 1 a questionmarkcard
    card qmc(15, 4);
    p1.add_card(&qmc, err);

    //count the number of cards in the game
    int cards_in_game = 0;
    for(int i=0; i<4; i++){
        cards_in_game += gameState._players[i]->_hand->get_nof_cards() ;
    }
     //because player 1 has a questionmarkcard that will be played
    cards_in_game -= 1;

    //player 1 plays questionmarkcard
    gameState.play_card(&p1, qmc.get_id(), err);

    //check if the number of cards in the hands is the same as before
    int cards_in_game_after = 0;
    for(int i=0; i<4; i++){
        cards_in_game_after += gameState._players[i]->_hand->get_nof_cards() ;
    }
    EXPECT_EQ(cards_in_game, cards_in_game_after);

    //find out which player has the most cards
    int largest_hand_size = 0;
    for(int i=0; i<4; i++){
        if(gameState._players[i]->_hand->get_nof_cards()  > largest_hand_size){
            largest_hand_size = gameState._players[i]->_hand->get_nof_cards() ;
        }
    }

    //find smallest hand size
    int smallest_hand_size = 7;
    for(int i=0; i<4; i++){
        if(gameState._players[i]->_hand->get_nof_cards()  < smallest_hand_size){
            smallest_hand_size = gameState._players[i]->_hand->get_nof_cards() ;
        }
    }

    int largest_difference = largest_hand_size - smallest_hand_size;

    //check the difference and the hand sizes
    EXPECT_TRUE(largest_difference == 0 || largest_difference == 1);

}

//test skip_player
TEST(GameStateTest, skip_player)
{
    std::string err;
    game_state gameState;
    //add 2 players
    player p1("ich");
    player p2("du");
    gameState.add_player(&p1, err);
    gameState.add_player(&p2, err);
    
    //gameState.start_game(err);
    gameState.skip_player(err);

    //edge case if the second player gets skipped (prev & current player are the same)
    EXPECT_EQ(gameState._current_player_idx->get_value(), 1);
    EXPECT_EQ(gameState._previous_player_idx->get_value(), 1);
    //if the first player gets skipped
    gameState._current_player_idx->set_value(1);
    gameState._previous_player_idx->set_value(0);
    gameState.skip_player(err);
    EXPECT_EQ(gameState._current_player_idx->get_value(), 0);
    EXPECT_EQ(gameState._previous_player_idx->get_value(), 0);

    //add more players
    player p3("er");
    player p4("sie");
    gameState.add_player(&p3, err);
    gameState.add_player(&p4, err);
    //edge case: if the previous player is not the one behind the current player
    gameState._current_player_idx->set_value(2);
    gameState._previous_player_idx->set_value(0);
    gameState.skip_player(err);
    EXPECT_EQ(gameState._current_player_idx->get_value(), 3);
    EXPECT_EQ(gameState._previous_player_idx->get_value(), 1);
    //normal skip with 4 players
    gameState._current_player_idx->set_value(1);
    gameState._previous_player_idx->set_value(0);
    gameState.skip_player(err);
    EXPECT_EQ(gameState._current_player_idx->get_value(), 2);
    EXPECT_EQ(gameState._previous_player_idx->get_value(), 0);
}

//test reverse_direction
TEST(GameStateTest, reverse_direction)
{
    game_state gameState;
    gameState._play_direction->set_value(1);
    gameState.reverse_direction();
    EXPECT_EQ(-1,gameState._play_direction->get_value());
    gameState.reverse_direction();
    EXPECT_EQ(1,gameState._play_direction->get_value());
}

//test set_color
TEST(GameStateTest, set_color)
{
    game_state gameState;
    std::string err;
    //add 2 players
    player p1("ich");
    player p2("du");
    player p3("er");
    gameState.add_player(&p1, err);
    gameState.add_player(&p2, err);
    gameState.add_player(&p3, err);

    card wish(WILDCARD, 4);
    card wish4(DRAW_4_WILDCARD, 4);
    //test normal wildcard
    gameState._discard_pile->_cards.push_back(&wish);
    gameState.set_color(0,err);
    EXPECT_EQ(gameState._discard_pile->get_top_card()->_color->get_value(), 0);
    //did the next player got updated correctly?
    EXPECT_EQ(gameState._current_player_idx->get_value(), 1);
    EXPECT_EQ(gameState._previous_player_idx->get_value(), 0);
    //test wildcard
    gameState._discard_pile->_cards.push_back(&wish4);
    gameState.set_color(2, err);
    //did the next player got skipped correctly?
    EXPECT_EQ(gameState._current_player_idx->get_value(), 0);
    EXPECT_EQ(gameState._previous_player_idx->get_value(), 1);
    //did the next player drew 4 times
    EXPECT_EQ(p3._hand->get_nof_cards(),4);
    //was the color set correctly?
    EXPECT_EQ(gameState._discard_pile->get_top_card()->_color->get_value(), 2);
}

// Test function remove_player
TEST(GameStateTest, remove_player)
{
    game_state gameState;
    std::string err;

    // Add players to the game
    player p1("player1");
    player p2("player2");
    gameState.add_player(&p1, err);
    gameState.add_player(&p2, err);

    bool result = gameState.remove_player(&p1, err);


    // Check if the player is removed successfully
    EXPECT_TRUE(result);
    EXPECT_EQ(gameState._players.size(), 1);

    // Failing attempt
    bool result2 = gameState.remove_player(&p1, err);
    EXPECT_FALSE(result2);
    EXPECT_EQ(gameState._players.size(), 1);
}

// Test function add_player
TEST(GameStateTest, add_player)
{
    game_state gameState;
    std::string err;

    // Add players to the game
    player p1("player1");
    player p2("player2");
    player p3("player3");

    bool result1 = gameState.add_player(&p1, err);
    bool result2 = gameState.add_player(&p2, err);
    bool result3 = gameState.add_player(&p3, err);

    // Check if players are added successfully
    EXPECT_TRUE(result1);
    EXPECT_TRUE(result2);
    EXPECT_TRUE(result3);
    EXPECT_EQ(gameState._players.size(), 3);

    // trying to add same player again
    bool result4 = gameState.add_player(&p3, err);
    EXPECT_FALSE(result4);

    player p4("player4");
    player p5("player5");
    bool result5 = gameState.add_player(&p4, err);
    bool result6 = gameState.add_player(&p5, err);

    EXPECT_TRUE(result5);
    //maximum players reached so next should be false
    EXPECT_FALSE(result6);
    EXPECT_EQ(err,"Could not join game, because the max number of players is already reached.");
}

TEST(GameStateTest, add_player_already_started)
{
    game_state gameState;
    std::string err;

    // Add players to the game
    player p1("player1");
    player p2("player2");
    player p3("player3");

    bool result1 = gameState.add_player(&p1, err);
    bool result2 = gameState.add_player(&p2, err);


    // Check if players are added successfully
    EXPECT_TRUE(result1);
    EXPECT_TRUE(result2);
    EXPECT_EQ(gameState._players.size(), 2);
    gameState.start_game(err);

    // adding a player should fail as game has already started
    bool result3 = gameState.add_player(&p3, err);
    EXPECT_FALSE(result3);

  }

TEST(GameStateTest, draw_card)
{
    game_state gameState;
    std::string err;

    // Add players to the game
    player p1("player1");
    player p2("player2");
    player p3("player3");
    gameState.add_player(&p1, err);
    gameState.add_player(&p2, err);
    gameState.start_game(err);
    // Set up initial conditions
    gameState._current_player_idx->set_value(gameState.get_player_index(&p1));
    EXPECT_EQ(gameState.get_current_player(), &p1);
    bool result = gameState.draw_card(&p1, err);
    bool result2 = gameState.draw_card(&p2, err);
    bool result22 = gameState.draw_card(&p2, err);
    bool result3 = gameState.draw_card(&p3, err);



    //Check if a card is drawn successfully
    EXPECT_TRUE(result);
    EXPECT_EQ(p1._hand->get_nof_cards(), 8);
    // Should work because its players 2 turn
    EXPECT_TRUE(result2);
    // Should fail, because it is player 1s turn
    EXPECT_FALSE(result22);
    // Should fail because p3 not in game
    EXPECT_FALSE(result3);
}

TEST(GameStateTest, start_game)
{
    game_state gameState;
    std::string err;

    // Add players
    player p1("player1");
    player p2("player2");
    player p3("player3");
    gameState.add_player(&p1, err);
    //g

    // Test starting the game when the number of players is less than the minimum required
    EXPECT_FALSE(gameState.start_game(err));
    EXPECT_EQ(err, "You need at least 2 players to start the game.");
    gameState.add_player(&p2, err);
    gameState.add_player(&p3, err);


    // Test starting the game when the game has already started
    EXPECT_TRUE(gameState.start_game(err));
    EXPECT_FALSE(gameState.start_game(err));
    EXPECT_EQ(err, "Could not start game, as the game was already started");

    // Reset game state for a new test
    game_state gameState1;

    // Check if the initial round setup is performed
    EXPECT_EQ(gameState._round_number->get_value(), 1);
    EXPECT_FALSE(gameState._draw_pile->is_empty());
    EXPECT_FALSE(gameState._discard_pile->_cards.empty());

    // Check if each player has drawn 7 cards
    for (const auto& player : gameState._players)
    {
        EXPECT_EQ(player->_hand->get_nof_cards(), 7);
    }
}

TEST(GameStateTest, setup_round)
{
    game_state gameState;
    std::string err;

    // Add players
    player p1("player1");
    player p2("player2");
    player p3("player3");
    player p4("player4");
    gameState.add_player(&p1, err);
    gameState.add_player(&p2, err);
    gameState.add_player(&p3, err);
    gameState.add_player(&p4, err);

    // Start the game
    EXPECT_TRUE(gameState.start_game(err));

    // Setup a round
    gameState.setup_round(err);

    // Check if round setup is correct
    // there should be 80 cards in the game when it starts
    EXPECT_EQ(gameState._discard_pile->_cards.size() + gameState._draw_pile->_cards.size(), 80); 

    // Check initial values
    EXPECT_EQ(gameState._current_player_idx->get_value(), 0); // player1 should be the starting player

    // Check each player's hand size
    for (int i = 0; i < 4; ++i)
    {
            EXPECT_EQ(gameState._players[i]->get_nof_cards(), 7); // players should have 7 cards
    }

    // there should be 80 cards in the game when it starts
    EXPECT_EQ(gameState._discard_pile->_cards.size() + gameState._draw_pile->_cards.size(), 80); 

    // check the play direction
    EXPECT_EQ(gameState._play_direction->get_value(), 1); // Play direction should be forward initially
}

TEST(GameStateTest, update_current_player)
{
    game_state gameState;
    std::string err;

    // Add players
    player p1("player1");
    player p2("player2");
    player p3("player3");
    player p4("player4");
    gameState.add_player(&p1, err);
    gameState.add_player(&p2, err);
    gameState.add_player(&p3, err);
    gameState.add_player(&p4, err);

    EXPECT_TRUE(gameState.start_game(err));

    //setup a round
    gameState.setup_round(err);

    //test case 1: Playing in the forward direction
    //current player: player1, play direction: 1 (forward)
    gameState._current_player_idx->set_value(0);
    gameState._play_direction->set_value(1);

    //execute update_current_player
    gameState.update_current_player(err);

    //check if the current player is updated correctly
    EXPECT_EQ(gameState._current_player_idx->get_value(), 1); // Next player is player2
    //check if the previous player is updated correctly
    EXPECT_EQ(gameState._previous_player_idx->get_value(), 0); // Previous player was player1

    //test case 2: Playing in the reverse direction
    //current player: player2, play direction: -1 (reverse)
    gameState._current_player_idx->set_value(1);
    gameState._play_direction->set_value(-1);

    //execute update_current_player
    gameState.update_current_player(err);

    //check if the current player is updated correctly
    EXPECT_EQ(gameState._current_player_idx->get_value(), 0); // Next player is player1
    //check if the previous player is updated correctly
    EXPECT_EQ(gameState._previous_player_idx->get_value(), 1); // Previous player was player2

    //test case 3: Playing in the forward direction with multiple players
    //current player: player3, play direction: 1 (forward)
    gameState._current_player_idx->set_value(2);
    gameState._play_direction->set_value(1);

    //execute update_current_player
    gameState.update_current_player(err);

    //check if the current player is updated correctly
    EXPECT_EQ(gameState._current_player_idx->get_value(), 3); // Next player is player4
    //check if the previous player is updated correctly
    EXPECT_EQ(gameState._previous_player_idx->get_value(), 2); // Previous player was player3

    //test case 4: Playing in the reverse direction with multiple players
    //current player: player4, play direction: -1 (reverse)
    gameState._current_player_idx->set_value(3);
    gameState._play_direction->set_value(-1);

    // Execute update_current_player
    gameState.update_current_player(err);

    //check if the current player is updated correctly
    EXPECT_EQ(gameState._current_player_idx->get_value(), 2); // Next player is player3
    //check if the previous player is updated correctly
    EXPECT_EQ(gameState._previous_player_idx->get_value(), 3); // Previous player was player4
}

// Test edge cases previous
TEST(GameStateTest, get_previous_player)
{
    // Create game state
    game_state gameState;

    // Test case: Get previous player (initially null)
    player* prevPlayer = gameState.get_previous_player();

    // Check if the previous player is null
    EXPECT_EQ(prevPlayer, nullptr);
}

// Test edge cases get_current_player
TEST(GameStateTest, get_current_player)
{
    game_state gameState;

    player* currPlayer = gameState.get_current_player();

    EXPECT_EQ(currPlayer, nullptr);
}


TEST(GameStateTest, get_player_index)
{
    game_state gameState;
    std::string err;

    player p1("player1");
    player p2("player2");
    player p3("player3");
    player p4("player4");
    gameState.add_player(&p1, err);
    gameState.add_player(&p2, err);
    gameState.add_player(&p3, err);
    gameState.add_player(&p4, err);

    // Test case 1: Player index when player is in the game
    EXPECT_EQ(gameState.get_player_index(&p3), 2);

    // case 2: Player index when player is not in the game
    player p5("player5");
    EXPECT_EQ(gameState.get_player_index(&p5), -1);
}


TEST(GameStateTest, is_player_in_game)
{
    game_state gameState;
    std::string err;

    player p1("player1");
    player p2("player2");
    player p3("player3");
    player p4("player4");
    gameState.add_player(&p1, err);
    gameState.add_player(&p2, err);
    gameState.add_player(&p3, err);
    gameState.add_player(&p4, err);

    // Test case 1: Player is in the game
    EXPECT_TRUE(gameState.is_player_in_game(&p2));

    //case 2: Player is not in the game
    player p5("player5");
    EXPECT_FALSE(gameState.is_player_in_game(&p5));
}


TEST(GameStateTest, is_allowed_to_play_now)
{
    game_state gameState;
    std::string err;

    player p1("player1");
    player p2("player2");
    player p3("player3");
    player p4("player4");
    gameState.add_player(&p1, err);
    gameState.add_player(&p2, err);
    gameState.add_player(&p3, err);
    gameState.add_player(&p4, err);

    gameState.start_game(err);

    // Test case 1: Allowed to play now (current player's turn)
    EXPECT_TRUE(gameState.is_allowed_to_play_now(&p1));

    //case 2: Not allowed to play now (not current player's turn)
    EXPECT_FALSE(gameState.is_allowed_to_play_now(&p2));
}

TEST(GameStateTest, get_players)
{
    game_state gameState;
    std::string err;

    player p1("player1");
    player p2("player2");
    player p3("player3");
    player p4("player4");
    gameState.add_player(&p1, err);
    gameState.add_player(&p2, err);
    gameState.add_player(&p3, err);
    gameState.add_player(&p4, err);
    std::vector<player*> players = gameState.get_players();

    // Cvector is not empty
    EXPECT_FALSE(players.empty());
    // players vector is correct
    EXPECT_EQ(players.size(), 4);
    //players vector contains the correct players
    EXPECT_EQ(players[0], &p1);
    EXPECT_EQ(players[1], &p2);
    EXPECT_EQ(players[2], &p3);
    EXPECT_EQ(players[3], &p4);
}

TEST(GameStateTest, is_full)
{
    // Create game state
    game_state gameState;
    std::string err;

    // Add players
    player p1("player1");
    player p2("player2");
    player p3("player3");
    player p4("player4");
    gameState.add_player(&p1, err);
    gameState.add_player(&p2, err);
    gameState.add_player(&p3, err);

    EXPECT_FALSE(gameState.is_full());

    gameState.add_player(&p4, err);
    EXPECT_TRUE(gameState.is_full());
}

TEST(GameStateTest, is_started)
{

    game_state gameState;
    std::string err;

    EXPECT_FALSE(gameState.is_started());

    player p1("player1");
    player p2("player2");
    gameState.add_player(&p1, err);
    gameState.add_player(&p2, err);
    gameState.start_game(err);

    EXPECT_TRUE(gameState.is_started());
}