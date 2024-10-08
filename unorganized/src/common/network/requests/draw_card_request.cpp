//
// Created by Manuel on 29.01.2021.
// Updates by UNOrganized on 11.12.2023.
//

#include "draw_card_request.h"

#ifdef UNO_SERVER
#include "../../../server/game_instance_manager.h"
#include "../../../server/game_instance.h"
#endif

// Public constructor
draw_card_request::draw_card_request(std::string game_id, std::string player_id)
        : client_request( client_request::create_base_class_properties(RequestType::draw_card, uuid_generator::generate_uuid_v4(), player_id, game_id) )
{
}

// private constructor for deserialization
draw_card_request::draw_card_request(client_request::base_class_properties props) :
        client_request(props)
{ }

draw_card_request* draw_card_request::from_json(const rapidjson::Value &json) {
    base_class_properties props = client_request::extract_base_class_properties(json);
    return new draw_card_request(props);
}

void draw_card_request::write_into_json(rapidjson::Value &json,
                                        rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    client_request::write_into_json(json, allocator);
}
