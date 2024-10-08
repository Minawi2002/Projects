//
// Created by Manuel on 29.01.2021.
// Updates by UNOrganized on 11.12.2023.
//

#ifndef UNO_PLAYER_LEFT_REQUEST_H
#define UNO_PLAYER_LEFT_REQUEST_H


#include <string>
#include "client_request.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class player_left_request : public client_request{

private:
    /**
    * \brief private constructor for deserialization
    * \param: all base class properties that are needed
    * \result player left request is constructed
    */
    player_left_request(base_class_properties props);

public:
    /**
    * \brief public constructor
    * \param: game id and player id
    * \result player left card request is constructed
    */
    player_left_request(std::string game_id, std::string player_id);
    
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static player_left_request* from_json(const rapidjson::Value& json);
};

#endif //UNO_PLAYER_LEFT_REQUEST_H