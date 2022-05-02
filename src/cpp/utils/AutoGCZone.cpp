#include <hxcpp.h>

#include "AutoGCZone.hpp"

cpp::utils::AutoGCZone::AutoGCZone()
    : open(true)
{
    hx::ExitGCFreeZone();
}

cpp::utils::AutoGCZone::~AutoGCZone()
{
    close();
}

void cpp::utils::AutoGCZone::close()
{
    if (open)
    {
        hx::EnterGCFreeZone();

        open = false;
    }
}