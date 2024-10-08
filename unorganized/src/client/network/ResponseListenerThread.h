//
// Created by Software Engineering TA-Team in 2021.
//

#ifndef UNO_CLIENT_NETWORK_THREAD_H
#define UNO_CLIENT_NETWORK_THREAD_H

#include <functional>
#include <wx/wx.h>
#include "sockpp/tcp_socket.h"
#include "sockpp/tcp_connector.h"


class ResponseListenerThread : public wxThread {

public:
    /**
    * \brief constructor of ResponseListenerThread
    * \param: a sockpp tcp connector
    * \result ResponseListenerthead is constructed
    */
    ResponseListenerThread(sockpp::tcp_connector* connection);

    /**
    * \brief Destructor of ResponseListenerThread
    * \result ResponseListenerthead is destructed
    */
    ~ResponseListenerThread();

protected:
/**
    * \brief extracts incomint message and handles it
    * \result handles message accordingly or returns an error via outputError
    */
    virtual ExitCode Entry();

private:
    /**
    * \brief handles and shows an error
    * \param: error title and message
    * \result shows the error to the client(s)
    */
    void outputError(std::string title, std::string message);


    sockpp::tcp_connector* _connection;

};

#endif //UNO_CLIENT_NETWORK_THREAD_H
