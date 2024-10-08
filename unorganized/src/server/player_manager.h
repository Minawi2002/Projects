//
// Created by Manuel on 29.01.2021.
//
// The player_manager only exists on the server side. It stores all connected users since starting the server. It offers
// functionality to retrieve players by id or adding players when they first connect to the server.
//

#ifndef UNO_PLAYER_MANAGER_H
#define UNO_PLAYER_MANAGER_H

#include <string>
#include <shared_mutex>
#include <unordered_map>

#include "../common/game_state/player/player.h"

class player_manager {

private:

    inline static std::shared_mutex _rw_lock;
    static std::unordered_map<std::string, player*> _players_lut;

public:
    /**
    * \brief tries to get the desired player
    * \param: player id and a pointer to a player (should point to the player after the execution of the function)
    * \result true if it found the player, false otherwise
    */
    static bool try_get_player(const std::string& player_id, player*& player_ptr);

    /**
    * \brief gets the player if it exists, else inserts player
    * \param: name and id of player and a pointer to a player (should point to the player after the execution of the function)
    * \result pointer points to player, returns true if playing the card worked, false otherwise
    */
    static bool add_or_get_player(std::string name, const std::string& player_id, player*& player_ptr);
    
    /**
    * \brief tries to remove the desired player
    * \param: player id and a pointer to a player
    * \result true if it could remove the player, false otherwise
    */
    static bool remove_player(const std::string& player_id, player*& player); 
};


#endif //UNO_PLAYER_MANAGER_H
