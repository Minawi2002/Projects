//
// Created by Manuel on 08.02.2022.
// Updates by UNOrganized on 11.12.2023.
//

#ifndef UNO_REQUEST_HANDLER_CPP
#define UNO_REQUEST_HANDLER_CPP

#include "request_handler.h"

#include "player_manager.h"
#include "game_instance_manager.h"
#include "game_instance.h"
#include "server_network_manager.h"

#include "../common/network/requests/join_game_request.h"
#include "../common/network/requests/start_game_request.h"
#include "../common/network/requests/new_round_request.h"
#include "../common/network/requests/draw_card_request.h"
#include "../common/network/requests/play_card_request.h"
#include "../common/network/requests/call_uno_request.h"
#include "../common/network/requests/pick_a_color_request.h"
#include "../common/network/requests/player_left_request.h"
#include "../common/game_state/game_state.h"


request_response* request_handler::handle_request(const client_request* const req) {

    // Prepare variables that are used by every request type
    player* player;
    std::string err;
    game_instance* game_instance_ptr = nullptr;

    // Get common properties of requests
    RequestType type = req->get_type();
    std::string req_id = req->get_req_id();
    std::string game_id = req->get_game_id();
    std::string player_id = req->get_player_id();


    // Switch behavior according to request type
    switch(type) {

        // ##################### JOIN GAME #####################  //
        case RequestType::join_game: {
            std::string player_name = ((join_game_request *) req)->get_player_name();

            // Create new player or get existing one with that name
            player_manager::add_or_get_player(player_name, player_id, player);

            bool first_game_of_player = ((join_game_request*) req)->get_first_game_of_player();
            if(!first_game_of_player){
                player->set_game_id("");
                player->set_score(0);
            }

            if (game_id.empty()) {
                // join any game
                if (game_instance_manager::try_add_player_to_any_game(player, game_instance_ptr, err)) {
                    // game_instance_ptr got updated to the joined game

                    // return response with full game_state attached
                    return new request_response(game_instance_ptr->get_id(), req_id, true,
                                                game_instance_ptr->get_game_state()->to_json(), err);
                } else {
                    // failed to find game to join
                    return new request_response("", req_id, false, nullptr, err);
                }
            } else {
                // join a specific game denoted by req->get_game_id()
                if (game_instance_manager::try_get_game_instance(game_id, game_instance_ptr)) {
                    if (game_instance_manager::try_add_player(player, game_instance_ptr, err)) {
                        // return response with full game_state attached
                        return new request_response(game_id, req_id, true,
                                                    game_instance_ptr->get_game_state()->to_json(), err);
                    } else {
                        // failed to join requested game
                        return new request_response("", req_id, false, nullptr, err);
                    }
                } else {
                    // failed to find requested game
                    return new request_response("", req_id, false, nullptr, "Requested game could not be found.");
                }
            }
        }


        // ##################### START GAME ##################### //
        case RequestType::start_game: {
            if (game_instance_manager::try_get_player_and_game_instance(player_id, player, game_instance_ptr, err)) {
                bool questionmarkbool = ((start_game_request*) req)->get_questionmark();
                if(game_instance_ptr->get_game_state()->set_play_with_questionmark(questionmarkbool)){
                    if (game_instance_ptr->start_game(player, err)) {
                        return new request_response(game_instance_ptr->get_id(), req_id, true,
                                                    game_instance_ptr->get_game_state()->to_json(), err);
                    }
                }
            }
            return new request_response("", req_id, false, nullptr, err);
        }

        // ##################### NEW ROUND ##################### //
        case RequestType::new_round: {
            if (game_instance_manager::try_get_player_and_game_instance(player_id, player, game_instance_ptr, err)) {
                if (game_instance_ptr->new_round(player, err)) {
                    return new request_response(game_instance_ptr->get_id(), req_id, true,
                                                game_instance_ptr->get_game_state()->to_json(), err);
                }
            }
            return new request_response("", req_id, false, nullptr, err);
        }


        // ##################### PLAY CARD ##################### //
        case RequestType::play_card: {
            if (game_instance_manager::try_get_player_and_game_instance(player_id, player, game_instance_ptr, err)) {
                card *drawn_card;
                std::string card_id = ((play_card_request *) req)->get_card_id();
                if (game_instance_ptr->play_card(player, card_id, err)) {
                    return new request_response(game_instance_ptr->get_id(), req_id, true,
                                                game_instance_ptr->get_game_state()->to_json(), err);
                }
            }
            return new request_response("", req_id, false, nullptr, err);
        }


        // ##################### DRAW CARD ##################### //
        case RequestType:: draw_card: {
            if (game_instance_manager::try_get_player_and_game_instance(player_id, player, game_instance_ptr, err)) {
                card *drawn_card;
                if (game_instance_ptr->draw_card(player, drawn_card, err)) {
                    return new request_response(game_instance_ptr->get_id(), req_id, true,
                                                game_instance_ptr->get_game_state()->to_json(), err);
                }
            }
            return new request_response("", req_id, false, nullptr, err);
        }


        // ################### CALL UNO #################### //
        case RequestType::call_uno: {
            if (game_instance_manager::try_get_player_and_game_instance(player_id, player, game_instance_ptr, err)) {
                if(game_instance_ptr->set_uno(player, err)){ 
                    return new request_response(game_instance_ptr->get_id(), req_id, true,
                                                game_instance_ptr->get_game_state()->to_json(), err);

                }
            }
            return new request_response("", req_id, false, nullptr, err);


        }

        //#################### PICK A COLOR ################ //
        case RequestType::pick_a_color: {
            if (game_instance_manager::try_get_player_and_game_instance(player_id, player, game_instance_ptr, err)) {
                int color = ((pick_a_color_request*)req)->get_color();
                if(game_instance_ptr->set_color_wildcard(player, color, err)){
                     return new request_response(game_instance_ptr->get_id(), req_id, true,
                                                game_instance_ptr->get_game_state()->to_json(), err);      
                }
            }
            return new request_response("", req_id, false, nullptr, err);
        }

        // ##################### PLAYER LEFT ##################### //
        case RequestType::player_left: {
            if (game_instance_manager::try_get_player_and_game_instance(player_id, player, game_instance_ptr, err)) {
                    if (game_instance_ptr->try_remove_player(player, err)) {
                        server_network_manager::on_player_left(player_id);
                        return new request_response(game_instance_ptr->get_id(), req_id, true,
                                                    game_instance_ptr->get_game_state()->to_json(), err);
                    }
            }
            return new request_response("", req_id, false, nullptr, err);
        }


        // ##################### UNKNOWN REQUEST ##################### //
        default:
            return new request_response("", req_id, false, nullptr, "Unknown RequestType " + type);
    }
}

#endif //UNO_REQUEST_HANDLER_CPP
