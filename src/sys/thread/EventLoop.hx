package sys.thread;

import cpp.luv.LuvEventLoop;
import cpp.luv.Luv;

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
	final mutex : Mutex;

	final oneTimeEvents : Array<Null<Void->Void>>;

	final waitLock : Lock;

	var oneTimeEventsIdx = 0;

	var promisedEventsCount = 0;

    final luvLoop : LuvLoop;

	public function new()
	{
		mutex         = new Mutex();
		oneTimeEvents = [];
		waitLock      = new Lock();
        luvLoop       = Luv.allocLoop();
    }

	/**
	 * Schedule event for execution every `intervalMs` milliseconds in current loop.
	 * @param event Function to call.
	 * @param intervalMs Number of miliseconds between calls.
	 * @return Handler for cancelling further calls.
	 */
	public function repeat(event : Void->Void, intervalMs : Int) : EventHandler
	{
		mutex.acquire();
		
		final handle = LuvEventLoop.queueRepeatTask(luvLoop, intervalMs, event);

		mutex.release();

		return new RegularEvent(handle);
	}

	/**
	 * Prevent execution of a previously scheduled event in current loop.
	 * @param eventHandler Hander of the event to cancel.
	 */
	public function cancel(eventHandler : EventHandler) : Void
	{
		mutex.acquire();
		
		final event = (eventHandler : RegularEvent);

		LuvEventLoop.cancelTask(event.handle);

		mutex.release();
	}

	/**
	 * Notify this loop about an upcoming event.
	 * 
	 * This makes the thread to stay alive and wait for as many events as many times
	 * `.promise()` was called. These events should be added via `.runPromised()`
	 */
	public function promise() : Void
	{
		mutex.acquire();
		++promisedEventsCount;
		mutex.release();
	}

	/**
	 * Execute `event` as soon as possible.
	 * @param event Function too call.
	 */
	public function run(event : Void->Void) : Void
	{
		mutex.acquire();
		
		LuvEventLoop.queueRepeatTask(luvLoop, 0, event);

		mutex.release();
	}

	/**
	 * Add previously promised `event` for execution.
	 * @param event Function to call.
	 */
	public function runPromised(event : Void->Void) : Void
	{
		mutex.acquire();
		oneTimeEvents[oneTimeEventsIdx++] = event;
		--promisedEventsCount;
		waitLock.release();
		mutex.release();
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
		if (Luv.runLoop(luvLoop, NoWait))
		{
			return AnyTime(null);
		}
		else
		{
			return Never;
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
		return waitLock.wait(timeout);
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
		Luv.runLoop(luvLoop, Default);
	}
}

abstract EventHandler(RegularEvent) from RegularEvent to RegularEvent {}

private class RegularEvent
{
	public final handle : LuvHandle;

	public function new(_handle)
	{
		handle = _handle;
	}
}