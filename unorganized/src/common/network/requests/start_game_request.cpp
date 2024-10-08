//
// Created by Manuel on 29.01.2021.
// Updates by UNOrganized on 11.12.2023.
//

#include "start_game_request.h"

#ifdef UNO_SERVER
#include "../../../server/game_instance_manager.h"
#include "../../../server/game_instance.h"
#endif

// Public constructor
start_game_request::start_game_request(std::string game_id, std::string player_id, bool questionmarkbool)
        : client_request( client_request::create_base_class_properties(RequestType::start_game, uuid_generator::generate_uuid_v4(), player_id, game_id) )
{ 
        _questionmarkbool = questionmarkbool;
}

// private constructor for deserialization
start_game_request::start_game_request(client_request::base_class_properties props, bool questionmarkbool) :
        client_request(props), _questionmarkbool(questionmarkbool)
{ }

start_game_request* start_game_request::from_json(const rapidjson::Value& json) {
    //return new start_game_request(client_request::extract_base_class_properties(json));
    base_class_properties props = client_request::extract_base_class_properties(json);
    if (json.HasMember("questionmarkbool")) {
        return new start_game_request(props, json["questionmarkbool"].GetBool());
    } else {
        throw UnoException("Could not find 'questionmarkbool' start_game_request");
    }
}

void start_game_request::write_into_json(rapidjson::Value &json,
                                         rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    client_request::write_into_json(json, allocator);
    json.AddMember("questionmarkbool", rapidjson::Value(this->_questionmarkbool),allocator);
}
