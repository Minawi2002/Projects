//
// Created by Manuel on 25.01.2021.
// Updated by UNOrganized on 11.12.2023.
//

#ifndef UNO_DRAW_PILE_H
#define UNO_DRAW_PILE_H

#include "card.h"
#include <vector>
#include <string>
#include <algorithm>
#include "../../serialization/serializable.h"
#include "../../serialization/unique_serializable.h"
#include "../../serialization/serializable_value.h"
#include "../../game_state/player/player.h"
#include "../../../../rapidjson/include/rapidjson/document.h"


class draw_pile : public unique_serializable {
private:
    std::vector<card*> _cards;
    serializable_value<bool>* _questionmarkbool;

    /**
    * \brief private deserialization constructor
    * \param: id, card vector and state
    * \result draw_pile is constructed
    */
    draw_pile(std::string id, std::vector<card*>& cards, serializable_value<bool>* state);

    /**
    * \brief from_diff constructor
    * \param: id
    * \result draw_pile is constructed
    */
    draw_pile(std::string id);


    

public:
    /**
    * \brief public constructor
    * \param: 
    * \result draw_pile is constructed
    */
    draw_pile();

    /**
    * \brief public constructor
    * \param: card vector and state of questionmarkcard
    * \result draw_pile is constructed
    */
    draw_pile(std::vector<card*>& cards, serializable_value<bool>* state);

    /**
    * \brief destructor
    * \param: 
    * \result draw_pile is destructed
    */
    ~draw_pile();

//** pile functions **//
    /**
    * \brief shuffles all cards in the draw pile
    */
    void shuffle();

//** setters **//
    /**
    * \brief sets the questionmarbool
    * \param: state of the questionmarkbool
    */
    void set_questionmarkbool(bool state);

//** accessors **//
    /**
    * \brief returns if pile is empty
    */
    bool is_empty() const noexcept;

    /**
    * \brief returns number of cards in the pile
    */
    int get_nof_cards() const noexcept;

    /**
    * \brief returns the top card of the pile
    */
    card* get_top_card() const;

#ifdef UNO_SERVER
//** state update functions **//
    /**
    * \brief sets up the draw pile for a new game
    * \result fills the draw pile with all cards of the game
    */
    void setup_game(std::string& err);  // Fills the stack with all cards of the game
    /**
    * \brief draws a card from the draw pile
    * \param: the player who wants to draw a card
    * \result true if the card could be drawn, false otherwise. also draws the card
    */
    bool draw(player* player, card*& drawn_card, std::string& err);
    /**
    * \brief adds a fresh set of cards to the draw pile
    * \param: player who wants to draw a card
    * \result new set of cards
    */
    void distribute_card(int nof_cards, player* player, std::string& err);
    /**
    * \brief removes the top card from the draw pile
    * \result returns the top card of the draw pile
    */
    card* remove_top(std::string& err);
#endif

// serialization
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static draw_pile* from_json(const rapidjson::Value& json);
};


#endif //UNO_DRAW_PILE_H
