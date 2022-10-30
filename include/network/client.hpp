//
// Created by blaga on 30-Oct-22.
//

#ifndef LBNES_CLIENT_HPP
#define LBNES_CLIENT_HPP

#include "TcpClientSocket.hpp"

#include <memory>

#define SCREEN_SIZE (240 * 256 * 4)


class client
{
private:
    std::shared_ptr<TcpClientSocket> socket;

public:
    client (const char *host, short port);

    void receive_screen (uint32_t *pixels);
};

#endif //LBNES_CLIENT_HPP
