//
// Created by UNOrganized on 11.12.2023.
//

#include "gtest/gtest.h"

#define private public

#include "../src/server/request_handler.h"
#include "../src/server/player_manager.h"
#include "../src/server/game_instance_manager.h"
#include "../src/server/game_instance.h"
#include "../src/server/server_network_manager.h"
#include "../src/common/network/requests/join_game_request.h"
#include "../src/common/network/requests/start_game_request.h"
#include "../src/common/network/requests/new_round_request.h"
#include "../src/common/network/requests/draw_card_request.h"
#include "../src/common/network/requests/play_card_request.h"
#include "../src/common/network/requests/call_uno_request.h"
#include "../src/common/network/requests/pick_a_color_request.h"
#include "../src/common/network/requests/player_left_request.h"
#include "../src/common/game_state/game_state.h"

//let's test function handle_request(client_request* req)
class ReqHandlerTest : public ::testing::Test {
    protected:
        void SetUp() override{
            player1 = new player("TestPlayer");
            player_id = player1->get_id();
            game_instance_ptr = game_instance_manager::create_new_game();
            game_id = game_instance_ptr->get_id();
        }

        void TearDown() override {
            delete player1;
            delete game_instance_ptr;
        }

        player* player1;
        game_instance* game_instance_ptr;
        std::string player_id;
        std::string game_id;
        std::string err;

};

TEST_F(ReqHandlerTest, JoinGameTest) {
    //add player to server
    bool couldAddPlayer = player_manager::add_or_get_player("TestPlayer", player_id, player1);

    //make a request and create response
    join_game_request* rec = new join_game_request(game_id, player_id, "TestPlayer", true);
    std::string req_id = rec->get_req_id();
    request_response* res = request_handler::handle_request(rec);

    //check if request ist handled correctly
    player* firstPlayer = game_instance_ptr->get_game_state()->get_players().at(0);
    EXPECT_EQ(firstPlayer->get_id(), player1->get_id());
    EXPECT_EQ(player1->get_game_id(), game_instance_ptr->get_id());
    
    //check response as good as possible without getting into rapidjson
    request_response* expected = new request_response(game_id, req_id, true,
                                                game_instance_ptr->get_game_state()->to_json(), err);
    EXPECT_EQ(res->get_success(), expected->get_success());
    EXPECT_EQ(res->get_error(), expected->get_error());
    EXPECT_EQ(res->get_req_id(), expected->get_req_id());

    //check if adding player to same game again doesn't work
    rec = new join_game_request(game_id, player_id, "TestPlayer", true);
    req_id = rec->get_req_id();
    res = request_handler::handle_request(rec);
    EXPECT_FALSE(res->get_success());
    EXPECT_EQ(res->get_error(), "Player is already active in this src");

    //check if adding player to any game again doesn't work
    rec = new join_game_request("", player_id, "TestPlayer", true);
    req_id = rec->get_req_id();
    res = request_handler::handle_request(rec);
    EXPECT_FALSE(res->get_success());
    EXPECT_EQ(res->get_error(), "Could not join game. Player is already active in a game");

    //check if it acts correctly if there is no game with this game_id
    rec = new join_game_request("1", player_id, "TestPlayer", true);
    req_id = rec->get_req_id();
    res = request_handler::handle_request(rec);
    EXPECT_FALSE(res->get_success());
    EXPECT_EQ(res->get_error(), "Requested game could not be found.");
}


