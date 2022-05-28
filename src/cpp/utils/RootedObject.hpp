#pragma once

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

namespace cpp::utils
{
    class RootedObject
    {
    private:
        hx::Object** rooted;

    public:
        RootedObject(void*);
        RootedObject(hx::Object*);
        RootedObject(hx::Object**);

        ~RootedObject();

        operator hx::Object*() const;
    };
}