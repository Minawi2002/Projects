//
// Created by Manuel on 27.01.2021.
// Updated by UNOrganized on 11.12.2023.
//

#include "game_state.h"

#include "../exceptions/UnoException.h"
#include "../serialization/vector_utils.h"


game_state::game_state() : unique_serializable()
{
    this->_draw_pile = new draw_pile();
    this->_discard_pile = new discard_pile();
    this->_players = std::vector<player *>();
    this->_is_started = new serializable_value<bool>(false);
    this->_is_finished = new serializable_value<bool>(false);
    this->_current_player_idx = new serializable_value<int>(0);
    this->_previous_player_idx = new serializable_value<int>(0);
    this->_play_direction = new serializable_value<int>(1);
    this->_round_number = new serializable_value<int>(0);
    this->_starting_player_idx = new serializable_value<int>(0);
    this->_unanswered_wildcard = new serializable_value<bool>(false);
    this->_missed_uno_call = new serializable_value<bool>(false);
    this->_play_with_questionmark = new serializable_value<bool>(false);
    this->_players_in_new_round = new serializable_value<int>(0);
}

game_state::game_state(std::string id, draw_pile *draw_pile, discard_pile *discard_pile,
                       std::vector<player *> &players, serializable_value<bool> *is_started,
                       serializable_value<bool> *is_finished, serializable_value<int> *current_player_idx,
                       serializable_value<int> *previous_player_idx,
                       serializable_value<int> *play_direction, serializable_value<int> *round_number,
                       serializable_value<int> *starting_player_idx, serializable_value<bool>* unanswered_wildcard,
                       serializable_value<bool>* missed_uno_call, serializable_value<bool>* play_with_questionmark,
                       serializable_value<int>* players_in_new_round)
        : unique_serializable(id),
          _draw_pile(draw_pile),
          _discard_pile(discard_pile),
          _players(players),
          _is_started(is_started),
          _is_finished(is_finished),
          _current_player_idx(current_player_idx),
          _previous_player_idx(previous_player_idx),
          _play_direction(play_direction),
          _round_number(round_number),
          _starting_player_idx(starting_player_idx),
          _unanswered_wildcard(unanswered_wildcard),
          _missed_uno_call(missed_uno_call),
          _play_with_questionmark(play_with_questionmark),
          _players_in_new_round(players_in_new_round)
{}

game_state::game_state(std::string id) : unique_serializable(id)
{
    this->_draw_pile = new draw_pile();
    this->_discard_pile = new discard_pile();
    this->_players = std::vector<player *>();
    this->_is_started = new serializable_value<bool>(false);
    this->_is_finished = new serializable_value<bool>(false);
    this->_current_player_idx = new serializable_value<int>(0);
    this->_previous_player_idx = new serializable_value<int>(0);
    this->_play_direction = new serializable_value<int>(1);
    this->_round_number = new serializable_value<int>(0);
    this->_starting_player_idx = new serializable_value<int>(0);
    this->_unanswered_wildcard = new serializable_value<bool>(false);
    this->_missed_uno_call = new serializable_value<bool>(false);
    this->_play_with_questionmark = new serializable_value<bool>(false);
    this->_players_in_new_round = new serializable_value<int>(0);
}

game_state::~game_state()
{
    if (_is_started != nullptr)
    {
        delete _is_started;
        delete _is_finished;
        delete _draw_pile;
        delete _discard_pile;
        delete _current_player_idx;
        delete _previous_player_idx;
        delete _starting_player_idx;
        delete _play_direction;
        delete _round_number;
        delete _unanswered_wildcard;
        delete _missed_uno_call;
        delete _play_with_questionmark;
        delete _players_in_new_round;

        _is_started = nullptr;
        _is_finished = nullptr;
        _draw_pile = nullptr;
        _discard_pile = nullptr;
        _current_player_idx = nullptr;
        _previous_player_idx = nullptr;
        _starting_player_idx = nullptr;
        _play_direction = nullptr;
        _round_number = nullptr;
        _unanswered_wildcard = nullptr;
        _missed_uno_call = nullptr;
        _play_with_questionmark = nullptr;
        _players_in_new_round = nullptr;
    }
}

player *game_state::get_current_player() const
{
    if (_current_player_idx == nullptr || _players.size() == 0)
    {
        return nullptr;
    }
    return _players[_current_player_idx->get_value()];
}

player *game_state::get_previous_player() const
{
    if (_previous_player_idx == nullptr || _players.size() == 0)
    {
        return nullptr;
    }
    return _players[_previous_player_idx->get_value()];
}

