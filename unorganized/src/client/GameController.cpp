//
// Created by Software Engineering TA-Team in 2021.
// Updates by UNOrganized on 11.12.2023.
//


#include "GameController.h"
#include "../common/network/requests/join_game_request.h"
#include "../common/network/requests/start_game_request.h"
#include "../common/network/requests/new_round_request.h"
#include "../common/network/requests/draw_card_request.h"
#include "../common/network/requests/call_uno_request.h"
#include "../common/network/requests/play_card_request.h"
#include "../common/network/requests/pick_a_color_request.h"
#include "../common/network/requests/player_left_request.h"
#include "network/ClientNetworkManager.h"

#include "wx/sound.h"



// initialize static members
GameWindow* GameController::_gameWindow = nullptr;
ConnectionPanel* GameController::_connectionPanel = nullptr;
MainGamePanel* GameController::_mainGamePanel = nullptr;
// color dialog is not open at the beginning
bool GameController::_colorDialogOpen = false;
player* GameController::_me = nullptr;
game_state* GameController::_currentGameState = nullptr;


void GameController::init(GameWindow* gameWindow) {

    GameController::_gameWindow = gameWindow;

    // Set up main panels
    GameController::_connectionPanel = new ConnectionPanel(gameWindow);
    GameController::_mainGamePanel = new MainGamePanel(gameWindow);

    // Hide all panels
    GameController::_connectionPanel->Show(false);
    GameController::_mainGamePanel->Show(false);

    // Only show connection panel at the start of the game
    GameController::_gameWindow->showPanel(GameController::_connectionPanel);

    // Set status bar
    GameController::showStatus("Not connected");

    // Bind the event handler for the window close event
    GameController::_gameWindow->Bind(wxEVT_CLOSE_WINDOW, [&](wxCloseEvent& event) {
        // Call the player_left_request function here
        if(_me != nullptr && !(_currentGameState->is_finished())){
            GameController::playerLeft();            
        }
        event.Skip(); // Allow the window to be closed
    });
}


void GameController::connectToServer() {

    // get values form UI input fields
    wxString inputServerAddress = GameController::_connectionPanel->getServerAddress().Trim();
    wxString inputServerPort = GameController::_connectionPanel->getServerPort().Trim();
    wxString inputPlayerName = GameController::_connectionPanel->getPlayerName().Trim();

    // check that all values were provided
    if(inputServerAddress.IsEmpty()) {
        GameController::showError("Input error", "Please provide the server's address");
        return;
    }
    if(inputServerPort.IsEmpty()) {
        GameController::showError("Input error", "Please provide the server's port number");
        return;
    }
    if(inputPlayerName.IsEmpty()) {
        GameController::showError("Input error", "Please enter your desired player name");
        return;
    }

    // convert host from wxString to std::string
    std::string host = inputServerAddress.ToStdString();

    // convert port from wxString to uint16_t
    unsigned long portAsLong;
    if(!inputServerPort.ToULong(&portAsLong) || portAsLong > 65535) {
        GameController::showError("Connection error", "Invalid port");
        return;
    }
    uint16_t port = (uint16_t) portAsLong;

    // convert player name from wxString to std::string
    std::string playerName = inputPlayerName.ToStdString();

    // connect to network
    ClientNetworkManager::init(host, port);

    // send request to join game
    GameController::_me = new player(playerName);
    join_game_request request = join_game_request(GameController::_me->get_id(), GameController::_me->get_player_name(), true);
    ClientNetworkManager::sendRequest(request);

}


