//
// Created by Manuel on 25.01.2021.
// Updated by UNOrganized on 11.12.2023.
//

#include "discard_pile.h"
#include "../../serialization/vector_utils.h"
#include "../../exceptions/UnoException.h"


discard_pile::discard_pile(std::string id) : unique_serializable(id) { }

discard_pile::discard_pile(std::string id, std::vector<card *> &cards):
        unique_serializable(id),
        _cards(cards)
{ }

discard_pile::discard_pile() : unique_serializable() { }

discard_pile::~discard_pile() {
    _cards.clear();
}


bool discard_pile::can_play(const card *card)  {
    return _cards.empty() || card->can_be_played_on(_cards.back());
}

card* discard_pile::get_top_card() const  {
    if (_cards.size() > 0) {
        return _cards.back();
    } else {
        return nullptr;
    }
}

int discard_pile::check_special_card(){
    card* top_card = this->get_top_card();
    return top_card->get_index();
}

#ifdef UNO_SERVER
void discard_pile::setup_game(std::string &err) {
    _cards.clear();
}

bool discard_pile::try_play(const std::string& card_id, player* player, std::string& err) {
    card* played_card = nullptr;
    if (player->get_hand()->try_get_card(card_id, played_card)) {
        if (can_play(played_card)) {
            card* local_system_card;
            if (player->remove_card(played_card->get_id(), local_system_card, err)) {
                _cards.push_back(local_system_card);
                return true;
            } else {
                err = "Could not play card " + played_card->get_id() + " because player does not have this card.";
            }
        } else {
            err = "The desired card cannot be played on top of the discard pile as it has neither the same symbol nor the same color";
        }
    } else {
        err = "The player does not possess the card " + card_id + ", which was requested to be played.";
    }
    return false;
}

bool discard_pile::try_play(card* played_card, std::string& err) {
    if (can_play(played_card)) {
        _cards.push_back(played_card);
        return true;
    } else {
        err = "The desired card cannot be played on top of the discard pile as it has neither the same symbol nor the same color";
    }
    return false;
}

bool discard_pile::change_color(card* wildcard, int color, std::string& err){
    if(wildcard->get_index() != 13 && wildcard->get_index() != 14 && wildcard->get_index() != 15){
        err = "The played card is no wildcard";
    }else if(color < 0 || color >= 4){
        err = "The desired color does not exist";
    }else{
        wildcard->change_color(color);
        return true;
    }
    return false;
}
#endif


discard_pile *discard_pile::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id") && json.HasMember("cards")) {
        std::vector<card*> deserialized_cards = std::vector<card*>();
        for (auto &serialized_card : json["cards"].GetArray()) {
            deserialized_cards.push_back(card::from_json(serialized_card.GetObject()));
        }
        return new discard_pile(json["id"].GetString(), deserialized_cards);
    } else {
        throw UnoException("Could not parse draw_pile from json. 'id' or 'cards' were missing.");
    }
}

void discard_pile::write_into_json(rapidjson::Value &json,
                                   rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    unique_serializable::write_into_json(json, allocator);
    json.AddMember("cards", vector_utils::serialize_vector(_cards, allocator), allocator);
}





