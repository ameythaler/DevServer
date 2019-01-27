#include "stdafx.h"
#include "server.h"
#include "toml/toml.h"
#include "aul/exception.h"
#include "aul/string.h"
#include <fstream>
#include <functional>
#include "control.h"

#define __CONFIG_HANDLER() [&](const toml::Value* res)

namespace
{
    void load_config_val(const mb_string& name, toml::Value& v, std::function<void(const toml::Value*)> handler)
    {
        const toml::Value* res = v.find(name.c_str());
        if (res)
        {
            handler(res);
        }
        else
        {
            mb_string msg("Config entry missing for ");
            msg += name;
            throw aul::exception(msg.c_str());
        }
    }
}

namespace devserver
{
    std::atomic<bool> server::_should_run(true);

    mb_string server::start_server(const mb_string& config_file)
    {
        try
        {
            server s(config_file);
            control::instance().initialize(s._config._control_address, s._config._control_port, s._config._control_family);
            control::instance().initialize_server();
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
            control::instance().initialize(s._config._control_address, s._config._control_port, s._config._control_family);
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
        while (_should_run)
        {
            control::instance().listen();
        }
    }

    void server::load_config()
    {
        std::ifstream cfg_stream(_config_file.c_str());
        toml::ParseResult result = toml::parse(cfg_stream);

        if (!result.valid()) throw exception("Invalid config file.");

        toml::Value& v = result.value;
        const toml::Value* res;

        // _local_address
        load_config_val("network.address", v, __CONFIG_HANDLER() { _config._local_address = res->as<mb_string>(); });

        // _port
        load_config_val("network.port", v, __CONFIG_HANDLER() { _config._port = res->as<mb_string>(); });

        // _network_family
        load_config_val("network.family", v, __CONFIG_HANDLER() {
            mb_string scratch = res->as<mb_string>();
            mb_string_to_upper(scratch);
            if (scratch == "IPV4")
            {
                _config._network_family = connection_family::IPV4;
            }
            else if (scratch == "IPV6")
            {
                _config._network_family = connection_family::IPV6;
            }
            else
            {
                throw new exception("network.family entry in config file must be valid value (ipv4 or ipv6).");
            }
        });

        // _worker_threads
        load_config_val("num_worker_threads", v, __CONFIG_HANDLER() { _config._worker_threads = (uint32)res->as<int>(); });

        // _control_address
        load_config_val("control.address", v, __CONFIG_HANDLER() { _config._control_address = res->as<mb_string>(); });

        // _control_port
        load_config_val("control.port", v, __CONFIG_HANDLER() { _config._control_port = res->as<mb_string>(); });

        // _control_family
        load_config_val("control.family", v, __CONFIG_HANDLER() {
            mb_string scratch = res->as<mb_string>();
            mb_string_to_upper(scratch);
            if (scratch == "IPV4")
            {
                _config._control_family = connection_family::IPV4;
            }
            else if (scratch == "IPV6")
            {
                _config._control_family = connection_family::IPV6;
            }
            else
            {
                throw new exception("control.family entry in config file must be valid value (ipv4 or ipv6).");
            }
        });
    }

    void server::remote_shutdown()
    {
        control::instance().stop();
    }
}