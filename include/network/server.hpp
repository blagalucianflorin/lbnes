//
// Created by blaga on 30-Oct-22.
//

#ifndef LBNES_SERVER_HPP
#define LBNES_SERVER_HPP

#include <memory>


class server
{
private:
    // TODO socket

public:
    server (const char *host, short port);

    void send_data (uint8_t *data, size_t size);

    void receive_data (uint8_t *data, size_t size);
};

#endif //LBNES_SERVER_HPP
