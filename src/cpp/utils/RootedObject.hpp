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
        RootedObject();
        RootedObject(void*);
        RootedObject(hx::Object**);
        RootedObject(hx::Object*);

        ~RootedObject();

        hx::Object* getObject() const;
        void setObject(hx::Object*) const;

        operator hx::Object*() const;
    };
}