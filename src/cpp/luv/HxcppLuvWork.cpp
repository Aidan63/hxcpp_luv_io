#include "hxcpp.h"
#include "HxcppLuvWork.hpp"
#include "AutoGCZone.hpp"
#include "RootedObject.hpp"
#include <memory>

void onCallback(uv_work_t* work)
{
	auto top = 0;
	hx::SetTopOfStack(&top, true);

	auto root = std::unique_ptr<cpp::utils::RootedObject>(static_cast<cpp::utils::RootedObject*>(work->data));
	auto task = Dynamic(root->getObject());

	task();

	hx::UnregisterCurrentThread();
}

void onAfter(uv_work_t* work, int status)
{
	delete work;
}

uv_work_t* cpp::luv::work::queue(uv_loop_t* loop, Dynamic task)
{
	auto work = new uv_work_t();
    work->data = new cpp::utils::RootedObject(task.mPtr);

	uv_queue_work(loop, work, onCallback, onAfter);

	return work;
}