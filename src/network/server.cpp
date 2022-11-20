//
// Created by blaga on 30-Oct-22.
//

#include "network/server.hpp"


server::server (const char *host, const short port)
{
    this -> socket = std::make_shared <TcpServerSocket> (host, port);

    this -> socket -> acceptConnection ();
}

void server::send_data (uint8_t *data, size_t size)
{
    this -> socket -> sendData (data, size);
}

void server::receive_data (uint8_t *data, size_t size)
{
    this -> socket -> receiveData (data, size);
}
