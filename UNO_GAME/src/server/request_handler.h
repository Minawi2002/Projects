//
// Created by Manuel on 08.02.2022.
// Updates by UNOrganized on 11.12.2023.
//

#ifndef UNO_REQUEST_HANDLER_H
#define UNO_REQUEST_HANDLER_H

#include "../common/network/responses/server_response.h"
#include "../common/network/requests/client_request.h"
#include "../common/network/responses/request_response.h"

class request_handler {
public:
    /**
    * \brief handles the incoming request and tries to construct a response accordingly
    * \param: pointer to a client request
    * \result request was executed, returns a request response, maybe with an error message
    */
    static request_response* handle_request(const client_request* const req);
};
#endif //UNO_REQUEST_HANDLER_H
