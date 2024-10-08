//
// Created by Manuel on 29.01.2021.
// Updates by UNOrganized on 11.12.2023.
//

#ifndef UNO_START_GAME_REQUEST_H
#define UNO_START_GAME_REQUEST_H


#include <string>
#include "client_request.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class start_game_request : public client_request{

private:
    bool _questionmarkbool;

    /**
    * \brief private constructor for deserialization
    * \param: all base class properties and a boolean that shows if the game is played with the questionmarkcard
    * \result start game request is constructed
    */
    start_game_request(base_class_properties props, bool questionmarkbool);

public:
    /**
    * \brief get the state of the questionmarkbool
    * \result returns the state of the questionmarkbool
    */
    [[nodiscard]] bool get_questionmark() const {return this -> _questionmarkbool;}

    /**
    * \brief public constructor
    * \param: game and player id and boolean that shows if the game is played with the questionmarkcard
    * \result call uno request is constructed
    */
    start_game_request(std::string game_id, std::string player_id, bool questionmarkbool);
    
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static start_game_request* from_json(const rapidjson::Value& json);
};

#endif //UNO_START_GAME_REQUEST_H