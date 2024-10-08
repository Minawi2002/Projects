//
// Created by UNOrganized on 11.12.2023.
//

#ifndef UNO_CALLED_UNO_REQUEST_H
#define UNO_CALLED_UNO_REQUEST_H


#include <string>
#include "client_request.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class call_uno_request : public client_request{
    private:

    /**
    * \brief private constructor for deserialization
    * \param: all base class properties that are needed
    * \result call uno request is constructed
    */
    call_uno_request(base_class_properties props);

    public:

    /**
    * \brief public constructor
    * \param: the game and player id used for the request
    * \result call uno request is constructed
    */
    call_uno_request(std::string game_id, std::string player_id);

    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static call_uno_request* from_json(const rapidjson::Value& json);


};

#endif //UNO_CALLED_UNO_REQUEST_H