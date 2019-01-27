#include "stdafx.h"
#include "control.h"
#include "server.h"

namespace
{
    enum class control_msg_type
    {
        STOP
    };

    struct control_msg
    {
        control_msg_type _type;
    };
}

namespace devserver
{
    AUL_IMPLEMENT_SINGLETON(control);

    control::control()
        : _shutdown_handler(nullptr)
    {

    }

    control::~control()
    {

    }

    void control::initialize(const mb_string& address, const mb_string& port, connection_family family)
    {
        _address = address;
        _port = port;
        _family = family;
    }

    void control::initialize_server()
    {
        connection::initialize_server(_listen_conn, _family, connection_protocol::TCP, _port.c_str(), _address.c_str());
        _listen_conn.bind();
        _listen_conn.listen();
    }

    void control::listen()
    {
        if (_listen_conn.is_ready_to_read())
        {
            connection new_conn;
            _listen_conn.accept(new_conn);

            control_msg msg;
            memset(&msg, 0, sizeof(msg));
            int32 msg_size = sizeof(msg);
            new_conn.recv((byte*)&msg, msg_size);


            if (msg._type == control_msg_type::STOP)
            {
                new_conn.close();
                _listen_conn.close();
                server::shutdown_server();
            }
        }
    }

    void control::stop()
    {
        connection new_conn;
        connection::initialize_client(new_conn, _family, connection_protocol::TCP, _port.c_str(), _address.c_str());
        new_conn.connect();
        control_msg msg;
        msg._type = control_msg_type::STOP;
        int32 msg_size = (int32)sizeof(msg);
        new_conn.send((byte*)&msg, msg_size);
        new_conn.close();
    }
}