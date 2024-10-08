//
// Created by Manuel on 27.01.2021.
// Updated by UNOrganized on 11.12.2023.
//

#include "hand.h"

#include "../../exceptions/UnoException.h"
#include "../../serialization/vector_utils.h"

hand::hand() : unique_serializable() { }

hand::hand(std::string id) : unique_serializable(id) { }

// deserialization constructor
hand::hand(std::string id, std::vector<card*> cards) : unique_serializable(id) {
    this->_cards = cards;
}

hand::~hand() {
    _cards.clear();
}

int hand::get_nof_cards() const {
    return _cards.size();
}

int hand::get_score() const {
    int res = 0;
    
    for (int i = 0; i < _cards.size(); i++) {
        int card_index = _cards[i]->get_index();
        if(card_index <=9 && card_index >= 0){
            res += card_index;
        }else if(card_index >= 10 && card_index <= 12){
            res+= 20;
        }else if(card_index >= 13 && card_index <= 15){
            res += 50;
        }
    }
    return res;
}

const std::vector<card*> hand::get_cards() const {
    return _cards;
}

bool hand::try_get_card(const std::string &card_id, card *&hand_card) const {
    auto it = std::find_if(_cards.begin(), _cards.end(),
                           [&card_id](const card* x) { return x->get_id() == card_id;});
    if (it < _cards.end()) {
        hand_card = *it;
        return true;
    }
    return false;
}


card* hand::remove_card(int idx) {
    return remove_card(_cards.begin() + idx);
}

card* hand::remove_card(card* card) {
    auto pos = std::find(_cards.begin(), _cards.end(), card);
    return remove_card(pos);
}

card* hand::remove_card(std::vector<card*>::iterator pos) {
    if (pos >= _cards.begin() && pos < _cards.end()) {
        card* res = *pos;
        _cards.erase(pos);
        return res;
    }
    return nullptr;
}

std::vector<card*>::iterator hand::get_card_iterator() {
    return _cards.begin();
}


#ifdef UNO_SERVER
void hand::setup_round(std::string &err) {
    _cards.clear();
}

bool hand::add_card(card* new_card, std::string &err) {
    _cards.push_back(new_card);
    return true;
}

bool hand::remove_card(std::string card_id, card*& played_card, std::string &err) {
    played_card = nullptr;
    auto it = std::find_if(_cards.begin(), _cards.end(),
                           [&card_id](const card* x) { return x->get_id() == card_id;});
    if (it < _cards.end()) {
        played_card = remove_card(it);
        return true;
    } else {
        err = "Could not play card, as the requested card was not on the player's hand.";
        return false;
    }
}

void hand::dump_cards_except_played(std::string &err, const std::string &card_id) {
    for (int i = 0; i < _cards.size(); i++) {
        if (_cards[i]->get_id() != card_id) {
            _cards[i] = this->remove_card(i);
        }
    }
}


#endif


void hand::write_into_json(rapidjson::Value &json, rapidjson::Document::AllocatorType& allocator) const {
    unique_serializable::write_into_json(json, allocator);
    json.AddMember("cards", vector_utils::serialize_vector(_cards, allocator), allocator);
}

hand *hand::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id") && json.HasMember("cards")) {
        std::vector<card*> deserialized_cards = std::vector<card*>();
        for (auto &serialized_card : json["cards"].GetArray()) {
            deserialized_cards.push_back(card::from_json(serialized_card.GetObject()));
        }
        return new hand(json["id"].GetString(), deserialized_cards);
    } else {
        throw UnoException("Could not parse hand from json. 'cards' were missing.");
    }
}



