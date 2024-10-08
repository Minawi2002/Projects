//
// Created by Manuel on 27.01.2021.
// Updated by UNOrganized on 11.12.2023.
//

#ifndef UNO_HAND_H
#define UNO_HAND_H

#include <vector>
#include "../../../../rapidjson/include/rapidjson/document.h"
#include "../cards/card.h"

class hand : public unique_serializable {

private:
    std::vector<card*> _cards;

    /**
    * \brief from_diff constructor
    * \param: id string
    * \result hand is constructed
    */
    hand(std::string id);

    /**
    * \brief private deserialization constructor
    * \param: id string and a vector of cards
    * \result hand is constructed
    */
    hand(std::string id, std::vector<card*> cards);

    /**
    * \brief tries to remove card 
    * \param: position of the card
    * \result returns nullptr if it worked
    */
    card* remove_card(std::vector<card*>::iterator pos);

    /**
    * \brief tries to remove card
    * \param: index
    * \result returns nullptr if it worked
    */
    card* remove_card(int idx);

    /**
    * \brief tries to remove card
    * \param: card pointer
    * \result returns nullptr if it worked
    */
    card* remove_card(card* card);

public:
    /**
    * \brief public constructor
    * \param:
    * \result constructs the hand
    */
    hand();

    /**
    * \brief destructor
    * \param: 
    * \result hand is destructed
    */
    ~hand();


//** serializable interface **//
    static hand* from_json(const rapidjson::Value& json);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;

//** accessors **//
    /**
    * \brief get number of cards in hand
    * \param: 
    * \result returns number of cards in hand
    */
    int get_nof_cards() const;

    /**
    * \brief get the score of the hand
    * \param: 
    * \result returns the total value of all cards in the hand
    */
    int get_score() const;

    /**
    * \brief get the cards of the hand
    * \param: 
    * \result returns the cards in the hand
    */
    const std::vector<card*> get_cards() const;

    /**
    * \brief tries to get a card of the hand
    * \param: card id and pointer to the card
    * \result returns true if the pointer is now pointing to the card
    */
    bool try_get_card(const std::string& card_id, card*& hand_card) const;

#ifdef UNO_SERVER
//** state update functions **//
    /**
    * \brief clears the card vector
    * \param: error string
    * \result card vector is clear
    */
    void setup_round(std::string& err);

    /**
    * \brief adds a card to the hand
    * \param: card pointer and error string
    * \result returns true if the card was added to the hand
    */
    bool add_card(card* card, std::string& err);

    /**
    * \brief removes a card from the hand
    * \param: card id, card pointer and error string
    * \result returns true if the card was removed from the hand
    */
    bool remove_card(std::string card_id, card*& played_card, std::string& err);

//** for questionmarkcard **//
    /**
    * \brief gets rid of all the cards except the one with the given id
    * \param card_id: the id of the card that should not be removed
    * \result deletes all card in the players hands except the one with the given id
    */
    void dump_cards_except_played(std::string& err, const std::string &card_id);
#endif

    std::vector<card*>::iterator get_card_iterator();
};


#endif //UNO_HAND_H
