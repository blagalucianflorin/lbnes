//
// Created by blaga on 30-Oct-22.
//

#include "network/client.hpp"


client::client (const char *host, const short port)
{
    this -> socket = std::make_shared <TcpClientSocket> (host, port);

    this -> socket -> openConnection ();
}

void client::receive_data (uint8_t *data, size_t size)
{
    this -> socket -> receiveData (data, size);
}

void client::send_data (uint8_t *data, size_t size)
{
    this -> socket -> sendData (data, size);
}
