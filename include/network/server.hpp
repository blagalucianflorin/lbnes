//
// Created by blaga on 30-Oct-22.
//

#ifndef LBNES_SERVER_HPP
#define LBNES_SERVER_HPP

#include "TcpServerSocket.hpp"

#include <memory>

#define SCREEN_SIZE (240 * 256 * 4)


class server
{
private:
    std::shared_ptr<TcpServerSocket> socket;

public:
    server (const char *host, short port);

    void send_screen (uint32_t *pixels);
};

#endif //LBNES_SERVER_HPP
