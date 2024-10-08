//
// Created by Manuel on 12.02.2021.
//
// The server_network_manager handles all incoming messages and offers functionality to broadcast messages
// to all connected players of a game.

#ifndef UNO_SERVER_NETWORK_MANAGER_H
#define UNO_SERVER_NETWORK_MANAGER_H

#include <thread>
#include <functional>
#include <unordered_map>
#include <shared_mutex>

#include "sockpp/tcp_socket.h"
#include "sockpp/tcp_connector.h"
#include "sockpp/tcp_acceptor.h"

#include "../common/network/requests/client_request.h"
#include "../common/network/responses/server_response.h"
#include "../common/game_state/player/player.h"
#include "../common/game_state/game_state.h"

class server_network_manager {
private:

    inline static server_network_manager* _instance;
    inline static std::shared_mutex _rw_lock;
    inline static sockpp::tcp_acceptor _acc;

    inline static std::unordered_map<std::string, std::string> _player_id_to_address;
    inline static std::unordered_map<std::string, sockpp::tcp_socket> _address_to_socket;

    /**
    * \brief tries to create a server
    * \param: url and port
    * \result starts a server with an endless listener loop
    */
    void connect(const std::string& url, const uint16_t  port);

    /**
    * \brief creates an endless listener loop
    * \result creates an endless listener loop
    */
    static void listener_loop();

    /**
    * \brief runs in a thread and reads anything coming in on the 'socket'.
    * \param: a socket and a message-handler function
    * \result runs in a thread and reads anything coming in on the 'socket'.
    */
    static void read_message(sockpp::tcp_socket socket,
                             const std::function<void(const std::string&, const sockpp::tcp_socket::addr_t&)>& message_handler);
    
    /**
    * \brief handles incoming message
    * \param: a message string and a socket adress
    * \result handles incoming message
    */
    static void handle_incoming_message(const std::string& msg, const sockpp::tcp_socket::addr_t& peer_address);
    
    /**
    * \brief sends a message
    * \param: a message string and adress of the socket
    * \result sends the desired message
    */
    static ssize_t send_message(const std::string& msg, const std::string& address);
public:
    /**
    * \brief constructor of server network manager
    * \result server network manager is constructed
    */
    server_network_manager();
    
    /**
    * \brief destructor of server network manager
    * \result server network manager is destructed
    */
    ~server_network_manager();

    /**
    * \brief broadcasts message to players
    * \param: a message, a vector of players and pointer to excluded player
    * \result message is broadcasted to all players that are not excluded
    */
    static void broadcast_message(server_response& msg, const std::vector<player*>& players, const player* exclude);
    
    /**
    * \brief removes a player that left the server
    * \param: player id
    * \result removed the player that left the server
    */
    static void on_player_left(std::string player_id);
};



#endif //UNO_SERVER_NETWORK_MANAGER_H