discard_pile *game_state::get_discard_pile() const
{
    return _discard_pile;
}

draw_pile *game_state::get_draw_pile() const
{
    return _draw_pile;
}

bool game_state::is_full() const
{
    return _players.size() == _max_nof_players;
}


bool game_state::is_started() const
{
    return _is_started->get_value();
}

bool game_state::is_finished() const
{
    return _is_finished->get_value();
}

int game_state::get_round_number() const
{
    return _round_number->get_value();
}

int game_state::get_players_in_new_round() const
{
    return _players_in_new_round->get_value();
}

bool game_state::get_unanswered_wildcard() const
{
    return _unanswered_wildcard->get_value();
}

bool game_state::get_missed_uno_call() const
{
    return _missed_uno_call->get_value();
}

bool game_state::get_play_with_questionmark() const
{
    return _play_with_questionmark->get_value();
}

int game_state::get_player_index(player *player) const
{
    auto it = std::find(_players.begin(), _players.end(), player);
    if (it == _players.end())
    {
        return -1;
    } else
    {
        return it - _players.begin();
    }
}

bool game_state::is_player_in_game(player *player) const
{
    return std::find(_players.begin(), _players.end(), player) < _players.end();
}

bool game_state::is_allowed_to_play_now(player *player) const
{
    return player == get_current_player(); 
}

std::vector<player *> &game_state::get_players()
{
    return _players;
}


#ifdef UNO_SERVER

// state modification functions without diff
void game_state::setup_round(std::string &err)
{

    // update round number
    _round_number->set_value(_round_number->get_value() + 1);

    // setup draw_pile
    _draw_pile->set_questionmarkbool(_play_with_questionmark->get_value());
    _draw_pile->setup_game(err);

    // setup discard_pile
    _discard_pile->setup_game(err);

    // setup players
    for (int i = 0; i < _players.size(); i++)
    {
        _players[i]->setup_round(err);
        // draw 7 cards
        card *drawn_card = nullptr;
        for (int j = 0; j < 7; j++)
        {
            if (!_draw_pile->draw(_players[i], drawn_card, err))
            {
                std::cerr << err << std::endl;
            }
        }
    }

    // set a first card onto the discard pile, if it is a special card, draw another card
    card *top_card = nullptr;
    do
    {
        _draw_pile->shuffle();
        top_card = _draw_pile->get_top_card();
    } while (top_card->get_index() >= 10);
    _draw_pile->remove_top(err);
    _discard_pile->try_play(top_card, err);

    _current_player_idx->set_value(_starting_player_idx->get_value());
    _previous_player_idx->set_value(_starting_player_idx->get_value());
}

void game_state::wrap_up_round(std::string &err)
{
    bool is_game_over = false;

    //add up the points the winner will get and give them to the winner
    for(int j = 0; j < _players.size(); j++) {
        if(_players[j]->get_nof_cards() == 0){
            int tempscore = _players[j]->get_score();
            for(int i = 0; i < _players.size(); i++) {
                tempscore += _players[i]->get_hand_score();
            }
            _players[j]->set_score(tempscore);
            if(tempscore >= 500){
                is_game_over = true;
            }
        }
    }

    if (is_game_over) {
        this->_is_finished->set_value(true);
    } else {
        // decide which player starts in the next round
        _starting_player_idx->set_value((_starting_player_idx->get_value() + 1) % _players.size());
        // start next round
        setup_round(err);
    }
}

void game_state::update_current_player(std::string &err)
{
    int nof_players = _players.size();
    int current_player_idx = _current_player_idx->get_value();

    //update player according to playing direction
    _previous_player_idx->set_value(current_player_idx);
    if (_play_direction->get_value() == 1)
    {
        ++current_player_idx %= nof_players;
    } else
    {
        --current_player_idx;
        if(current_player_idx < 0){
            current_player_idx += nof_players;
        }
    }
    _current_player_idx->set_value(current_player_idx);

}

void game_state::skip_player(std::string &err)
{
    update_current_player(err);

    // previous player is the player before the skipped one
    if (_play_direction->get_value() == 1) {
        int current_player_idx = _current_player_idx->get_value() - 2;
        if(current_player_idx < 0){
            current_player_idx += _players.size();
        }
        _previous_player_idx->set_value(current_player_idx);

    } else {
        _previous_player_idx->set_value((_current_player_idx->get_value() + 2) % _players.size());
    }
}

void game_state::reverse_direction()
{
    //if the play direction is 1, set it to -1. Otherwise, set it to 1.
    _play_direction->set_value(_play_direction->get_value() * -1);
}

