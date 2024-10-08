//
// Created by Software Engineering TA-Team in 2021.
//

#ifndef UNO_CLIENTNETWORKMANAGER_H
#define UNO_CLIENTNETWORKMANAGER_H


#include <string>
#include "ResponseListenerThread.h"
#include "../../common/network/requests/client_request.h"


class ClientNetworkManager {

public:
    /**
    * \brief tries to connect to the server, calls connect
    * \param: host and port
    * \result connects to the server or gets an error
    */
    static void init(const std::string& host, const uint16_t port);

    /**
    * \brief tries to send a request to the server
    * \param: a request
    * \result sent a request to the server or prints an error
    */
    static void sendRequest(const client_request& request);

    /**
    * \brief tries to translate response and process it
    * \param: a request
    * \result processes the response or shows an error
    */
    static void parseResponse(const std::string& message);

private:
    /**
    * \brief creates a sockpp adress and tries to establish connection to given adress
    * \param: host and port
    * \result true if it was able to connect, false otherwise
    */
    static bool connect(const std::string& host, const uint16_t port);


    static sockpp::tcp_connector* _connection;

    static bool _connectionSuccess;
    static bool _failedToConnect;

};


#endif //UNO_CLIENTNETWORKMANAGER_H
