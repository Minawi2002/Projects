//
// Created by Manuel on 29.01.2021.
// Updates by UNOrganized on 11.12.2023.
//

#include "player_left_request.h"

#ifdef UNO_SERVER
#include "../../../server/game_instance_manager.h"
#include "../../../server/game_instance.h"
#endif

// Public constructor
player_left_request::player_left_request(std::string game_id, std::string player_id)
        : client_request( client_request::create_base_class_properties(RequestType::player_left, uuid_generator::generate_uuid_v4(), player_id, game_id) )
{ 
}

// private constructor for deserialization
player_left_request::player_left_request(client_request::base_class_properties props) :
        client_request(props)
{ }

player_left_request* player_left_request::from_json(const rapidjson::Value& json) {
    base_class_properties props = client_request::extract_base_class_properties(json);
    return new player_left_request(props);

}

void player_left_request::write_into_json(rapidjson::Value &json,
                                         rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    client_request::write_into_json(json, allocator);
}
