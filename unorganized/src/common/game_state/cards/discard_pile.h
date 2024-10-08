//
// Created by Manuel on 25.01.2021.
// Updated by UNOrganized on 11.12.2023.
//

#ifndef UNO_DISCARD_PILE_H
#define UNO_DISCARD_PILE_H

#include <string>
#include <vector>
#include "card.h"
#include "../player/player.h"
#include "../../serialization/serializable.h"
#include "../../serialization/serializable_value.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class discard_pile: public unique_serializable {
private:
    std::vector<card*> _cards;

    /**
    * \brief from_diff constructor
    * \param: id
    * \result discard pile is constructed
    */
    discard_pile(std::string id);

    /**
    * \brief from_diff constructor
    * \param: id, card vector and state
    * \result discard pile is constructed
    */
    discard_pile(std::string id, std::vector<card*>& cards);
public:
    /**
    * \brief public constructor
    * \result discard pile is constructed
    */
    discard_pile();

    /**
    * \brief destructor
    * \result draw_pile is destructed
    */
    ~discard_pile();

//** accessors **//
    /**
    * \brief checks if the card can be played on the discard pile
    * \param: the card to be played
    * \result true if the card can be played, false otherwise
    */
    bool can_play(const card* card);
    /**
    * \brief gets the top card of the discard pile
    * \result the top card of the discard pile
    */
    card* get_top_card() const;
    /**
    * \brief checks if the top card is a special card
    * \result index of the card
    */
    int check_special_card(); 

#ifdef UNO_SERVER
// state update functions
    /**
    * \brief clears the discard pile
    */
    void setup_game(std::string& err);  // Clears the stack
    /**
    * \brief tries to play a card on the discard pile
    * \param: the id of the card to be played, the player who wants to play the card
    * \result true if the card could be played, false otherwise. also plays the card
    */
    bool try_play(const std::string& card_id, player* player, std::string& err);
    /**
    * \brief tries to play a card on the discard pile
    * \param: the card to be played
    * \result true if the card could be played, false otherwise. also plays the card
    */
    bool try_play(card* played_card, std::string& err);
    /**
    * \brief changes the color of the top card
    * \param: the wildcard, the color to be changed to
    * \result true if the color could be changed, false otherwise. also changes the color
    */
    bool change_color(card* wildcard, int color, std::string& err);
#endif

// serializable interface
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static discard_pile* from_json(const rapidjson::Value& json);
};


#endif //UNO_DISCARD_PILE_H