void GameController::updateGameState(game_state* newGameState) {

    // the existing game state is now old
    game_state* oldGameState = GameController::_currentGameState;

    // save the new game state as our current game state
    GameController::_currentGameState = newGameState;

    if(oldGameState != nullptr) {

        // check if a new round started, and display message accordingly
        if(oldGameState->get_round_number() > 0 && oldGameState->get_round_number() < newGameState->get_round_number()) {
            GameController::showNewRoundMessage(oldGameState, newGameState);
        }

        //check if questionmarkcard option is chosen, and display message accordingly
        if(oldGameState->get_play_with_questionmark() != newGameState->get_play_with_questionmark()
                && newGameState->get_play_with_questionmark()){
            wxString message= "You chose to play the Uno Version with the questionmarkcard! \n";
            message += "When playing the questionmarkcard, the server will collect all cards and redistribute them equally among the players. \n";
            message += "The rest of the game will be played according to the normal UNO rules.";

            wxMessageBox(message, "Game Configuration", wxOK | wxICON_INFORMATION);
        }

        //check if there was a missed uno call, and display message accordingly
        if(newGameState->get_missed_uno_call() && oldGameState->get_previous_player()->get_id() == _me->get_id()){
            showError("Missed Uno Call", "You missed calling uno and will be given 2 cards");
        }

        //play the sound if a player has called uno
        std::vector<player *> current_players = newGameState->get_players();
        int nof_players = current_players.size();
        for(int i = 0; i < nof_players; ++i){
            if(current_players[i]->has_called_uno()){
                wxSound sound(wxT("assets/uno_sound_16_48k_a.wav"), false);
                if (sound.IsOk()) {
                    sound.Play(wxSOUND_ASYNC);
                }
            }
        }

        // delete the old game state, we don't need it anymore
        delete oldGameState;

        //checks if needs to choose color
        if(newGameState->get_unanswered_wildcard()){
            //in case of questionmarkcard, we want to show the new GamePanel before one has to choose the color
            GameController::_mainGamePanel->buildGameState(GameController::_currentGameState, GameController::_me);
            GameController::showPickColor();
        }
    }
    
    if(GameController::_currentGameState->is_finished()) {
        GameController::showGameOverMessage();
    }

    // make sure we are showing the main game panel in the window (if we are already showing it, nothing will happen)
    GameController::_gameWindow->showPanel(GameController::_mainGamePanel);

    // command the main game panel to rebuild itself, based on the new game state
    GameController::_mainGamePanel->buildGameState(GameController::_currentGameState, GameController::_me);
}

void GameController::WaitForNewRound(game_state* state){
    GameController::_currentGameState = state;
    GameController::_mainGamePanel->buildWaitForNewRound();
}

void GameController::startGame(bool questionmarkbool) {
    start_game_request request = start_game_request(GameController::_currentGameState->get_id(), GameController::_me->get_id(), questionmarkbool);
    ClientNetworkManager::sendRequest(request);
}

void GameController::newRound() {
    new_round_request request = new_round_request(GameController::_currentGameState->get_id(), GameController::_me->get_id());
    ClientNetworkManager::sendRequest(request);
}

void GameController::playerLeft() {
    player_left_request request = player_left_request(GameController::_currentGameState->get_id(), GameController::_me->get_id());
    ClientNetworkManager::sendRequest(request);
}


void GameController::drawCard() {
    draw_card_request request = draw_card_request(GameController::_currentGameState->get_id(), GameController::_me->get_id());
    ClientNetworkManager::sendRequest(request);
}


void GameController::callUno() {
    call_uno_request request = call_uno_request(GameController::_currentGameState->get_id(), GameController::_me->get_id());
    ClientNetworkManager::sendRequest(request);
}


void GameController::playCard(card* cardToPlay) {
    play_card_request request = play_card_request(GameController::_currentGameState->get_id(), GameController::_me->get_id(), cardToPlay->get_id());
    ClientNetworkManager::sendRequest(request);
}

void GameController::pickColor(int color) {
    pick_a_color_request request = pick_a_color_request(GameController::_currentGameState->get_id(), GameController::_me->get_id(), color);
    ClientNetworkManager::sendRequest(request);
}


wxEvtHandler* GameController::getMainThreadEventHandler() {
    return GameController::_gameWindow->GetEventHandler();
}


void GameController::showError(const std::string& title, const std::string& message) {
    wxMessageBox(message, title, wxICON_ERROR);
}