TEST_F(ReqHandlerTest, CallUnoTest) {
    //add player and game to server and add player to game
    bool couldAddPlayer = player_manager::add_or_get_player("TestPlayer", player_id, player1);
    bool couldJoinGame = game_instance_manager::try_add_player(player1, game_instance_ptr, err);

    //create call uno request and response
    call_uno_request* rec = new call_uno_request(game_id, player_id);
    std::string req_id = rec->get_req_id();
    request_response* res = request_handler::handle_request(rec);

    //check if request ist handled correctly
    EXPECT_TRUE(game_instance_ptr->get_game_state()->get_current_player()->has_called_uno());
    
    //check response as good as possible without getting into rapidjson
    request_response* expected = new request_response(game_id, req_id, true, game_instance_ptr->get_game_state()->to_json(), err);
    EXPECT_EQ(res->get_success(), expected->get_success());
    EXPECT_EQ(res->get_error(), expected->get_error());
    EXPECT_EQ(res->get_req_id(), expected->get_req_id());

    //check if calling uno again gives error
    rec = new call_uno_request(game_id, player_id);
    req_id = rec->get_req_id();
    res = request_handler::handle_request(rec);
    EXPECT_FALSE(res->get_success());
    EXPECT_EQ(res->get_error(), "You already called uno.");

    //check if it doesn't know an unknown player
    //(so check try_get_player_and_game_instance)
    player* player2 = new player("unknownPlayer");
    rec = new call_uno_request(game_id, player2->get_id());
    req_id = rec->get_req_id();
    res = request_handler::handle_request(rec);
    EXPECT_FALSE(res->get_success());
    EXPECT_EQ(res->get_error(), "Could not find requested player " + player2->get_id() + " in database.");
}

TEST_F(ReqHandlerTest, StartGameTest) {
    //add player and game to server and add player to game
    bool couldAddPlayer = player_manager::add_or_get_player("TestPlayer", player_id, player1);
    bool couldJoinGame = game_instance_manager::try_add_player(player1, game_instance_ptr, err);

    //test if it realized it has not enough players yet
    start_game_request* rec = new start_game_request(game_id, player_id, 1);
    std::string req_id = rec->get_req_id();
    request_response* res = request_handler::handle_request(rec);
    EXPECT_FALSE(res->get_success());
    EXPECT_EQ(res->get_error(), "You need at least 2 players to start the game.");

    //add a second player to the game
    player* player2 = new player("TestPlayer2");
    std::string player2_id = player2->get_id();
    bool couldAddPlayerToGame = game_instance_ptr->get_game_state()->add_player(player2, err);
    player2->set_game_id(game_instance_ptr->get_id());

    //test if start game worked
    rec = new start_game_request(game_id, player_id, 1);
    req_id = rec->get_req_id();
    res = request_handler::handle_request(rec);
    EXPECT_TRUE(game_instance_ptr->get_game_state()->get_play_with_questionmark());
    EXPECT_TRUE(game_instance_ptr->get_game_state()->is_started());
    
    //check response as good as possible without getting into rapidjson
    request_response* expected = new request_response(game_id, req_id, true, game_instance_ptr->get_game_state()->to_json(), err);
    EXPECT_EQ(res->get_success(), expected->get_success());
    EXPECT_EQ(res->get_error(), expected->get_error());
    EXPECT_EQ(res->get_req_id(), expected->get_req_id());

    //test if game is already started
    rec = new start_game_request(game_id, player_id, 1);
    req_id = rec->get_req_id();
    res = request_handler::handle_request(rec);
    EXPECT_FALSE(res->get_success());
    EXPECT_EQ(res->get_error(), "Could not start game, as the game was already started");
}


TEST_F(ReqHandlerTest, NewRoundTest) {
    //add player and game to server and add player to game
    bool couldAddPlayer = player_manager::add_or_get_player("TestPlayer", player_id, player1);
    bool couldJoinGame = game_instance_manager::try_add_player(player1, game_instance_ptr, err);
    
    //create new round request and response
    new_round_request* rec = new new_round_request(game_id, player_id);
    std::string req_id = rec->get_req_id();
    request_response* res = request_handler::handle_request(rec);

    //check if request ist handled correctly
    EXPECT_EQ(game_instance_ptr->get_game_state()->get_players_in_new_round(), 1);
    
    //check response as good as possible without getting into rapidjson
    request_response* expected = new request_response(game_id, req_id, true, game_instance_ptr->get_game_state()->to_json(), err);
    EXPECT_EQ(res->get_success(), expected->get_success());
    EXPECT_EQ(res->get_error(), expected->get_error());
    EXPECT_EQ(res->get_req_id(), expected->get_req_id());
}

