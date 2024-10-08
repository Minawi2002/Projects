//
// Created by Manuel on 29.01.2021.
// Updates by UNOrganized on 11.12.2023.
//

#ifndef UNO_DRAW_CARD_REQUEST_H
#define UNO_DRAW_CARD_REQUEST_H

#include <string>
#include "client_request.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class draw_card_request : public client_request {

private:

    /**
    * \brief private constructor for deserialization
    * \param: all base class properties that are needed and the 
    * \result draw card request is constructed
    */
    draw_card_request(base_class_properties props);

public:


    /**
    * \brief public constructor 
    * \param: the game and player id used for the request
    * \result draw card request is constructed
    */
    draw_card_request(std::string game_id, std::string player_id);
    
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static draw_card_request* from_json(const rapidjson::Value& json);
};


#endif //UNO_DRAW_CARD_REQUEST_H
