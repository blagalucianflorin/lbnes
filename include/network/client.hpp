//
// Created by blaga on 30-Oct-22.
//

#ifndef LBNES_CLIENT_HPP
#define LBNES_CLIENT_HPP

#include <memory>


class client
{
public:
    client (const char *host, short port);

    void receive_data (uint8_t *data, size_t size);

    void send_data (uint8_t *data, size_t size);

private:
    // TODO socket
};

#endif //LBNES_CLIENT_HPP