//changes the color of the card on top of the discard pile to color
bool game_state::set_color(int color, std::string &err)
{
    discard_pile* pile = this->get_discard_pile();
    card* top_card = pile->get_top_card();
    if(pile->change_color(top_card, color, err)){
        set_unanswered_wildcard(false);

        //execute +4 wildcard if necessary
        if(top_card->get_index() == DRAW_4_WILDCARD){

            update_current_player(err);
            _draw_pile->distribute_card(4, this->get_current_player(), err);
            skip_player(err);
        }else{
            update_current_player(err);
        }
        return true;
    }

    err = "could not change color of wildcard";
    return false;
}

void game_state::set_unanswered_wildcard(bool state){
    _unanswered_wildcard->set_value(state);
}

void game_state::set_missed_uno_call(bool state){
    _missed_uno_call->set_value(state);
}

void game_state::execute_questionmark(std::string &err, const std::string &card_id){

    //redistribute cards in hands evenly among all players
    int nof_players = _players.size();

    //collect all cards in a vector except current card
    std::vector<card*> all_cards;
    for(int i = 0; i < nof_players; ++i){
        std::vector<card*> hand = _players[i]->get_hand()->get_cards();
        for(int j = 0; j < hand.size(); ++j){
            if(hand[j]->get_id() != card_id){
                all_cards.push_back(hand[j]);
            }
        }
    }
    //delete all cards from all hands
    for(int i = 0; i < nof_players; ++i){
        int nof_hand_cards = _players[i]->get_nof_cards();
        for(int j = 0; j < nof_hand_cards; ++j){
            _players[i]->dump_cards_except_played(err, card_id);
        }
    }

    //shuffle the vector
    std::shuffle(all_cards.begin(), all_cards.end(), std::mt19937(std::random_device()()));

    //distribute the cards evenly among all players
    int nof_cards = all_cards.size();
    for(int i = 0; i < nof_cards;){
        for(int j = 0; j < nof_players; ++j){
            _players[j]->add_card(all_cards[i], err);
            ++i;
            if(i >= nof_cards){
                break;
            }
        }
    }

}

bool game_state::set_play_with_questionmark(bool state){
        _play_with_questionmark->set_value(state);
        return true;
    }

bool game_state::start_game(std::string &err)
{
    if (_players.size() < _min_nof_players)
    {
        err = "You need at least " + std::to_string(_min_nof_players) + " players to start the game.";
        game_state::set_play_with_questionmark(false);
        return false;
    }

    if (!_is_started->get_value())
    {
        this->setup_round(err);
        this->_is_started->set_value(true);
        _players_in_new_round->set_value(0);
        return true;
    } else 
    {
        err = "Could not start game, as the game was already started";
        return false;
    }
}

bool game_state::new_round(std::string &err)
{
    _players_in_new_round->set_value(_players_in_new_round->get_value() + 1);
    return true;
}

bool game_state::remove_player(player *player_ptr, std::string &err)
{
    int idx = get_player_index(player_ptr);
    if (idx != -1)
    {
        if (idx < _current_player_idx->get_value())
        {
            // reduce current_player_idx if the player who left had a lower index
            _current_player_idx->set_value(_current_player_idx->get_value() - 1);
        }
        _players.erase(_players.begin() + idx);

        _players_in_new_round->set_value(_players_in_new_round->get_value() - 1);
        if(get_players().size() == 1){
            _is_finished->set_value(true);
        }
        return true;
    } else
    {
        err = "Could not leave game, as the requested player was not found in that game.";
        return false;
    }
}


bool game_state::add_player(player *player_ptr, std::string &err)
{
    if (_is_started->get_value())
    {
        err = "Could not join game, because the requested game is already started.";
        return false;
    }
    if (_is_finished->get_value())
    {
        err = "Could not join game, because the requested game is already finished.";
        return false;
    }
    if (_players.size() >= _max_nof_players)
    {
        err = "Could not join game, because the max number of players is already reached.";
        return false;
    }
    if (std::find(_players.begin(), _players.end(), player_ptr) != _players.end())
    {
        err = "Could not join game, because this player is already subscribed to this game.";
        return false;
    }

    _players.push_back(player_ptr);
    return true;
}

