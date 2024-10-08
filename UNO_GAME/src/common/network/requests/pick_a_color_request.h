//
// Created by UNOrganized on 11.12.2023.
//

#ifndef UNO_PICK_A_COLOR_REQUEST_H
#define UNO_PICK_A_COLOR_REQUEST_H


#include <string>
#include "client_request.h"
#include "../../../../rapidjson/include/rapidjson/document.h"

class pick_a_color_request : public client_request{
    private:
    int _color;

    /**
    * \brief private constructor for deserialization
    * \param: all base class properties that are needed and color index
    * \result pick a color request is constructed
    */
    pick_a_color_request(base_class_properties props, int color);

    public:

    /**
    * \brief get color that is requested
    * \result returns color that is requested
    */
   [[nodiscard]] int get_color() const {return this -> _color;}
    
    /**
    * \brief public constructor
    * \param: game and player id and color index
    * \result pick a color request is constructed
    */
    pick_a_color_request(std::string game_id, std::string player_id, int color);

    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    static pick_a_color_request* from_json(const rapidjson::Value& json);


};

#endif //UNO_PICK_A_ COLOR_REQUEST_H