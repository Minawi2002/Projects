//
// Created by UNOrganized on 11.12.2023.
//
#include "new_round_request.h"

#ifdef UNO_SERVER
#include "../../../server/game_instance_manager.h"
#include "../../../server/game_instance.h"
#endif

// Public constructor
new_round_request::new_round_request(std::string game_id, std::string player_id)
        : client_request( client_request::create_base_class_properties(RequestType::new_round, uuid_generator::generate_uuid_v4(), player_id, game_id) )
{ 
}

// private constructor for deserialization
new_round_request::new_round_request(client_request::base_class_properties props) :
        client_request(props)
{ }

new_round_request* new_round_request::from_json(const rapidjson::Value& json) {
    //return new new_round_request(client_request::extract_base_class_properties(json));
    base_class_properties props = client_request::extract_base_class_properties(json);
    return new new_round_request(props);
}

void new_round_request::write_into_json(rapidjson::Value &json,
                                         rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    client_request::write_into_json(json, allocator);
}
