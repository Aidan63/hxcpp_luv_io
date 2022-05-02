#pragma once

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

namespace cpp::utils
{
    class AutoGCZone
    {
    public:
        AutoGCZone();
        ~AutoGCZone();

        void close();

    private:
        bool open;
    };
}