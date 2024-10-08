//
// Created by Manuel on 25.01.2021.
// Updated by UNOrganized on 11.12.2023.
//

#include "player.h"

#include "../../exceptions/UnoException.h"

player::player(std::string name) : unique_serializable() {
    this->_player_name = new serializable_value<std::string>(name);
    this->_called_uno = new serializable_value<bool>(false);
    this->_score = new serializable_value<int>(0);
    this->_hand = new hand();
}

player::player(std::string id, serializable_value<std::string>* name,
               serializable_value<int>* score, hand *hand, serializable_value<bool>* called_uno) :
        unique_serializable(id),
        _player_name(name),
        _hand(hand),
        _score(score),
        _called_uno(called_uno)
{ }

player::~player() {
    if (_player_name != nullptr) {
        delete _hand;
        delete _player_name;
        delete _score;
        delete _called_uno;

        _hand = nullptr;
        _player_name = nullptr;
        _score = nullptr;
        _called_uno = nullptr;
    }
}

#ifdef UNO_SERVER
player::player(std::string id, std::string name) :
        unique_serializable(id)
{
    this->_player_name = new serializable_value<std::string>(name);
    this->_called_uno = new serializable_value<bool>(false);
    this->_score = new serializable_value<int>(0);
    this->_hand = new hand();
}

std::string player::get_game_id() {
    return _game_id;
}

void player::set_game_id(std::string game_id) {
    _game_id = game_id;
}
#endif


int player::get_score() const noexcept {
    return _score->get_value();
}

std::string player::get_player_name() const noexcept {
    return this->_player_name->get_value();
}

const hand* player::get_hand() const noexcept {
    return this->_hand;
}

bool player::has_called_uno() const noexcept {
    return this->_called_uno->get_value();
}

int player::get_nof_cards() const noexcept {
    return _hand->get_nof_cards();
}

int player::get_hand_score() const noexcept {
    return _hand->get_score();
}

void player::set_score(int score) {
    _score->set_value(score);
}

#ifdef UNO_SERVER
void player::setup_round(std::string& err) {
    _called_uno->set_value(false);
    _hand->setup_round(err);
}

void player::dump_cards_except_played(std::string &err, const std::string &card_id) {
    _hand->dump_cards_except_played(err, card_id);
}

bool player::set_called_uno(bool state, std::string &err) {
    if (state == true && has_called_uno()) {
        err = "You already called uno.";
        return false;
    }
    
    _called_uno->set_value(state);
    return true;
}

bool player::add_card(card *card, std::string &err) {
    return _hand->add_card(card, err);
}

bool player::remove_card(std::string card_id, card*& card, std::string &err) {
    card = nullptr;
    return _hand->remove_card(card_id, card, err);
}

#endif


void player::write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const {
    unique_serializable::write_into_json(json, allocator);

    rapidjson::Value id_val(_id.c_str(), allocator);
    json.AddMember("id", id_val, allocator);

    rapidjson::Value name_val(rapidjson::kObjectType);
    _player_name->write_into_json(name_val, allocator);
    json.AddMember("player_name", name_val, allocator);

    rapidjson::Value called_uno_val(rapidjson::kObjectType);
    _called_uno->write_into_json(called_uno_val, allocator);
    json.AddMember("has_called_uno", called_uno_val, allocator);

    rapidjson::Value score_val(rapidjson::kObjectType);
    _score->write_into_json(score_val, allocator);
    json.AddMember("score", score_val, allocator);

    rapidjson::Value hand_val(rapidjson::kObjectType);
    _hand->write_into_json(hand_val, allocator);
    json.AddMember("hand", hand_val, allocator);
}


player *player::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id")
        && json.HasMember("player_name")
        && json.HasMember("has_called_uno")
        && json.HasMember("hand"))
    {
        return new player(
                json["id"].GetString(),
                serializable_value<std::string>::from_json(json["player_name"].GetObject()),
                serializable_value<int>::from_json(json["score"].GetObject()),
                hand::from_json(json["hand"].GetObject()),
                serializable_value<bool>::from_json(json["has_called_uno"].GetObject()));
    } else {
        throw UnoException("Failed to deserialize player from json. Required json entries were missing.");
    }
}
