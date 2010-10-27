//
// C++ Implementation: Thread
//
// Description: 
//
//
// Author: Micke Prag <micke.prag@telldus.se>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "Mutex.h"
#include "common.h"

using namespace TelldusCore;

#ifdef _WINDOWS
	#include <windows.h>
	typedef HANDLE MUTEX_T;
#else
	#include <pthread.h>
	typedef pthread_mutex_t MUTEX_T;
#endif

class Mutex::PrivateData {
public:
	MUTEX_T mutex;
};

Mutex::Mutex() {
	d = new PrivateData;
#ifdef _WINDOWS
	d->mutex = CreateMutex(NULL, FALSE, NULL);
#else
	pthread_mutex_init(&d->mutex, NULL);
#endif
}

Mutex::~Mutex() {
#ifdef _WINDOWS
	CloseHandle(d->mutex);
#else
	pthread_mutex_destroy(&d->mutex);
#endif
	delete d;
}
			
void Mutex::lock() {
#ifdef _WINDOWS
	WaitForSingleObject(d->mutex, INFINITE);
#else
	pthread_mutex_lock(&d->mutex);
#endif
}

void Mutex::unlock() {
#ifdef _WINDOWS
	ReleaseMutex(d->mutex);
#else
	pthread_mutex_unlock(&d->mutex);
#endif
}

		
void LoggedMutex::lock() {
#ifdef _WINDOWS
	debuglog(GetCurrentThreadId(), "Locking");
#endif
	Mutex::lock();
#ifdef _WINDOWS
	debuglog(GetCurrentThreadId(), "Locked");
#endif
}

void LoggedMutex::unlock() {
#ifdef _WINDOWS
	debuglog(GetCurrentThreadId(), "Unlocking");
#endif
	Mutex::unlock();
#ifdef _WINDOWS
	debuglog(GetCurrentThreadId(), "Unlocked");
#endif
}

MutexLocker::MutexLocker(Mutex *m)
	:mutex(m) 
{
	mutex->lock();
}

MutexLocker::~MutexLocker() {
	mutex->unlock();
}
