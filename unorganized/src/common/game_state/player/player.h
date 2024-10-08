//
// Created by Manuel on 25.01.2021.
// Updated by UNOrganized on 11.12.2023.
//

#ifndef UNO_PLAYER_H
#define UNO_PLAYER_H


#include <string>
#include "hand.h"
#include "../../serialization/uuid_generator.h"
#include "../../../../rapidjson/include/rapidjson/document.h"
#include "../../serialization/unique_serializable.h"
#include "../../serialization/serializable_value.h"

class player : public unique_serializable {
private:
    serializable_value<std::string>* _player_name;
    serializable_value<bool>* _called_uno;
    serializable_value<int>* _score;
    hand* _hand;

#ifdef UNO_SERVER
    std::string _game_id;
#endif

    /**
    * \brief private deserialization constructor
    * \param: id, name, score, hand, called uno
    * \result constructs the player
    */
    player(std::string id,
           serializable_value<std::string>* name,
           serializable_value<int>* score,
           hand* hand,
           serializable_value<bool>* called_uno);

public:
    /**
    * \brief public constructor for client
    * \param: name of the player
    * \result player is constructed
    */
    explicit player(std::string name);  

    /**
    * \brief destructor 
    * \param: 
    * \result player is destructed
    */
    ~player();

#ifdef UNO_SERVER
    /**
    * \brief public constructor for server
    * \param: name of the player
    * \result player is constructed
    */
    player(std::string id, std::string name);

    /**
    * \brief returns the game id for the game the player joined
    */
    std::string get_game_id();

    /**
    * \brief sets the game id of the player to the given game id
    * \param: game id
    */
    void set_game_id(std::string game_id);
#endif

//** accessors **//
    /**
    * \brief checks if the player has called uno
    * \result returns true if the player has called uno
    */
    bool has_called_uno() const noexcept;

    /**
    * \brief get number of cards in players hand
    * \param: 
    * \result returns number of cards in players hand
    */
    int get_nof_cards() const noexcept;

    /**
    * \brief returns players hand
    */
    const hand* get_hand() const noexcept;

    /**
    * \brief returns players name
    */
    std::string get_player_name() const noexcept;
    
    
/**
    * \brief gets the score of the player
    */
    int get_score() const noexcept; 
    /**
    * \brief gets the value of the hand of the player
    */
    int get_hand_score() const noexcept; //returns total points of players current hand

    //change score from game_state
    void set_score(int score);

#ifdef UNO_SERVER
//** state update functions **//
    /**
    * \brief sets the called_uno state of the player
    * \param current state of the player
    * \result true if the state was changed, false if the state was already set to the given state
    */
    bool set_called_uno(bool state, std::string& err);

    /**
    * \brief adds a card to the players hand
    * \param: card pointer and error string
    * \result returns true if the card was added to the hand
    */
    bool add_card(card* card, std::string& err);

    /**
    * \brief removes a card from the players hand
    * \param: card id, card pointer and error string
    * \result returns true if the card was removed from the hand
    */
    bool remove_card(std::string card_id, card*& card, std::string& err);
    
    /**
    * \brief clears the players hand and reset called uno
    * \param: error string
    * \result hand is cleared and called uno is reset
    */
    void setup_round(std::string& err);

//** for questionmarkcard **//
    /**
    * \brief gets rid of all the cards except the one with the given id
    * \param card_id: the id of the card that should not be removed
    * \result deletes all card in the players hands except the one with the given id
    */
    void dump_cards_except_played(std::string& err, const std::string &card_id);
#endif


    // serialization
    static player* from_json(const rapidjson::Value& json);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;

};


#endif //UNO_PLAYER_H
