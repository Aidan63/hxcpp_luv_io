package sys.thread;

import cpp.luv.Async;
import cpp.luv.Timer;
import cpp.luv.Luv;
import haxe.exceptions.NotImplementedException;

/**
 * When an event loop has an available event to execute.
 */
enum NextEventTime
{
	/**
	 * There's already an event waiting to be executed
	 */
	Now;

	/**
	 * No new events are expected.
	 */
	Never;

	/**
	 * An event is expected to arrive at any time.
	 * 
	 * If `time` is specified, then the event will be ready at that time for sure.
	 */
	AnyTime(time : Null<Float>);

	/**
	 * An event is expected to be ready for execution at `time`.
	 */
	At(time : Float);
}

/**
 * An event loop implementation used for `sys.thread.Thread`
 */
@:coreApi
class EventLoop
{
	final lock : Mutex;

	final queue : Array<RegularEvent>;

    final luvLoop : LuvLoop;

	final async : LuvAsync;

	var promisedEvents : Int;

	public function new()
	{
		lock           = new Mutex();
		queue          = [];
        luvLoop        = Luv.allocLoop();
		async          = Async.init(luvLoop, enqueue);
		promisedEvents = 0;

		Async.unref(async);
    }

	/**
	 * Schedule event for execution every `intervalMs` milliseconds in current loop.
	 * @param event Function to call.
	 * @param intervalMs Number of miliseconds between calls.
	 * @return Handler for cancelling further calls.
	 */
	public function repeat(event : Void->Void, intervalMs : Int) : EventHandler
	{
		final handle = new RegularEvent(EventType.Repeat, event, intervalMs);

		lock.acquire();
		
		queue.push(handle);

		lock.release();

		Async.send(async);

		return handle;
	}

	/**
	 * Prevent execution of a previously scheduled event in current loop.
	 * @param eventHandler Hander of the event to cancel.
	 */
	public function cancel(eventHandler : EventHandler) : Void
	{
		run(() -> {
			final event = (eventHandler : RegularEvent);

			Timer.stop(event.handle);
			Timer.close(event.handle);
		});
	}

	/**
	 * Notify this loop about an upcoming event.
	 * 
	 * This makes the thread to stay alive and wait for as many events as many times
	 * `.promise()` was called. These events should be added via `.runPromised()`
	 */
	public function promise() : Void
	{
		run(() -> {
			promisedEvents++;

			referenceAsync();
		});
	}

	/**
	 * Execute `event` as soon as possible.
	 * @param event Function too call.
	 */
	public function run(event : Void->Void) : Void
	{
		lock.acquire();
		
		queue.push(new RegularEvent(EventType.OneShot, event, 0));

		lock.release();

		Async.send(async);
	}

	/**
	 * Add previously promised `event` for execution.
	 * @param event Function to call.
	 */
	public function runPromised(event : Void->Void) : Void
	{
		run(() -> {
			promisedEvents--;

			referenceAsync();

			event();
		});
	}

	/**
	 * Executes all pending events.
	 * 
	 * The returned time stamps can be used with `Sys.time()` for calculations.
	 * Depending on a target platform this method may be non-reentrant. It must not be called from event callbacks.
	 * @return NextEventTime
	 */
	public function progress() : NextEventTime
	{
		return if (Luv.runLoop(luvLoop, NoWait))
		{
			AnyTime(null);
		}
		else
		{
			Never;
		}
	}

	/**
	 * Blocks until a new event is added or `timeout` (in seconds) to expires.
	 * 
	 * Depending on a target platform this method may also automatically execute arriving
	 * events while waiting. However if any event is executed it will stop waiting.
	 * 
	 * - Returns `true` if more events are expected.
	 * - Returns `false` if no more events expected.
	 * 
	 * Depending on a target platform this method may be non-reentrant. It must not be called from event callbacks.
	 * @param timeout Timeout in seconds, or null to not wait.
	 * @return Returns true if the lock is released and false if a time-out occurs.
	 */
	public function wait(?timeout : Float) : Bool
	{
		throw new NotImplementedException();
	}

	/**
	 * Execute all pending events.
	 * 
	 * Wait and execute as many events as many times `promiseEvent()` was called.
	 * Runs until all repeating events are cancelled and no more events is expected.
	 * 
	 * Depending on a target platform this method may be non-reentrant. It must not be called from event callbacks.
	 */
	public function loop() : Void
	{
		enqueue();

		Luv.runLoop(luvLoop, Default);
	}

	function enqueue() : Void
	{
		lock.acquire();

		for (event in queue)
		{
			event.handle = switch event.type
			{
				case OneShot:
					Timer.run(luvLoop, event.task);
				case Repeat:
					Timer.repeat(luvLoop, event.interval, event.task);
			}
		}

		queue.resize(0);

		lock.release();
	}

	function referenceAsync() : Void
	{
		if (promisedEvents > 0)
		{
			Async.ref(async);
		}
		else
		{
			Async.unref(async);
		}
	}
}

abstract EventHandler(RegularEvent) from RegularEvent to RegularEvent {}

private enum abstract EventType(Int)
{
	var OneShot;
	var Repeat;
}

private class RegularEvent
{
	public final type : EventType;

	public final task : Void->Void;

	public final interval : Int;

	public var handle : Null<LuvTimer>;

	public function new(_type, _task, _interval)
	{
		type     = _type;
		task     = _task;
		interval = _interval;
	}
}