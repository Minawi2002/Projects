//
// Created by Manuel on 25.01.2021.
// Updates by UNOrganized on 11.12.2023.
//
// The game_instance class is a wrapper around the game_state of an active instance of the game.
// This class contains functions to modify the contained game_state.

#ifndef UNO_GAME_H
#define UNO_GAME_H

#include <vector>
#include <string>
#include <mutex>

#include "../common/game_state/player/player.h"
#include "../common/game_state/game_state.h"

class game_instance {

private:
    game_state* _game_state;
    inline static std::mutex modification_lock;

    /**
    * \brief checks if the given player is allowed to play
    * \param: pointer to the player
    * \result returns if the given player is allowed to play
    */
    bool is_player_allowed_to_play(player* player);

public:
    /**
    * \brief constructor of game instance
    * \result game instance is constructed
    */
    game_instance();

    /**
    * \brief destructor of game instance 
    * \result game instance is destructed
    */
    ~game_instance() {
        if (_game_state != nullptr) {
            delete _game_state;
        }
        _game_state = nullptr;
    }

    // ** accessors **//
    /**
    * \brief get the game id
    * \result returns the game id
    */
    std::string get_id();

    /**
    * \brief get the game state pointer
    * \result returns the game state pointer
    */
    game_state* get_game_state();

    /**
    * \brief get if the game is full
    * \result returns if the game is full
    */
    bool is_full();

    /**
    * \brief get if the game has already started
    * \result returns if the game has already started
    */
    bool is_started();

    /**
    * \brief get if the game is already finished 
    * \result returns if the game is already finished
    */
    bool is_finished();

    //** game update functions **//
    /**
    * \brief tries to start the game
    * \param: pointer to the player and an error string
    * \result true if starting the game worked, false otherwise
    */
    bool start_game(player* player, std::string& err);

    /**
    * \brief tries to start a new round of the game
    * \param: pointer to the player and an error string
    * \result true if starting a new round of the game worked, false otherwise
    */
    bool new_round(player* player, std::string& err);

    /**
    * \brief tries to add a player to the game
    * \param: pointer to the player and an error string
    * \result true if adding the player the game worked, false otherwise
    */
    bool try_add_player(player* new_player, std::string& err);

    /**
    * \brief tries to remove a player to the game
    * \param: pointer to the player and an error string
    * \result true if removing the player the game worked, false otherwise
    */
    bool try_remove_player(player* player, std::string& err);

    /**
    * \brief tries to play a card of the player in the game
    * \param: pointer to the player, the card id and an error string
    * \result true if playing the card worked, false otherwise
    */
    bool play_card(player* player, const std::string& card_id, std::string& err);

    /**
    * \brief tries to draw a card for a player in the game
    * \param: pointer to the player, pointer for the card (should point to the drawn card after the function is executed) and an error string
    * \result true if drawing the card worked, false otherwise
    */
    bool draw_card(player* player, card*& drawn_card, std::string& err);

    /**
    * \brief tries to change the color of the wildcard on top of the games discard pile
    * \param: pointer to the player, the color index and an error string
    * \result true if changing the color of the card worked, false otherwise
    */
    bool set_color_wildcard(player* player, int color, std::string& err);

    /**
    * \brief tries to set uno to a player in the game
    * \param: pointer to the player and an error string
    * \result true if calling uno worked, false otherwise
    */
    bool set_uno(player* player, std::string& err);

};


#endif //UNO_GAME_H

