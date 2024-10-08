//
// Created by Manuel on 15.02.2021.
//

#ifndef UNO_FULL_STATE_RESPONSE_H
#define UNO_FULL_STATE_RESPONSE_H

#include "server_response.h"
#include "../../game_state/game_state.h"

class full_state_response : public server_response {
private:
    rapidjson::Value* _state_json;

    /**
    * \brief private constructor for deserialization
    * \param: all base class properties and a rapidjson state
    * \result full state response is constructed
    */
    full_state_response(base_class_properties props, rapidjson::Value* state_json);

public:
    /**
    * \brief public constructor
    * \param: game id and game state
    * \result full state response is constructed
    */
    full_state_response(std::string game_id, const game_state& state);

    /**
    * \brief destructor
    * \result full state response is destructed
    */
    ~full_state_response();

    /**
    * \brief get the json state
    * \result returns the json state
    */
    rapidjson::Value* get_state_json() const;

    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static full_state_response* from_json(const rapidjson::Value& json);

#ifdef UNO_CLIENT
    /**
    * \brief tries to execute and build the new game state
    * \result new game state is executed and built or error message is sent
    */
    virtual void Process() const override;
#endif
};


#endif //UNO_FULL_STATE_RESPONSE_H