bool game_state::draw_card(player * curr_player, std::string &err)
{
    if (!is_player_in_game(curr_player))
    {
        err = "Server refused to perform draw_card. Player is not part of the game.";
        return false;
    }
    if (!is_allowed_to_play_now(curr_player))
    {
        err = "It's not this players turn yet.";
        return false;
    }
    if (_is_finished->get_value())
    {
        err = "Could not draw card, because the requested game is already finished.";
        return false;
    }

    if (_draw_pile->is_empty())
    {
        /**
        add a fresh set of cards to the draw pile, this is done because the edge case 
        that all cards are in the players hand and no cards are in the discard pile
        **/
        _draw_pile->setup_game(err);
    }

    card *drawn_card;
    if (_draw_pile->draw(curr_player, drawn_card, err))
    {
        //check uno of previous player
        player* prev_player = this->get_previous_player();

        set_missed_uno_call(false);
        if(curr_player->get_id() == prev_player->get_id()){
            //if current player is previous player he already has 2 cards again
            if (prev_player->get_nof_cards() == 2 && !(prev_player->has_called_uno()))
            {
                _draw_pile->distribute_card(2, prev_player, err);
                set_missed_uno_call(true);
            }
        }else{
            if (prev_player->get_nof_cards() == 1 && !(prev_player->has_called_uno()))
            {
                _draw_pile->distribute_card(2, prev_player, err);
                set_missed_uno_call(true);
            } 
        }
        
        
        //reset uno for all players
        for(int i = 0; i < _players.size(); ++i){
            _players[i]->set_called_uno(false, err);
        }
        
        update_current_player(err);
        return true;
    } else
    {
        return false;
    }
}

bool game_state::play_card(player * curr_player, const std::string &card_id, std::string &err)
{
    if (!is_player_in_game(curr_player))
    {
        err = "Server refused to perform draw_card. Player is not part of the game.";
        return false;
    }
    if (!is_allowed_to_play_now(curr_player))
    {
        err = "It's not this players turn yet.";
        return false;
    }
    if (_is_finished->get_value())
    {
        err = "Could not play card, because the requested game is already finished.";
        return false;
    }

    if (_discard_pile->try_play(card_id, curr_player, err))
    {
        player* prev_player = this->get_previous_player();
        
        //check uno of previous player
        set_missed_uno_call(false);
        if(prev_player->get_id() == curr_player->get_id()){
            //if current player is previous player he already has 0 cards
            if(curr_player->get_nof_cards() == 0 && !(curr_player->has_called_uno())){
                _draw_pile->distribute_card(2, curr_player, err);
                set_missed_uno_call(true);
            }
        }else{
            if(prev_player->get_nof_cards() == 1 && !(prev_player->has_called_uno()))
            {
                _draw_pile->distribute_card(2, prev_player, err);
                set_missed_uno_call(true);
            }
        }

        if (curr_player->get_nof_cards() == 0)
        {
            //check end of game, calculate scores, prepare new round
            wrap_up_round(err);
            return true;
        }

        //reset uno for all players
        for(int i = 0; i < _players.size(); ++i){
            _players[i]->set_called_uno(false, err);
        }

        //execute special cards
        int card_index = _discard_pile->check_special_card();
        if (card_index >= 10)
        {
            switch (card_index)
            {
                case SKIP_PLAYER:
                    update_current_player(err);
                    skip_player(err);
                    break;
                case REVERSE_DIRECTION:
                    reverse_direction();
                    update_current_player(err);
                    break;
                case DRAW_2:
                    update_current_player(err);
                    _draw_pile->distribute_card(2, this->get_current_player(), err);
                    skip_player(err);
                    break;
                case WILDCARD:
                    //let it go into draw 4, as case destinction will be made in set_color
                case DRAW_4_WILDCARD:
                    //do not update player yet on purpose
                    set_unanswered_wildcard(true);
                    break;
                case QUESTIONMARK:
                    //redistribute cards evenly among all players
                    execute_questionmark(err, card_id);
                    set_unanswered_wildcard(true);
                    break;
                default:
                    err = "could not execute special card";
            }
        } else
        {
            update_current_player(err);
        }
        return true;
    } else
    {
        return false;
    }

}


#endif

    


