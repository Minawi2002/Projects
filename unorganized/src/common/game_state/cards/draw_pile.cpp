//
// Created by Manuel on 25.01.2021.
// Updated by UNOrganized on 11.12.2023.
//

#include "draw_pile.h"


#include "../../serialization/vector_utils.h"
#include "../../exceptions/UnoException.h"


// deserialization constructor
draw_pile::draw_pile(std::string id, std::vector<card*> &cards, serializable_value<bool>* state)
        : unique_serializable(id),
          _cards(cards),
          _questionmarkbool(state)
{ }

// from_diff constructor
draw_pile::draw_pile(std::string id) : unique_serializable(id) { }


draw_pile::draw_pile(std::vector<card*> &cards, serializable_value<bool>* state)
        : unique_serializable(), _cards(cards), _questionmarkbool(state)
{ }


draw_pile::draw_pile() : unique_serializable() {
    this->_cards = std::vector<card *>();
    this->_questionmarkbool = new serializable_value<bool>(false);
 }

draw_pile::~draw_pile() {
    _cards.clear();

    delete _questionmarkbool;
    _questionmarkbool = nullptr;
}


void draw_pile::shuffle() {
    std::shuffle(_cards.begin(), _cards.end(), std::mt19937(std::random_device()()));
}

void draw_pile::set_questionmarkbool(bool state){
    _questionmarkbool->set_value(state);
}


bool draw_pile::is_empty() const noexcept  {
    return _cards.empty();
}

int draw_pile::get_nof_cards() const noexcept  {
    return _cards.size();
}

card* draw_pile::get_top_card() const{
    return _cards.back();
}


#ifdef UNO_SERVER
void draw_pile::setup_game(std::string &err) {
    _cards.clear();

    /*** add a fresh set of cards ***/
    //one 0 per color and 2 for all other color-index-combinations
    for(int card_color = 0; card_color <= 3; card_color++){
        _cards.push_back(new card(0, card_color));
        for (int card_index = 1; card_index <= 12; card_index++) {
            for (int i = 0; i < 2; i ++) {
                _cards.push_back(new card(card_index, card_color));
            }
        }
    }

    //add 4 wildcards, 4 +4 wildcards and, if necessary, 4 questionmark cards
    for(int i = 0; i < 4; i ++){
        _cards.push_back(new card(13, 4));
        _cards.push_back(new card(14, 4));

        if(_questionmarkbool->get_value()){
            _cards.push_back(new card(15, 4));
        }
        
    }

    // shuffle the draw pile
    this->shuffle();
}

bool draw_pile::draw(player* player, card*& drawn_card, std::string& err)  {
    if(is_empty()){
        /**
        add a fresh set of cards to the draw pile, this is done because the edge case 
        that all cards are in the players hand and no cards are in the discard pile
        **/
        this->setup_game(err);
    }

    drawn_card = _cards.back();
    if (player->add_card(drawn_card, err)) {
        _cards.pop_back();
        return true;
    } else {
        drawn_card = nullptr;
        err = "Could not draw card."; //should never happen
    }
    return false;
    
}

void draw_pile::distribute_card(int nof_cards, player* player, std::string& err){
    if(is_empty()){
        /**
        add a fresh set of cards to the draw pile, this is done because the edge case 
        that all cards are in the players hand and no cards are in the discard pile
        **/
        this->setup_game(err);
    }

    for(int i = 0; i < nof_cards; ++i){
        card *drawn_card;
        drawn_card = _cards.back();
        if(player->add_card(drawn_card, err)) {
            _cards.pop_back();
        } else {
            drawn_card = nullptr;
            err = "Could not distribute card."; //should never happen
        }
    }
}

card* draw_pile::remove_top(std::string& err) {
    if(is_empty()){
        /**
        add a fresh set of cards to the draw pile, this is done because the edge case 
        that all cards are in the players hand and no cards are in the discard pile
        **/
        this->setup_game(err);
    }

    card* drawn_card = nullptr;
    drawn_card = _cards.back();
    _cards.pop_back();
    return drawn_card;

}


#endif


void draw_pile::write_into_json(rapidjson::Value &json, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    unique_serializable::write_into_json(json, allocator);
    json.AddMember("cards", vector_utils::serialize_vector(_cards, allocator), allocator);
    
    rapidjson::Value questionmark_val(rapidjson::kObjectType);
    _questionmarkbool->write_into_json(questionmark_val, allocator);
    json.AddMember("questionmark", questionmark_val, allocator);
}


draw_pile *draw_pile::from_json(const rapidjson::Value &json) {
    if (json.HasMember("id") && json.HasMember("cards") && json.HasMember("questionmark")) {
        std::vector<card*> deserialized_cards = std::vector<card*>();
        for (auto &serialized_card : json["cards"].GetArray()) {
            deserialized_cards.push_back(card::from_json(serialized_card.GetObject()));
        }
        return new draw_pile(json["id"].GetString(), deserialized_cards, 
                            serializable_value<bool>::from_json(json["questionmark"].GetObject()));
    } else {
        throw UnoException("Could not parse draw_pile from json. 'id' or 'cards' were missing.");
    }
}
