//
// Created by Manuel on 25.01.2021.
// Updated by UNOrganized on 11.12.2023.
//

#include "card.h"

#include "../../exceptions/UnoException.h"


card::card(std::string id) : unique_serializable(id) { }

card::card(std::string id, serializable_value<int> * ind, serializable_value<int>* col)
        : unique_serializable(id), _index(ind), _color(col)
{ }

card::card(int ind, int col) :
        unique_serializable(),
        _index(new serializable_value<int>(ind)),
        _color(new serializable_value<int>(col))
{ }

card::~card() { }


int card::get_index() const noexcept {
    return _index->get_value();
}

int card::get_color() const noexcept {
    return _color->get_value();
}

bool card::can_be_played_on(const card *const other) const noexcept {
    int this_index = this->get_index();
    int index_delta = this_index - other->get_index();
    int color_delta = this->get_color() - other->get_color();
    
    //returns true if same color or same number or wildcard
    return color_delta == 0 || index_delta == 0 || this_index >= 13;
}

void card::change_color(int col){
    _color->set_value(col);
}


card *card::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id") && json.HasMember("index") && json.HasMember("color")) {
        return new card(json["id"].GetString(), serializable_value<int>::from_json(json["index"].GetObject()), 
                    serializable_value<int>::from_json(json["color"].GetObject()));
    } else {
        throw UnoException("Could not parse json of card. Was missing 'id' or 'index' or 'color'.");
    }
}


void card::write_into_json(rapidjson::Value &json, rapidjson::Document::AllocatorType& allocator) const {
    unique_serializable::write_into_json(json, allocator);

    rapidjson::Value value_ind(rapidjson::kObjectType);
    rapidjson::Value value_col(rapidjson::kObjectType);
    _index->write_into_json(value_ind, allocator);
    _color->write_into_json(value_col, allocator);
    json.AddMember("index", value_ind, allocator);
    json.AddMember("color", value_col, allocator);
}