// Serializable interface
void game_state::write_into_json(rapidjson::Value &json,
                                 rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const
{
    unique_serializable::write_into_json(json, allocator);

    rapidjson::Value is_finished_val(rapidjson::kObjectType);
    _is_finished->write_into_json(is_finished_val, allocator);
    json.AddMember("is_finished", is_finished_val, allocator);

    rapidjson::Value is_started_val(rapidjson::kObjectType);
    _is_started->write_into_json(is_started_val, allocator);
    json.AddMember("is_started", is_started_val, allocator);

    rapidjson::Value current_player_idx_val(rapidjson::kObjectType);
    _current_player_idx->write_into_json(current_player_idx_val, allocator);
    json.AddMember("current_player_idx", current_player_idx_val, allocator);
    // added previous player idx
    rapidjson::Value previous_player_idx_val(rapidjson::kObjectType);
    _previous_player_idx->write_into_json(previous_player_idx_val, allocator);
    json.AddMember("previous_player_idx", previous_player_idx_val, allocator);

    rapidjson::Value play_direction_val(rapidjson::kObjectType);
    _play_direction->write_into_json(play_direction_val, allocator);
    json.AddMember("play_direction", play_direction_val, allocator);

    rapidjson::Value starting_player_idx_val(rapidjson::kObjectType);
    _starting_player_idx->write_into_json(starting_player_idx_val, allocator);
    json.AddMember("starting_player_idx", starting_player_idx_val, allocator);

    rapidjson::Value round_number_val(rapidjson::kObjectType);
    _round_number->write_into_json(round_number_val, allocator);
    json.AddMember("round_number", round_number_val, allocator);

    rapidjson::Value unanswered_wildcard_val(rapidjson::kObjectType);
    _unanswered_wildcard->write_into_json(unanswered_wildcard_val, allocator);
    json.AddMember("unanswered_wildcard", unanswered_wildcard_val, allocator);

    rapidjson::Value missed_uno_call_val(rapidjson::kObjectType);
    _missed_uno_call->write_into_json(missed_uno_call_val, allocator);
    json.AddMember("missed_uno_call", missed_uno_call_val, allocator);

    rapidjson::Value play_with_questionmark_val(rapidjson::kObjectType);
    _play_with_questionmark->write_into_json(play_with_questionmark_val, allocator);
    json.AddMember("play_with_questionmark", play_with_questionmark_val, allocator);

    rapidjson::Value players_in_new_round_val(rapidjson::kObjectType);
    _players_in_new_round->write_into_json(players_in_new_round_val, allocator);
    json.AddMember("players_in_new_round", players_in_new_round_val, allocator);

    rapidjson::Value draw_pile_val(rapidjson::kObjectType);
    _draw_pile->write_into_json(draw_pile_val, allocator);
    json.AddMember("draw_pile", draw_pile_val, allocator);

    rapidjson::Value discard_pile_val(rapidjson::kObjectType);
    _discard_pile->write_into_json(discard_pile_val, allocator);
    json.AddMember("discard_pile", discard_pile_val, allocator);

    json.AddMember("players", vector_utils::serialize_vector(_players, allocator), allocator);
}


game_state *game_state::from_json(const rapidjson::Value &json)
{
    if (json.HasMember("id")
        && json.HasMember("is_finished")
        && json.HasMember("is_started")
        && json.HasMember("current_player_idx")
        && json.HasMember("previous_player_idx")
        && json.HasMember("play_direction")
        && json.HasMember("round_number")
        && json.HasMember("unanswered_wildcard")
        && json.HasMember("missed_uno_call")
        && json.HasMember("play_with_questionmark")
        && json.HasMember("players_in_new_round")
        && json.HasMember("starting_player_idx")
        && json.HasMember("players")
        && json.HasMember("draw_pile")
        && json.HasMember("discard_pile"))
    {
        std::vector<player *> deserialized_players;
        for (auto &serialized_player: json["players"].GetArray())
        {
            deserialized_players.push_back(player::from_json(serialized_player.GetObject()));
        }
        return new game_state(json["id"].GetString(),
                              draw_pile::from_json(json["draw_pile"].GetObject()),
                              discard_pile::from_json(json["discard_pile"].GetObject()),
                              deserialized_players,
                              serializable_value<bool>::from_json(json["is_started"].GetObject()),
                              serializable_value<bool>::from_json(json["is_finished"].GetObject()),
                              serializable_value<int>::from_json(json["current_player_idx"].GetObject()),
                              serializable_value<int>::from_json(json["previous_player_idx"].GetObject()),
                              serializable_value<int>::from_json(json["play_direction"].GetObject()),
                              serializable_value<int>::from_json(json["round_number"].GetObject()),
                              serializable_value<int>::from_json(json["starting_player_idx"].GetObject()),
                              serializable_value<bool>::from_json(json["unanswered_wildcard"].GetObject()),
                              serializable_value<bool>::from_json(json["missed_uno_call"].GetObject()),
                              serializable_value<bool>::from_json(json["play_with_questionmark"].GetObject()),
                              serializable_value<int>::from_json(json["players_in_new_round"].GetObject()));


    } else
    {
        throw UnoException("Failed to deserialize game_state. Required entries were missing.");
    }
}

