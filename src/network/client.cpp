//
// Created by blaga on 30-Oct-22.
//

#include "network/client.hpp"


client::client (const char *host, const short port)
{
    this -> socket = std::make_shared <TcpClientSocket> (host, port);

    this -> socket -> openConnection ();
}

void client::receive_screen (uint32_t *pixels)
{
    socket -> receiveData (pixels, SCREEN_SIZE);
}
