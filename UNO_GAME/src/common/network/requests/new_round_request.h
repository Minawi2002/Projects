//
// Created by UNOrganized on 11.12.2023.
//

#ifndef UNO_NEW_ROUND_REQUEST_H
#define UNO_NEW_ROUND_REQUEST_H


#include <string>
#include "client_request.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class new_round_request : public client_request{

private:
    /**
    * \brief private constructor for deserialization
    * \param: all base class properties that are needed
    * \result new round request is constructed
    */
    new_round_request(base_class_properties props);

public:

    /**
    * \brief public constructor 
    * \param: game and player id
    * \result new round request is constructed
    */
    new_round_request(std::string game_id, std::string player_id);
    
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static new_round_request* from_json(const rapidjson::Value& json);
};

#endif //UNO_NEW_ROUND_REQUEST_H