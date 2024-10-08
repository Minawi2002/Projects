//
// Created by Manuel on 29.01.2021.
// Updates by UNOrganized on 11.12.2023.
//

#ifndef UNO_JOIN_GAME_REQUEST_H
#define UNO_JOIN_GAME_REQUEST_H


#include <string>
#include "client_request.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class join_game_request : public client_request{

private:
    std::string _player_name;
    bool _first_game_of_player;

    static std::string undefined_game_id;
    
    /**
    * \brief private constructor for deserialization
    * \param: all base class properties, player name and a boolean if its the players first game
    * \result join game request is constructed
    */
    join_game_request(base_class_properties, std::string name, bool first_game_of_player);

public:

    /**
    * \brief get the players name
    * \result returns the players name
    */
    [[nodiscard]] std::string get_player_name() const { return this->_player_name; }

    /**
    * \brief get if its the players first game
    * \result returns boolean if its the players first game
    */
    [[nodiscard]] bool get_first_game_of_player() const {return this -> _first_game_of_player;}
    
    /**
    * \brief public constructor to join any game
    * \param: player id and name and a boolean if its the players first game
    * \result join game request is constructed
    */
    join_game_request(std::string player_id, std::string name, bool first_game_of_player);

    /**
    * \brief public constructor to join a specific game
    * \param: game id, player id and name and a boolean if its the players first game
    * \result call uno request is constructed
    */
    join_game_request(std::string game_id, std::string player_id, std::string name, bool first_game_of_player);

    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static join_game_request* from_json(const rapidjson::Value& json);
};


#endif //UNO_JOIN_GAME_REQUEST_H
