//
// Created by Software Engineering TA-Team in 2021.
// Updates by UNOrganized on 11.12.2023.
//

#include "MainGamePanel.h"
#include "../uiElements/ImagePanel.h"
#include "../uiElements/Scoreboard.h"
#include "../GameController.h"


MainGamePanel::MainGamePanel(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(960, 680)) {
}

void MainGamePanel::buildGameState(game_state* gameState, player* me) {

    // remove any existing UI
    this->DestroyChildren();

    std::vector<player*> players = gameState->get_players();
    int numberOfPlayers = players.size();
    int roundNumber = gameState->get_round_number();

    // find our own player object in the list of players
    int myPosition = -1;
    std::vector<player*>::iterator it = std::find_if(players.begin(), players.end(), [me](const player* x) {
        return x->get_id() == me->get_id();
    });
    if (it < players.end()) {
        me = *it;
        myPosition = it - players.begin();
    } else {
        GameController::showError("Game state error", "Could not find this player among players of server game.");
        return;
    }

    double anglePerPlayer = MainGamePanel::twoPi / (double) numberOfPlayers;

    // show all other players
    for(int i = 1; i < numberOfPlayers; i++) {

        // get player at i-th position after myself
        player* otherPlayer = players.at((myPosition + i) % numberOfPlayers);

        double playerAngle = (double) i * anglePerPlayer;
        int side = (2 * i) - numberOfPlayers; // side < 0 => right, side == 0 => center, side > 0 => left

        this->buildOtherPlayerHand(gameState, otherPlayer, playerAngle);
        this->buildOtherPlayerLabels(gameState, otherPlayer, playerAngle, side);
    }

    // show both card piles at the center
    this->buildCardPiles(gameState, me);

    // show turn indicator below card piles
    this->buildTurnIndicator(gameState, me);

    // show our own player
    this->buildThisPlayer(gameState, me);

    // show UNO button
    this->buildUnoButton(gameState, me);

    // show scoreboard
    this->buildScoreboard(gameState, me, myPosition);
    
    // update layout
    this->Layout();
}

void MainGamePanel::buildWaitForNewRound(){
    // remove any existing UI
    this->DestroyChildren();

    wxBoxSizer* outerLayout = new wxBoxSizer(wxHORIZONTAL);
    this->SetSizer(outerLayout);
    wxBoxSizer* innerLayout = new wxBoxSizer(wxVERTICAL);
    outerLayout->Add(innerLayout, 1, wxALIGN_CENTER);

    wxStaticText* waitingForPlayers = buildStaticText(
            "waiting for all players to start next round ...",
            wxDefaultPosition,
            wxSize(200, 100),
            wxALIGN_CENTER
    );
    innerLayout->Add(waitingForPlayers, 0, wxALIGN_CENTER | wxALL, 10);

    // update layout
    this->Layout();
}

void MainGamePanel::buildScoreboard(game_state* gameState, player *me, int myPosition) {
    //will only be shown if game has already started
    if (gameState->is_started()) {
        std::vector<player*> players = gameState->get_players();
        int numberOfPlayers = players.size();
        int scoreboard_height = 24 + numberOfPlayers * 24;
        wxSize scoreboard_size = wxSize(250, scoreboard_height);
        Scoreboard* scoreboard = new Scoreboard(this, wxID_ANY, wxDefaultPosition, scoreboard_size, wxBORDER_NONE);

        // Add players and update scores
        for(int i = 0; i < numberOfPlayers; i++) {
            player* player = players.at(i);
            std::string player_name = player->get_player_name();
            scoreboard->AddPlayer(player_name);
            scoreboard->UpdateScore(player_name, player->get_score());
        }
        scoreboard->SelectItem(myPosition);
    }
}

void MainGamePanel::buildUnoButton(game_state* gameState, player *me) {
    //will only be shown if game has already started
    if (gameState->is_started()) {
        //determine the position for the UNO button
        wxPoint unoButtonPosition = MainGamePanel::tableCenter + MainGamePanel::unoButtonOffset;

        wxImage unoImage(wxT("assets/uno_hand_0.png"), wxBITMAP_TYPE_PNG);
        wxBitmap unoButtonBitmap(unoImage.Scale(40, 40)); 

        wxBitmapButton *unoButton = new wxBitmapButton(this, wxID_ANY, unoButtonBitmap, unoButtonPosition, unoButtonSize);

        unoButton->Bind(wxEVT_BUTTON, [this, gameState](wxCommandEvent& event){
            if(!gameState->get_missed_uno_call()){
                GameController::callUno();
            }
        });

        unoButton->SetToolTip("Call UNO!");
    }
}

