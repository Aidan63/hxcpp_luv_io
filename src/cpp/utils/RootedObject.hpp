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
        RootedObject(hx::Object*);

        ~RootedObject();

        hx::Object* getObject();

        operator hx::Object*() const;
    };
}