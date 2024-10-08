//
// Created by UNOrganized on 11.12.2023.
//

#include "call_uno_request.h"

#ifdef UNO_SERVER
#include <string>
#include "../../../server/game_instance_manager.h"
#include "../../../server/player_manager.h"
#include "../../../server/game_instance.h"
#endif

//private constructor
call_uno_request::call_uno_request(client_request::base_class_properties props) :
    client_request(props)
    {}

//public constructor
call_uno_request::call_uno_request(std::string game_id, std::string player_id) :
    client_request(client_request::create_base_class_properties(RequestType::call_uno, uuid_generator::generate_uuid_v4(), player_id, game_id) )
 { }


void call_uno_request::write_into_json(rapidjson::Value &json,
                                        rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    client_request::write_into_json(json, allocator);
}

call_uno_request* call_uno_request::from_json(const rapidjson::Value& json) {
    base_class_properties props = client_request::extract_base_class_properties(json);
    return new call_uno_request(props);
}




