//
// Created by Manuel on 27.01.2021.
// Updated by UNOrganized on 11.12.2023.
//

#ifndef SPECIAL_CARDS
#define SPECIAL_CARDS
#define SKIP_PLAYER 10
#define REVERSE_DIRECTION 11
#define DRAW_2 12
#define WILDCARD 13
#define DRAW_4_WILDCARD 14
#define QUESTIONMARK 15
#endif

#ifndef UNO_GAME_STATE_H
#define UNO_GAME_STATE_H

#include <vector>
#include <string>
#include <algorithm>
#include "../../rapidjson/include/rapidjson/document.h"
#include "player/player.h"
#include "cards/draw_pile.h"
#include "cards/discard_pile.h"
#include "../serialization/serializable.h"
#include "../serialization/serializable_value.h"
#include "../serialization/unique_serializable.h"

class game_state : public unique_serializable {
private:

    static const int _max_nof_players = 4;
    static const int _min_nof_players = 2;

    std::vector<player*> _players;
    draw_pile* _draw_pile;
    discard_pile* _discard_pile;
    serializable_value<bool>* _is_started;
    serializable_value<bool>* _is_finished;
    serializable_value<int>* _round_number;
    serializable_value<int>* _current_player_idx;
    serializable_value<int>* _previous_player_idx;
    serializable_value<int>* _play_direction;  // 1 or -1 depending on which direction is played in
    serializable_value<int>* _starting_player_idx;
    serializable_value<bool>* _unanswered_wildcard;
    serializable_value<bool>* _missed_uno_call;
    serializable_value<bool>* _play_with_questionmark;
    serializable_value<int>* _players_in_new_round;

    /**
    * \brief from_diff constructor
    * \param: id
    * \result game state is constructed
    */
    game_state(std::string id);

    /**
    * \brief private deserialization constructor
    * \param: all informations that the game state needs
    * \result game state is constructed
    */
    game_state(
            std::string id,
            draw_pile* draw_pile,
            discard_pile* discard_pile,
            std::vector<player*>& players,
            serializable_value<bool>* is_started,
            serializable_value<bool>* is_finished,
            serializable_value<int>* current_player_idx,
            serializable_value<int>* previous_player_idx,
            serializable_value<int>* play_direction,
            serializable_value<int>* round_number,
            serializable_value<int>* starting_player_idx,
            serializable_value<bool>* unanswered_wildcard,
            serializable_value<bool>* missed_uno_call,
            serializable_value<bool>* play_with_questionmark,
            serializable_value<int>* players_in_new_round);
            

    /**
    * \brief returns the index of 'player' in the '_players' vector
    */
    int get_player_index(player* player) const;
public:
    /**
    * \brief public constructor
    * \param: 
    * \result game state is constructed
    */
    game_state();

    /**
    * \brief destructor
    * \param: 
    * \result game state is destructed
    */
    ~game_state();

//** accessors **//
    /**
    * \brief returns if game is full
    */
    bool is_full() const;

    /**
    * \brief returns if game has started
    */
    bool is_started() const;

    /**
    * \brief returns if game is finished
    */
    bool is_finished() const;

    /**
    * \brief returns if the player is in the game
    * \param: player pointer
    */
    bool is_player_in_game(player* player) const;

    /**
    * \brief returns if the player is allowed to play
    * \param: player pointer
    */
    bool is_allowed_to_play_now(player* player) const;

    /**
    * \brief returns all players in a vector
    */
    std::vector<player*>& get_players();

    /**
    * \brief returns the round number
    */
    int get_round_number() const;

    /**
    * \brief returns if there is an unanswered wildcard
    */
    bool get_unanswered_wildcard() const;

    /**
    * \brief returns if there was a missed uno call
    */
    bool get_missed_uno_call() const;

    /**
    * \brief returns if the game is played with the questionmarkcard
    */
    bool get_play_with_questionmark() const;

    /**
    * \brief returns the number of players that already joined the new round
    */
    int get_players_in_new_round() const;

    /**
    * \brief returns the draw pile
    */
    draw_pile* get_draw_pile() const;

    /**
    * \brief returns the discard pile
    */
    discard_pile* get_discard_pile() const;

    /**
    * \brief returns a pointer to the current player
    */
    player* get_current_player() const;

    /**
    * \brief returns a pointer to the previous player
    */
    player* get_previous_player() const;

#ifdef UNO_SERVER
//** server-side state update functions **//¨
    /**
    * \brief server side initialization of the round
    * \param: error string
    * \result sets up card piles and players hands
    */
    void setup_round(std::string& err); 

    /**
    * \brief removes a player from the game
    * \param: player pointer and error string
    * \result returns true if it could remove the player
    */
    bool remove_player(player* player, std::string& err);

    /**
    * \brief adds a player to the game
    * \param: player pointer and error string
    * \result returns true if it could add the player
    */
    bool add_player(player* player, std::string& err);

    /**
    * \brief tries to start the game
    * \param: error string
    * \result returns true if it could start the game
    */
    bool start_game(std::string& err);

    /**
    * \brief adds 1 to the players in new round variable
    * \param: error string
    */
    bool new_round(std::string& err);

    /**
    * \brief tries to draw a card from the draw pile
    * \param: error string
    * \result returns true if it could draw the card
    */
    bool draw_card(player* player, std::string& err);

    /**
    * \brief tries to play a card on the discard pile
    * \param: error string
    * \result returns true if it could play the card
    */
    bool play_card(player* player, const std::string& card_id, std::string& err);
    
    /**
    * \brief skips the next player
    */
    void skip_player(std::string& err);
    
    /**
    * \brief reverses the direction of play
    */
    void reverse_direction();
    
    /**
    * \brief sets the color of the special card on the discard pile
    */
    bool set_color(int color, std::string &err); //used for wildcards
    
    /**
    * \brief collects all the cards of the players and distributes them evenly to the players
    */
    void execute_questionmark(std::string &err, const std::string& card_id);
    
//** variable setters **//
    /**
    * \brief executes the wildcards
    */
    void set_unanswered_wildcard(bool state);
    
    /**
    * \brief if a player forgets to call uno, gives them two cards and informs them why
    */
    void set_missed_uno_call(bool state);
    
    /**
    * \brief chooses whether the questionmark card should be played with or not
    */
    bool set_play_with_questionmark(bool state);

    /**
    * \brief sets the current player to the next player (depending on the direction of play and skips)
    */
    void update_current_player(std::string& err);

    /**
    * \brief calculates the scores at the end of the round
    */
    void wrap_up_round(std::string& err);
#endif

//** serializable interface **//
    static game_state* from_json(const rapidjson::Value& json);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;

};


#endif //UNO_GAME_STATE_H
