//
// Created by Manuel on 25.01.2021.
// Updated by UNOrganized on 11.12.2023.
//

#ifndef UNO_CARD_H
#define UNO_CARD_H

#include <string>
#include "../../serialization/unique_serializable.h"
#include "../../serialization/serializable_value.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class card : public unique_serializable {
private:
    serializable_value<int>* _index;
    serializable_value<int>* _color;

    /**
    * \brief from_diff constructor
    * \param: id string
    * \result card is constructed
    */
    card(std::string id);
    
    /**
    * \brief private deserialization constructor
    * \param: id string, index and color
    * \result card is constructed
    */
    card(std::string id, serializable_value<int>* ind, serializable_value<int>* col);
public:
    /**
    * \brief public constructor
    * \param: index and color
    * \result card is constructed
    */
    card(int ind, int col);

    /**
    * \brief destructor
    * \result card is destructed
    */
    ~card();

//** accessors **//
    /**
    * \brief gets the index (nuber or special card)
    * \result the index of the card
    */
    int get_index() const noexcept;
    /**
    * \brief gets the color of the card
    * \result the color of the card as an integer (0-4, see cards in assets)
    */
    int get_color() const noexcept;

//** card functions **//
    /**
    * \brief checks if the card can be played on the other card
    * \param: the card to be played on
    * \result true if the card can be played on the other card, false otherwise
    */
    bool can_be_played_on(const card* const other) const noexcept;
    /**
    * \brief changes the color of the card
    * \param: the color to be changed to
    * \result changes the color of the card
    */
    void change_color(int col);

//** serializable interface **//
    void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static card* from_json(const rapidjson::Value& json);
};


#endif //UNO_CARD_H
