// DevServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "server.h"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "devserver requires at least one parameter\n";
        return 0;
    }

    if (!strcmp(argv[1], "start"))
    {
        if (argc < 3)
        {
            std::cerr << "devserver start command requires path to config file as additional parameter\n";
            return 0;
        }

        std::cout << "Running devserver...\n";
        std::cout << devserver::server::start_server(argv[2]) << '\n';
    }
    else if (!strcmp(argv[1], "stop"))
    {
        std::cout << "Stopping devserver...\n";
        std::cout << devserver::server::stop_server(argv[2]) << '\n';
    }
    else if (!strcmp(argv[1], "local"))
    {

    }

    return 0;
}