TEST_F(ReqHandlerTest, PlayCardTest){
    //add player and game to server and add player to game
    bool couldAddPlayer = player_manager::add_or_get_player("TestPlayer", player_id, player1);
    bool couldJoinGame = game_instance_manager::try_add_player(player1, game_instance_ptr, err);
    
    //add card to players hand
    card* testcard = new card(0,0);
    player1->add_card(testcard, err);

    //create request and response
    play_card_request* rec = new play_card_request(game_id, player_id, testcard->get_id());
    std::string req_id = rec->get_req_id();
    request_response* res = request_handler::handle_request(rec);

    //check if request is handled correctly
    EXPECT_EQ(game_instance_ptr->get_game_state()->get_discard_pile()->get_top_card(), testcard);
    EXPECT_TRUE(game_instance_ptr->get_game_state()->get_missed_uno_call());
    EXPECT_EQ(player1->get_nof_cards(), 2);
    
    //add the card again and now call uno
    player1->add_card(testcard, err);
    player1->set_called_uno(true, err);

    //create request and response and expected response
    rec = new play_card_request(game_id, player_id, testcard->get_id());
    req_id = rec->get_req_id();
    res = request_handler::handle_request(rec);

    //check if request ist handled correctly
    EXPECT_EQ(game_instance_ptr->get_game_state()->get_discard_pile()->get_top_card(), testcard);
    EXPECT_FALSE(game_instance_ptr->get_game_state()->get_missed_uno_call());
    EXPECT_EQ(player1->get_nof_cards(), 2);
    
    //check response as good as possible without getting into rapidjson
    request_response* expected = new request_response(game_id, req_id, true, game_instance_ptr->get_game_state()->to_json(), err);
    EXPECT_EQ(res->get_success(), expected->get_success());
    EXPECT_EQ(res->get_error(), expected->get_error());
    EXPECT_EQ(res->get_req_id(), expected->get_req_id());
    
}


TEST_F(ReqHandlerTest, DrawCardTest){
    //add player and game to server and add player to game
    bool couldAddPlayer = player_manager::add_or_get_player("TestPlayer", player_id, player1);
    bool couldJoinGame = game_instance_manager::try_add_player(player1, game_instance_ptr, err);

    //add cards to draw pile
    draw_pile* draw_pile = game_instance_ptr->get_game_state()->get_draw_pile();
    draw_pile->setup_game(err);
    card* testcard = draw_pile->get_top_card();

    //create request and response
    draw_card_request* rec = new draw_card_request(game_id, player_id);
    std::string req_id = rec->get_req_id();
    request_response* res = request_handler::handle_request(rec);

    //check if request is handled correctly
    EXPECT_EQ(player1->get_hand()->get_cards().at(0), testcard);
    EXPECT_FALSE(game_instance_ptr->get_game_state()->get_missed_uno_call());
    EXPECT_EQ(player1->get_nof_cards(), 1);
    
    //change the testcard to the new top card
    testcard = draw_pile->get_top_card();

    //create request and response and expected response
    rec = new draw_card_request(game_id, player_id);
    req_id = rec->get_req_id();
    res = request_handler::handle_request(rec);

    //check if request ist handled correctly
    EXPECT_EQ(player1->get_hand()->get_cards().at(1), testcard);
    EXPECT_TRUE(game_instance_ptr->get_game_state()->get_missed_uno_call());
    EXPECT_EQ(player1->get_nof_cards(), 4);
    
    //check response as good as possible without getting into rapidjson
    request_response* expected = new request_response(game_id, req_id, true, game_instance_ptr->get_game_state()->to_json(), err);
    EXPECT_EQ(res->get_success(), expected->get_success());
    EXPECT_EQ(res->get_error(), expected->get_error());
    EXPECT_EQ(res->get_req_id(), expected->get_req_id());
    
}

