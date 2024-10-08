//
// Created by Manuel Nowack on 10.04.21.
// Updates by UNOrganized on 11.12.2023.
//

#include "gtest/gtest.h"
#include "../src/common/exceptions/UnoException.h"
#include "../src/common/game_state/cards/card.h"
#include "../src/common/serialization/json_utils.h"


// A card can be played on another card if and only if the card has the same color or the same value
TEST(CardTest, PlayCardsOn1) {
    card c_0_yellow(0,0);
    card c_0_blue(0,1);
    card c_0_red(0,2);
    card c_0_green(0,3);
    card c_1_yellow(1,0);
    card c_1_blue(1,1);
    card c_1_red(1,2);
    card c_1_green(1,3);
    card c_2_yellow(1,0);
    card c_2_blue(1,1);
    card c_2_red(1,2);
    card c_2_green(1,3);
    card c_3_yellow(2,0);
    card c_3_blue(2,1);
    card c_3_red(2,2);
    card c_3_green(2,3);
    card wildcard(13,4);
    card reverse_green(10,3);
    card reverse_red(10, 2);
    card plus2_blue(11, 1);
    card plus2_green(11,3);
    card skip_green(12,3);
    card skip_yellow(12,0);

    EXPECT_TRUE(c_1_yellow.can_be_played_on((&c_3_yellow)));
    EXPECT_TRUE(c_2_green.can_be_played_on((&c_2_blue)));
    EXPECT_TRUE(c_3_red.can_be_played_on((&c_3_blue)));
    EXPECT_TRUE(c_3_red.can_be_played_on((&c_3_red)));
    EXPECT_TRUE(c_1_green.can_be_played_on((&c_3_green)));
    EXPECT_TRUE(c_0_blue.can_be_played_on((&c_2_blue)));
    EXPECT_TRUE(c_1_red.can_be_played_on((&c_1_blue)));
    EXPECT_TRUE(c_3_red.can_be_played_on((&c_3_green)));


    EXPECT_FALSE(c_3_blue.can_be_played_on((&c_1_green)));
    EXPECT_FALSE(c_0_yellow.can_be_played_on((&c_1_red)));
    EXPECT_FALSE(c_3_red.can_be_played_on((&c_2_blue)));
    EXPECT_FALSE(c_2_green.can_be_played_on((&c_0_blue)));
    EXPECT_FALSE(c_0_green.can_be_played_on((&c_3_red)));
    EXPECT_FALSE(c_0_red.can_be_played_on((&c_1_yellow)));

    //wildcard tests
    EXPECT_TRUE(wildcard.can_be_played_on(&c_1_yellow));
    EXPECT_TRUE(wildcard.can_be_played_on(&c_2_blue));
    EXPECT_TRUE(wildcard.can_be_played_on(&c_3_red));
    EXPECT_TRUE(wildcard.can_be_played_on(&wildcard));

    wildcard.change_color(3);
    EXPECT_TRUE(c_3_green.can_be_played_on(&wildcard));
    EXPECT_FALSE(c_3_yellow.can_be_played_on(&wildcard));

    //test if the reverse card works
    EXPECT_TRUE(c_0_yellow.can_be_played_on(&skip_yellow));
    EXPECT_FALSE(c_3_red.can_be_played_on(&skip_green));
    EXPECT_TRUE(skip_yellow.can_be_played_on(&c_3_yellow));
    EXPECT_FALSE(skip_green.can_be_played_on((&c_1_red)));

    //test if skipcard works

    //test if +2card works
}

// Serialization and subsequent deserialization must yield the same object
TEST(CardTest, SerializationEquality)
{
    card card_send(1,2);
    rapidjson::Document* json_send = card_send.to_json();
    std::string message = json_utils::to_string(json_send);
    delete json_send;

    rapidjson::Document json_recv = rapidjson::Document(rapidjson::kObjectType);
    json_recv.Parse(message.c_str());
    card* card_recv = card::from_json(json_recv);
    EXPECT_EQ(card_send.get_id(), card_recv->get_id());
    EXPECT_EQ(card_send.get_index(), card_recv->get_index());
    EXPECT_EQ(card_send.get_color(),card_send.get_color());
    delete card_recv;
}

// Deserializing an invalid string must throw a UnoException
TEST(CardTest, SerializationException) {
    rapidjson::Document json = rapidjson::Document(rapidjson::kObjectType);
    json.Parse("not json");
    EXPECT_THROW(card::from_json(json), UnoException);
}

