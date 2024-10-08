//
// Created by Manuel on 29.01.2021.
//
// The game_instance_manager only exists on the server side. It stores all currently active games and offers
// functionality to retrieve game instances by id and adding players to games.
// If a new player requests to join a game but no valid game_instance is available, then this class
// will generate a new game_instance and add it to the unordered_map of (active) game instances.

#ifndef UNO_GAME_INSTANCE_MANAGER_H
#define UNO_GAME_INSTANCE_MANAGER_H

#include <string>
#include <shared_mutex>
#include <unordered_map>

#include "game_instance.h"

class game_instance_manager {

private:

    inline static std::shared_mutex games_lut_lock;
    static std::unordered_map<std::string, game_instance*> games_lut;

    /**
    * \brief creates new game instance
    * \result returns new game instance
    */
    static game_instance* create_new_game();

    /**
    * \brief tries to find a non-full, non-started game, else creates new game and removes finished games
    * \result returns a joinable game instance and removed finished games
    */
    static game_instance* find_joinable_game_instance();

public:
    /**
    * \brief tries to load the game instance of the game with game id to the given pointer
    * \param:  desired game id and a ptr that should point to the game instance after executing the function
    * \result true if found the game instance (else false), pointer points on the game instance (else nullptr)
    */
    static bool try_get_game_instance(const std::string& game_id, game_instance*& game_instance_ptr);
    
    /**
    * \brief tries to load the game instance and player of the game the player already joined to the given pointers
    * \param:  desired player id and a two pointers (that should point to the player and game instance after executing the function) and an error string
    * \result returns true if the desired player 'player_id' was found and is connected to a game_instance (else false). pointers point to player and game instance (else nullptrs)
    */
    static bool try_get_player_and_game_instance(const std::string& player_id, player*& player, game_instance*& game_instance_ptr, std::string& err);

    /**
    * \brief tries to load the game instance and player of any game to the given pointers
    * \param:  two pointers (that should point to the player and game instance after executing the function) and an error string
    * \result returns true if the desired player 'player_id' was found and is connected to a game_instance (else false). pointers point to player and game instance (else nullptrs)
    */
    static bool try_add_player_to_any_game(player* player, game_instance*& game_instance_ptr, std::string& err);

   /**
    * \brief Try to add 'player' to the provided 'game_instance_ptr'.
    * \param:  player pointer, game instance pointer and an error string
    * \result Adds 'player' to the provided 'game_instance_ptr'. Returns true if success and false otherwise.
    */
    static bool try_add_player(player* player, game_instance*& game_instance_ptr, std::string& err);

    /**
    * \brief finds the game instance and tries to remove the player from the game instance
    * \param:  player pointer, game id and an error string
    * \result Removes 'player' of the provided game. Returns true if success and false otherwise.
    */
    static bool try_remove_player(player* player, const std::string& game_id, std::string& err);
    
    /**
    * \brief calls try remove player from given game instance, tries to remove the player from the game instance
    * \param:  player pointer, game instance pointer and an error string
    * \result Removes 'player' of the provided 'game_instance_ptr'. Returns true if success and false otherwise.
    */
    static bool try_remove_player(player* player, game_instance*& game_instance_ptr, std::string& err);

};


#endif //UNO_GAME_INSTANCE_MANAGER_H