void MainGamePanel::buildOtherPlayerHand(game_state* gameState, player* otherPlayer, double playerAngle) {

    // define the ellipse which represents the virtual player circle
    double horizontalRadius = MainGamePanel::otherPlayerHandDistanceFromCenter * 1.4; // 1.4 to horizontally elongate players' circle
    double verticalRadius = MainGamePanel::otherPlayerHandDistanceFromCenter;

    // get this player's position on that ellipse
    wxPoint handPosition = MainGamePanel::tableCenter;
    handPosition += this->getPointOnEllipse(horizontalRadius, verticalRadius, playerAngle);

    // add image of player's hand
    int numberOfCards = otherPlayer->get_nof_cards();
    if(numberOfCards > 0 && gameState->is_started()) {

        // get new bounds of image, as they increase when image is rotated
        wxSize boundsOfRotatedHand = this->getBoundsOfRotatedSquare(MainGamePanel::otherPlayerHandSize, playerAngle);
        handPosition -= boundsOfRotatedHand / 2;

        std::string handImage = "assets/uno_hand_" + std::to_string(numberOfCards) + ".png";
        if(numberOfCards > 10) {
            handImage = "assets/uno_hand_10.png";
        }
        new ImagePanel(this, handImage, wxBITMAP_TYPE_ANY, handPosition, boundsOfRotatedHand, playerAngle);

    } else {

        wxSize nonRotatedSize = wxSize((int) MainGamePanel::otherPlayerHandSize, (int) MainGamePanel::otherPlayerHandSize);
        handPosition -= nonRotatedSize / 2;

        new ImagePanel(this, "assets/uno_hand_0.png", wxBITMAP_TYPE_ANY, handPosition, nonRotatedSize);
    }
}


void MainGamePanel::buildOtherPlayerLabels(game_state* gameState, player* otherPlayer, double playerAngle, int side) {

    long textAlignment = wxALIGN_CENTER;
    int labelOffsetX = 0;

    if(side < 0) { // right side
        textAlignment = wxALIGN_LEFT;
        labelOffsetX = 85;

    } else if(side > 0) { // left side
        textAlignment = wxALIGN_RIGHT;
        labelOffsetX = -85;
    }

    // define the ellipse which represents the virtual player circle
    double horizontalRadius = MainGamePanel::otherPlayerLabelDistanceFromCenter * 1.25; // 1.25 to horizontally elongate players' circle (but less than the hands' circle)
    double verticalRadius = MainGamePanel::otherPlayerLabelDistanceFromCenter;

    // get this player's position on that ellipse
    wxPoint labelPosition = MainGamePanel::tableCenter;
    labelPosition += this->getPointOnEllipse(horizontalRadius, verticalRadius, playerAngle);
    labelPosition += wxSize(labelOffsetX, 0);

    // if game has not yet started
    if(!gameState->is_started()) {
        this->buildStaticText(
                otherPlayer->get_player_name(),
                labelPosition + wxSize(-100, -18),
                wxSize(200, 18),
                textAlignment,
                true
        );
        this->buildStaticText(
                "waiting...",
                labelPosition + wxSize(-80, 0),
                wxSize(200, 18),
                textAlignment
        );

    //if game has already started
    } else {
        this->buildStaticText(
                otherPlayer->get_player_name(),
                labelPosition + wxSize(-100, -27),
                wxSize(200, 18),
                textAlignment,
                true
        );
        
        // Show other player's status label
        std::string statusText = "waiting...";
        bool bold = false;


        if(otherPlayer == gameState->get_current_player()) {
            statusText = "their turn";
            bold = true;
        }
        this->buildStaticText(
                statusText,
                labelPosition + wxSize(-100, -5),
                wxSize(200, 18),
                textAlignment,
                bold
        );
    }
}


