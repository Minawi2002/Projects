//
// Created by Manuel on 28.01.2021.
// Updates by UNOrganized on 11.12.2023.
//

#ifndef UNO_PLAY_CARD_REQUEST_H
#define UNO_PLAY_CARD_REQUEST_H


#include "client_request.h"
#include <string>
#include "../../../../rapidjson/include/rapidjson/document.h"

class play_card_request : public client_request {

private:

    std::string _card_id;

    /**
    * \brief private constructor for deserialization
    * \param: all base class properties that are needed and the card id
    * \result play card request is constructed
    */
    play_card_request(base_class_properties, std::string card_id);

public:
    /**
    * \brief get the id of the card in the request
    * \result returns the id of the card in the request
    */
    [[nodiscard]] std::string get_card_id() const { return this->_card_id; }

    /**
    * \brief public constructor
    * \param: id of game, player and card
    * \result play card request is constructed
    */
    play_card_request(std::string game_id, std::string player_id, std::string card_id);
    
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static play_card_request* from_json(const rapidjson::Value& json);
};


#endif //UNO_PLAY_CARD_REQUEST_H
