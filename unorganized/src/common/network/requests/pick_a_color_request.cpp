//
// Created by UNOrganized on 11.12.2023.
//

#include "pick_a_color_request.h"

#ifdef UNO_SERVER
#include <string>
#include "../../../server/game_instance_manager.h"
#include "../../../server/player_manager.h"
#include "../../../server/game_instance.h"
#endif

//private constructor
pick_a_color_request::pick_a_color_request(client_request::base_class_properties props, int color) :
    client_request(props), _color(color)
    {}

//public constructor
pick_a_color_request::pick_a_color_request(std::string game_id, std::string player_id, int color) :
    client_request(client_request::create_base_class_properties(RequestType::pick_a_color, uuid_generator::generate_uuid_v4(), player_id, game_id) )
 { 
    _color = color;
 }


void pick_a_color_request::write_into_json(rapidjson::Value &json,
                                        rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    client_request::write_into_json(json, allocator);
    json.AddMember("color", rapidjson::Value(this->_color),allocator);
}

pick_a_color_request* pick_a_color_request::from_json(const rapidjson::Value& json) {
    base_class_properties props = client_request::extract_base_class_properties(json);
    if (json.HasMember("color")) {
        return new pick_a_color_request(props, json["color"].GetInt());
    } else {
        throw UnoException("Could not find 'color' pick_a_color_request");
    }
}