TEST_F(ReqHandlerTest, PickaColorTest){
    //add player and game to server and add player to game
    bool couldAddPlayer = player_manager::add_or_get_player("TestPlayer", player_id, player1);
    bool couldJoinGame = game_instance_manager::try_add_player(player1, game_instance_ptr, err);
    player* player_before_request = player1;
    
    //add wildcard on top of discard pile
    card* wildcard = new card(13,4);
    game_instance_ptr->get_game_state()->get_discard_pile()->try_play(wildcard, err);

    //create request and response and expected response
    pick_a_color_request* rec = new pick_a_color_request(game_id, player_id, 3);
    std::string req_id = rec->get_req_id();
    request_response* res = request_handler::handle_request(rec);

    //check if request is handled correctly
    EXPECT_EQ(game_instance_ptr->get_game_state()->get_discard_pile()->get_top_card()->get_color(), 3);
    EXPECT_EQ(game_instance_ptr->get_game_state()->get_discard_pile()->get_top_card()->get_index(), 13);
    EXPECT_EQ(player_before_request->get_id(), game_instance_ptr->get_game_state()->get_previous_player()->get_id());

    //check response as good as possible without getting into rapidjson
    request_response* expected = new request_response(game_id, req_id, true, game_instance_ptr->get_game_state()->to_json(), err);
    EXPECT_EQ(res->get_success(), expected->get_success());
    EXPECT_EQ(res->get_error(), expected->get_error());
    EXPECT_EQ(res->get_req_id(), expected->get_req_id());
}

TEST_F(ReqHandlerTest, PlayerLeftTest) {
    //add player and game to server and add player to game
    bool couldAddPlayer = player_manager::add_or_get_player("TestPlayer", player_id, player1);
    bool couldJoinGame = game_instance_manager::try_add_player(player1, game_instance_ptr, err);

    //add a second player to the game
    player* player2 = new player("TestPlayer2");
    std::string player2_id = player2->get_id();
    bool couldAddPlayerToManager = player_manager::add_or_get_player("TestPlayer2", player2_id, player2);
    bool couldAddPlayerToGame = game_instance_ptr->get_game_state()->add_player(player2, err);
    player2->set_game_id(game_instance_ptr->get_id());

    //create request
    player_left_request* rec = new player_left_request(game_id, player_id);
    std::string req_id = rec->get_req_id();

    //check if it is handled correctly
    request_response* res = request_handler::handle_request(rec);
    EXPECT_TRUE(res->get_success());
    EXPECT_EQ(game_instance_ptr->get_game_state()->get_players().size(), 1);
    EXPECT_TRUE(game_instance_ptr->get_game_state()->is_finished());

    //create new request trying to remove same player again
    rec = new player_left_request(game_id, player_id);
    req_id = rec->get_req_id();

    //check if it is handled correctly
    res = request_handler::handle_request(rec);
    EXPECT_FALSE(res->get_success());
    EXPECT_EQ(res->get_error(), "Could not find game_id associated with this player");
    EXPECT_EQ(game_instance_ptr->get_game_state()->get_players().size(), 1);

    //create request trying to remove other player
    game_instance_ptr->get_game_state()->_is_finished->set_value(0);
    rec = new player_left_request(game_id, player2_id);
    req_id = rec->get_req_id();

    //check if it is handled correctly
    res = request_handler::handle_request(rec);
    EXPECT_TRUE(res->get_success());
    EXPECT_EQ(game_instance_ptr->get_game_state()->get_players().size(), 0);
    EXPECT_FALSE(game_instance_ptr->get_game_state()->is_finished());
    
    //check response as good as possible without getting into rapidjson
    request_response* expected = new request_response(game_id, req_id, true, game_instance_ptr->get_game_state()->to_json(), err);
    EXPECT_EQ(res->get_success(), expected->get_success());
    EXPECT_EQ(res->get_error(), expected->get_error());
    EXPECT_EQ(res->get_req_id(), expected->get_req_id());
}