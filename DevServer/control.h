#pragma once

#include <functional>
#include "aul/connection.h"

namespace devserver
{
    class control
    {
        AUL_DECLARE_SINGLETON(control);
    public:
        std::function<void()> _shutdown_handler;

        void initialize(const mb_string& address, const mb_string& port, connection_family family);
        void initialize_server();
        void listen();
        void stop();

    private:
        connection _listen_conn;
        mb_string _address;
        mb_string _port;
        connection_family _family;
    };
}