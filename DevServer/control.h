#pragma once

#include <functional>

namespace devserver
{
    class control
    {
        AUL_DECLARE_SINGLETON(control);
    public:
        std::function<void()> _shutdown_handler;

        void listen();
        void stop();

    private:

    };
}