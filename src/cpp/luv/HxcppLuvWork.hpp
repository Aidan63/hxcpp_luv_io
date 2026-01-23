#pragma once

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

#include <uv.h>

namespace cpp::luv::work
{
	uv_work_t* queue(uv_loop_t*, Dynamic);
}