void MainGamePanel::buildCardPiles(game_state* gameState, player *me) {

    //if game has already started
    if(gameState->is_started()) {

        // Show discard pile
        const card* topCard = gameState->get_discard_pile()->get_top_card();
        if(topCard != nullptr) {
            std::string cardImage = "assets/uno_" + std::to_string(topCard->get_color()) +"_"+ std::to_string(topCard->get_index()) + ".png";

            wxPoint discardPilePosition = MainGamePanel::tableCenter + MainGamePanel::discardPileOffset;

            ImagePanel* discardPile = new ImagePanel(this, cardImage, wxBITMAP_TYPE_ANY, discardPilePosition, MainGamePanel::cardSize);
            discardPile->SetToolTip("Discard pile");
        }

        // Show draw pile
        wxPoint drawPilePosition = MainGamePanel::tableCenter + MainGamePanel::drawPileOffset;

        ImagePanel* drawPile = new ImagePanel(this, "assets/uno_back.png", wxBITMAP_TYPE_ANY, drawPilePosition, MainGamePanel::cardSize);

        if(gameState->get_current_player() == me ) { 
            drawPile->SetToolTip("Draw card");
            drawPile->SetCursor(wxCursor(wxCURSOR_HAND));
            drawPile->Bind(wxEVT_LEFT_UP, [](wxMouseEvent& event) {
                GameController::drawCard();
            });
        } else {
            drawPile->SetToolTip("Draw pile");
        }

        // Show color box
        if(topCard != nullptr) {
            std::string color = std::to_string(topCard->get_color()); 
            std::string cardImage = "assets/uno_" + color + ".png";

            wxPoint colorBoxPosition = MainGamePanel::tableCenter + MainGamePanel::colorBoxOffset;

            ImagePanel* colorBox = new ImagePanel(this, cardImage, wxBITMAP_TYPE_ANY, colorBoxPosition, MainGamePanel::boxSize);
            colorBox->SetToolTip("Color Box");
        }

    //if the game did not start yet
    } else {
        // show a back side of a card in the center (only for the mood)
        wxPoint cardPosition = MainGamePanel::tableCenter - (MainGamePanel::cardSize / 2);
        new ImagePanel(this, "assets/uno_back.png", wxBITMAP_TYPE_ANY, cardPosition, MainGamePanel::cardSize);
    }

}

void MainGamePanel::buildTurnIndicator(game_state *gameState, player *me) {

    if(gameState->is_started() && gameState->get_current_player() != nullptr) {

        std::string turnIndicatorText = "It's " + gameState->get_current_player()->get_player_name() + "'s turn!";
        if(gameState->get_current_player() == me) {
            turnIndicatorText = "It's your turn!";
        }

        wxPoint turnIndicatorPosition = MainGamePanel::tableCenter + MainGamePanel::turnIndicatorOffset;

        this->buildStaticText(
                turnIndicatorText,
                turnIndicatorPosition,
                wxSize(200, 18),
                wxALIGN_CENTER,
                true
        );
    }
}


