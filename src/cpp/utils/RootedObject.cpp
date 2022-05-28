#include <hxcpp.h>
#include "RootedObject.hpp"

cpp::utils::RootedObject::RootedObject(void* baton)
    : rooted(reinterpret_cast<hx::Object**>(baton))
{
    //
}

cpp::utils::RootedObject::RootedObject(hx::Object* object)
    : rooted(new hx::Object*(object))
{
    hx::GCAddRoot(rooted);
}

cpp::utils::RootedObject::RootedObject(hx::Object** object)
    : rooted(object)
{
    //
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