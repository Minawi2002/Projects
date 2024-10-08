//
// Created by Manuel on 15.02.2021.
//

#ifndef UNO_REQUEST_RESPONSE_H
#define UNO_REQUEST_RESPONSE_H

#include <string>
#include "server_response.h"


class request_response : public server_response {
private:
    bool _success;
    std::string _err;
    std::string _req_id;
    rapidjson::Value* _state_json = nullptr;

    /**
    * \brief private constructor for deserialization
    * \param: all base class properties, the request id, a success bool, an error string and a json state
    * \result request response is constructed
    */
    request_response(base_class_properties props, std::string req_id, bool success, rapidjson::Value* state_json, std::string& err);

public:
    /**
    * \brief public constructor
    * \param: a game id, a request id, a success bool, an error string and a json state
    * \result request response is constructed
    */
    request_response(std::string game_id, std::string req_id, bool success, rapidjson::Value* state_json, std::string err);
    
    /**
    * \brief destructor
    * \result request response is destructed
    */
    ~request_response();

    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static request_response* from_json(const rapidjson::Value& json);

    /**
    * \brief returns the error
    */
    std::string get_error();

    /**
    * \brief returns the request id
    */
    std::string get_req_id();

    /**
    * \brief returns the state of succes of the request
    */
    bool get_success();

#ifdef UNO_CLIENT
    /**
    * \brief tries to execute and build the new game state
    * \result new game state is executed and built or error message is sent
    */
    virtual void Process() const override;
#endif
};


#endif //UNO_REQUEST_RESPONSE_H
