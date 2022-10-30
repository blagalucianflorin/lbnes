//
// Created by blaga on 30-Oct-22.
//

#include "network/server.hpp"


server::server (const char *host, const short port)
{
    this -> socket = std::make_shared <TcpServerSocket> (host, port);

    this -> socket -> acceptConnection ();
}

void server::send_screen (uint32_t *pixels)
{
    this -> socket -> sendData (pixels, SCREEN_SIZE);
}
