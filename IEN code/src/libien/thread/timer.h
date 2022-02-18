/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
			   IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
**************************************************/
#pragma once
#include <memory>
#include <uv.h>
#include "./thread.h"

class TimerCallback {
public:
	static IcnUserData wrap(std::shared_ptr<TimerCallback> ptr) {
		void* userData = reinterpret_cast<void*>(new std::shared_ptr<TimerCallback>(ptr));
		IcnUserData wrap;
		wrap.userData = userData;
		wrap.lpfnReleaseUserData = releaseWrap;
		return wrap;
	}

	static void callbackWrap(void* userData) {
		std::shared_ptr<TimerCallback> callback = *reinterpret_cast<std::shared_ptr<TimerCallback>*>(userData);
		callback->onTimer();
	}

	static void releaseWrap(void* userData) {
		std::shared_ptr<TimerCallback>* ptr = reinterpret_cast<std::shared_ptr<TimerCallback>*>(userData);
		delete ptr;
	}

	virtual void onTimer() = 0;
};

class Timer {
public:
	static ICN_TIMER_HANDLE setImmediate(std::shared_ptr<TimerCallback> callback) {
		return IcnSetImmediate(TimerCallback::callbackWrap, TimerCallback::wrap(callback));
	}

	static ICN_TIMER_HANDLE setTimeout(int64_t delayInMicroseconds, std::shared_ptr<TimerCallback> callback) {
		return IcnSetTimeout(delayInMicroseconds, TimerCallback::callbackWrap, TimerCallback::wrap(callback));
	}

	static ICN_TIMER_HANDLE setInterval(int64_t delayInMicroseconds, std::shared_ptr<TimerCallback> callback) {
		return IcnSetInterval(delayInMicroseconds, TimerCallback::callbackWrap, TimerCallback::wrap(callback));
	}

public:
	Timer()
		: _isStartPending(0)
		, _activeTimeSeq(1)
		, _timer(NULL) {
		uv_mutex_init(&this->_lock);
	}

	~Timer() {
		this->stop();
		uv_mutex_destroy(&this->_lock);
	}

	bool start(std::shared_ptr<TimerCallback> callback, int32_t timeout) {
		uint32_t curActiveSeq = 0;
		{
			uv_mutex_lock(&this->_lock);
			if (this->_timer == NULL 
				&& !this->_isStartPending) {
				this->_isStartPending = 1;
				curActiveSeq = this->_activeTimeSeq;
			}
			uv_mutex_unlock(&this->_lock);
		}

		if (curActiveSeq == 0) {
			return false;
		}

		ICN_TIMER_HANDLE newTimer = Timer::setTimeout(timeout, callback);

		bool timerHasDestroy = false;

		{
			uv_mutex_lock(&this->_lock);
			if (this->_activeTimeSeq == curActiveSeq) {
				this->_timer = newTimer;
				this->_isStartPending = 0;
			} else {
				timerHasDestroy = true;
			}
			uv_mutex_unlock(&this->_lock);
		}

		if (timerHasDestroy) {
			IcnClearTimer(newTimer);
		}
		return newTimer != NULL;
	}

	void stop() {
		ICN_TIMER_HANDLE oldTimer = NULL;

		{
			uv_mutex_lock(&this->_lock);
			this->_activeTimeSeq++;
			oldTimer = this->_timer;
			this->_timer = NULL;
			this->_isStartPending = 0;
			uv_mutex_unlock(&this->_lock);
		}

		if (oldTimer != NULL) {
			IcnClearTimer(oldTimer);
		}
	}

	bool restart(std::shared_ptr<TimerCallback> callback, int32_t timeout) {
		this->stop();
		return this->start(callback, timeout);
	}

private:
	uv_mutex_t _lock;
	uint32_t _isStartPending;
	uint32_t _activeTimeSeq;
	ICN_TIMER_HANDLE _timer;
};