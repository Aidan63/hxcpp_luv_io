#pragma once

#include <hxcpp.h>

namespace cpp::utils
{
    class RootedObject
    {
    private:
        hx::Object** rooted;

    public:
        RootedObject(void*);
        RootedObject(hx::Object**);
        RootedObject(hx::Object* object);

        ~RootedObject();

        operator hx::Object*() const;
    };
}