#pragma once

#include "aul/connection.h"
#include <atomic>

namespace devserver
{
    class server
    {
        AUL_DISABLECOPYING(server);
    public:
        static mb_string start_server(const mb_string& config_file);
        static mb_string stop_server(const mb_string& config_file);

    private:
        const mb_string& _config_file;

        server(const mb_string& config_file);
        ~server();
        void run();
        void load_config();
        void remote_shutdown();
        static void shutdown_server() { _should_run = false; }

        struct
        {
            connection_family _network_family;
            mb_string _local_address;
            mb_string _port;
            uint32 _worker_threads;
            connection_family _control_family;
            mb_string _control_address;
            mb_string _control_port;
        } _config;

        static std::atomic<bool> _should_run;

        friend class control;
    };

}