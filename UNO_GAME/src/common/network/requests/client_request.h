//
// Created by Manuel on 28.01.2021.z
// Updates by UNOrganized on 11.12.2023.
//
// client_request is the base-class for all requests that are being sent from the client to the server.
// It offers a function to deserialize a client_request subclass from a valid json.

#ifndef UNO_CLIENT_REQUEST_H
#define UNO_CLIENT_REQUEST_H

#include <string>
#include <unordered_map>
#include "../../../../rapidjson/include/rapidjson/document.h"
#include "../../serialization/serializable.h"
#include "../../exceptions/UnoException.h"
#include "../../serialization/uuid_generator.h"
#include "../../serialization/json_utils.h"

// Identifier for the different request types.
// The RequestType is sent with every client_request to identify the type of client_request
// during deserialization on the server side.
enum RequestType {
    join_game,
    start_game,
    new_round,
    play_card,
    draw_card,
    call_uno,
    pick_a_color,
    player_left
};

class client_request : public serializable {
protected:

    struct base_class_properties {
        RequestType _type;
        std::string _req_id;
        std::string _player_id;
        std::string _game_id;
    };

    RequestType _type;
    std::string _req_id;
    std::string _player_id;
    std::string _game_id;

    /**
    * \brief constructs client request
    * \param: a struct with all base class properties
    * \result client request is constructed
    */
    explicit client_request(base_class_properties); // base constructor

    /**
    * \brief creates a struct with all base class properties
    * \param: the request type and id, the player and game id
    * \result returns a struct with all base class properties
    */
    static base_class_properties create_base_class_properties(RequestType type, std::string req_id, std::string& player_id, std::string& game_id);
    
    /**
    * \brief extracts the base class properties that are received from json
    * \param: a rapidjson::value with informations of the base class properties
    * \result returns a struct with all base class properties
    */
    static base_class_properties extract_base_class_properties(const rapidjson::Value& json);

private:

    // for deserialization
    static const std::unordered_map<std::string, RequestType> _string_to_request_type;
    // for serialization
    static const std::unordered_map<RequestType, std::string> _request_type_to_string;

public:
    virtual ~client_request() {}

    /**
    * \brief get type of the request
    * \result returns type of the request
    */
    [[nodiscard]] RequestType get_type() const { return this->_type; }

    /**
    * \brief get id of the request 
    * \result returns id of the request
    */
    [[nodiscard]] std::string get_req_id() const { return this->_req_id; }

    /**
    * \brief get game id corresponding to the request
    * \result returns game id corresponding to the request
    */
    [[nodiscard]] std::string get_game_id() const { return this->_game_id; }

    /**
    * \brief get player id corresponding to the request
    * \result returns player id corresponding to the request
    */
    [[nodiscard]] std::string get_player_id() const { return this->_player_id; }

    // Tries to create the specific client_request from the provided json.
    // Throws exception if parsing fails -> Use only in "try{ }catch()" block
    static client_request* from_json(const rapidjson::Value& json);

    // Serializes the client_request into a json object that can be sent over the network
    void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;

    [[nodiscard]] virtual std::string to_string() const;
};


#endif //UNO_CLIENT_REQUEST_H
