#include "stdafx.h"
#include "control.h"

namespace
{

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
}