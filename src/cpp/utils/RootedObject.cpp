#include <hxcpp.h>
#include "RootedObject.hpp"

cpp::utils::RootedObject::RootedObject(void* _baton)
    : rooted(static_cast<hx::Object**>(_baton))
{
    //
}

cpp::utils::RootedObject::RootedObject(hx::Object** _root)
    : rooted(_root)
{
    //
}

cpp::utils::RootedObject::RootedObject(hx::Object* _object)
    : rooted(new hx::Object*(_object))
{
    hx::GCAddRoot(rooted);
}

cpp::utils::RootedObject::~RootedObject()
{
    hx::GCRemoveRoot(rooted);

    delete rooted;
}

cpp::utils::RootedObject::operator hx::Object*() const
{
    return *rooted;
}

hx::Object* cpp::utils::RootedObject::getObject()
{
    return *rooted;
}