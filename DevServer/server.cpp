#include "stdafx.h"
#include "server.h"
#include "toml/toml.h"
#include "aul/exception.h"
#include <fstream>

namespace
{

}

namespace devserver
{
    mb_string server::start_server(const mb_string& config_file)
    {
        try
        {
            server s(config_file);
            s.run();
        }
        catch (std::exception e)
        {
            return e.what();
        }
        return "Stopped.";
    }

    mb_string server::stop_server(const mb_string& config_file)
    {
        try
        {
            server s(config_file);
            s.remote_shutdown();
        }
        catch (std::exception e)
        {
            return e.what();
        }
        return "Stopped.";
    }

    server::server(const mb_string& config_file)
        : _config_file(config_file)
    {
        load_config();
    }

    server::~server()
    {

    }

    void server::run()
    {

    }

    void server::load_config()
    {
        std::ifstream cfg_stream(_config_file.c_str());
        toml::ParseResult result = toml::parse(cfg_stream);

        if (!result.valid()) throw exception("Invalid config file.");

        toml::Value& v = result.value;

        // _local_address
        const toml::Value* res = v.find("network.address");
        if (res)
        {
            _config._local_address = res->as<mb_string>();
        }
        else
        {
            throw exception("Config file missing entry for network.address");
        }

        // _port
        res = v.find("network.port");
        if (res)
        {
            _config._port = res->as<mb_string>();
        }

        // _worker_threads
        res = v.find("num_worker_threads");
        if (res && res->is<int>())
        {
            _config._worker_threads = (uint32)res->as<int>();
        }

        // _control_address
        res = v.find("control.address");
        if (res)
        {
            _config._control_address = res->as<mb_string>();
        }

        // _control_port
        res = v.find("control.port");
        if (res)
        {
            _config._control_port = res->as<mb_string>();
        }
    }

    void server::remote_shutdown()
    {

    }
}