void GameController::showPickColor(){
    if(_currentGameState->get_current_player()->get_id() == _me->get_id() && _colorDialogOpen == false){
        _colorDialogOpen = true;
        UnoColorDialog dialog(wxT("Select Uno Color"));
        int selectedColor_index = -1;
        if (dialog.ShowModal() == wxID_OK)
        {
            wxString selectedColor = dialog.GetSelectedColor();
            selectedColor_index = dialog.GetSelectedColorIndex();
            //wxMessageBox(wxString::Format(wxT("You selected: %s"), selectedColor), wxT("Selection"));
            GameController::pickColor(selectedColor_index);
        }
        _colorDialogOpen = false;
    }
}

void GameController::showStatus(const std::string& message) {
    GameController::_gameWindow->setStatus(message);
}


void GameController::showNewRoundMessage(game_state* oldGameState, game_state* newGameState) {
    std::string title = "Round Completed";
    std::string message = "The updated scores are:\n";
    std::string buttonLabel = "Start next round";

    // add the point differences of all players to the messages
    for(int i = 0; i < oldGameState->get_players().size(); i++) {

        player* oldPlayerState = oldGameState->get_players().at(i);
        player* newPlayerState = newGameState->get_players().at(i);

        int new_score = newPlayerState->get_score();
        int old_score = oldPlayerState->get_score();
        int scoreDelta = new_score - old_score;

        std::string playerName = newPlayerState->get_player_name();
        if(newPlayerState->get_id() == GameController::_me->get_id()) {
            playerName = "You";
        }

        if(scoreDelta == 0){
            std::string scoreText = std::to_string(new_score);
            message += "\n" + playerName + ":     " + scoreText;
        }else{
            std::string scoreText1 = std::to_string(old_score);
            std::string scoreText2 = std::to_string(scoreDelta);
            std::string scoreText3 = std::to_string(new_score);
            message += "\n" + playerName + ":     " + scoreText1 + " + " + scoreText2 + " = " + scoreText3;
        }
        
    }

    wxMessageDialog dialogBox = wxMessageDialog(nullptr, message, title, wxICON_NONE);
    dialogBox.SetOKLabel(wxMessageDialog::ButtonLabel(buttonLabel));
    dialogBox.ShowModal();

    //NewRoundRequest after Dialog is closed
    GameController::newRound();
}


void GameController::showGameOverMessage() {
    std::string title = "Game Over!";
    std::string message;
    if(!GameController::_currentGameState->is_started()){
        message = "All other players already left the game, you need to start a new game to continue.";
    }else if(GameController::_currentGameState->get_players().size() == 1){
        message = "All other players left the game, which makes you the winner of this game. Congratulations! \n You can quit or join another game.";
    }else{
        message = "Final score:\n";

        // sort players by score
        std::vector<player*> players = GameController::_currentGameState->get_players();
        std::sort(players.begin(), players.end(), [](const player* a, const player* b) -> bool {
            return a->get_score() > b->get_score();
        });

        // list all players
        for(int i = 0; i < players.size(); i++) {

            player* playerState = players.at(i);
            std::string scoreText = std::to_string(playerState->get_score());

            // first entry is the winner
            std::string winnerText = "";
            if(i == 0) {
                winnerText = "     Winner!";
            }

            std::string playerName = playerState->get_player_name();
            if(playerState->get_id() == GameController::_me->get_id()) {
                playerName = "You";

                if(i == 0) {
                    winnerText = "     You won!!!";
                }
            }
            message += "\n" + playerName + ":     " + scoreText + winnerText;
        }
    }

    wxMessageDialog dialogBox = wxMessageDialog(nullptr, message, title, wxYES_NO |wxICON_INFORMATION);
    dialogBox.SetYesNoLabels("Start New Game", "Quit");

    int result = dialogBox.ShowModal();
    if(result == wxID_YES){
        join_game_request request = join_game_request(GameController::_me->get_id(), GameController::_me->get_player_name(), false);
        ClientNetworkManager::sendRequest(request);
    }else if(result == wxID_NO){
        GameController::_gameWindow->Close();
    }
}