void MainGamePanel::buildThisPlayer(game_state* gameState, player* me) {

    // Setup two nested box sizers, in order to align our player's UI to the bottom center
    wxBoxSizer* outerLayout = new wxBoxSizer(wxHORIZONTAL);
    this->SetSizer(outerLayout);
    wxBoxSizer* innerLayout = new wxBoxSizer(wxVERTICAL);
    outerLayout->Add(innerLayout, 1, wxALIGN_BOTTOM);

    // Show the label with our player name
    wxStaticText* playerName = buildStaticText(
            me->get_player_name(),
            wxDefaultPosition,
            wxSize(200, 18),
            wxALIGN_CENTER,
            true
    );
    innerLayout->Add(playerName, 0, wxALIGN_CENTER);

    // if the game has not yet started
    if(!gameState->is_started()) {

        wxStaticText* playerPoints = buildStaticText(
                "waiting for game to start...",
                wxDefaultPosition,
                wxSize(200, 18),
                wxALIGN_CENTER
        );
        innerLayout->Add(playerPoints, 0, wxALIGN_CENTER | wxBOTTOM, 8);

        //show checkbox that allows player to choose to play with questionmarkcard
        _playWithQuestionMarkCheckBox = new wxCheckBox(this, wxID_ANY, "Play with questionmark card");
        innerLayout->Add(_playWithQuestionMarkCheckBox, 0, wxALIGN_CENTER | wxBOTTOM, 8);

        // show button that allows our player to start the game
        wxButton* startGameButton = new wxButton(this, wxID_ANY, "Start Game!", wxDefaultPosition, wxSize(160, 64));
        startGameButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
            bool questionmarkbool = _playWithQuestionMarkCheckBox->IsChecked();
            GameController::startGame(questionmarkbool);
        });
        innerLayout->Add(startGameButton, 0, wxALIGN_CENTER | wxBOTTOM, 8);

    //if game has already started
    } else {

        // if it's not our turn, display "waiting..."
        if (gameState->get_current_player() != me){
            wxStaticText *playerStatus = buildStaticText(
                    "waiting...",
                    wxDefaultPosition,
                    wxSize(200, 32),
                    wxALIGN_CENTER
            );
            innerLayout->Add(playerStatus, 0, wxALIGN_CENTER | wxBOTTOM, 10);
        }

        // display our player's hand, if we have cards
        int numberOfCards = me->get_nof_cards();
        if (numberOfCards > 0) {

            // create horizontal layout for the individual hand cards of our player
            wxBoxSizer *handLayout = new wxBoxSizer(wxHORIZONTAL);
            innerLayout->Add(handLayout, 0, wxALIGN_CENTER);

            wxSize scaledCardSize = MainGamePanel::cardSize;

            // Adjust card size (if the number of cards does not fit on the screen)
            if (numberOfCards * (MainGamePanel::cardSize.GetWidth() + 8) >
                MainGamePanel::panelSize.GetWidth()) { // 8 -> 4 pixel padding on both sides
                int scaledCardWidth = (MainGamePanel::panelSize.GetWidth() / numberOfCards) - 8;
                double cardAspectRatio =
                        (double) MainGamePanel::cardSize.GetHeight() / (double) MainGamePanel::cardSize.GetWidth();
                int scaledCardHeight = (int) ((double) scaledCardWidth * cardAspectRatio);
                scaledCardSize = wxSize(scaledCardWidth, scaledCardHeight);
            }

            // Show all cards
            for (int i = 0; i < me->get_hand()->get_cards().size(); i++) {
                card *handCard = me->get_hand()->get_cards().at(i);
                std::string cardFile = "assets/uno_" + std::to_string(handCard->get_color()) +"_"+ std::to_string(handCard->get_index()) + ".png";

                ImagePanel *cardButton = new ImagePanel(this, cardFile, wxBITMAP_TYPE_ANY, wxDefaultPosition, scaledCardSize);

                if (gameState->get_current_player() == me ) { 
                    cardButton->SetToolTip("Play card");
                    cardButton->SetCursor(wxCursor(wxCURSOR_HAND));
                    cardButton->Bind(wxEVT_LEFT_UP, [handCard](wxMouseEvent& event) {
                        GameController::playCard(handCard);
                    });
                }
                handLayout->Add(cardButton, 0, wxLEFT | wxRIGHT, 4);
            }
        }
    }
}


wxStaticText* MainGamePanel::buildStaticText(std::string content, wxPoint position, wxSize size, long textAlignment, bool bold) {
    wxStaticText* staticText = new wxStaticText(this, wxID_ANY, content, position, size, textAlignment);
    if(bold) {
        wxFont font = staticText->GetFont();
        font.SetWeight(wxFONTWEIGHT_BOLD);
        staticText->SetFont(font);
    }
    return staticText;
}


wxSize MainGamePanel::getBoundsOfRotatedSquare(double edgeLength, double rotationAngle) {
    double newEdgeLength = this->getEdgeLengthOfRotatedSquare(edgeLength, rotationAngle);
    return wxSize(newEdgeLength, newEdgeLength);
}


double MainGamePanel::getEdgeLengthOfRotatedSquare(double originalEdgeLength, double rotationAngle) {
    return originalEdgeLength * (abs(sin(rotationAngle)) + abs(cos(rotationAngle)));
}


wxPoint MainGamePanel::getPointOnEllipse(double horizontalRadius, double verticalRadius, double angle) {
    return wxPoint((int) (sin(angle) * horizontalRadius), (int) (cos(angle) * verticalRadius));
}
