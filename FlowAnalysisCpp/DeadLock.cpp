#include "Shapes.hpp"

#if defined(_MSC_VER) || defined(__MINGW32__)

#include <windows.h>

#define THREAD HANDLE
#define THREAD_RETURN_TYPE DWORD WINAPI
#define THREAD_CREATE(thread, func) thread = CreateThread(0, 0, func, 0, 0, 0)

#define LOCK CRITICAL_SECTION
#define LOCK_ACQUIRE(lock) EnterCriticalSection(&lock)
#define LOCK_RELEASE(lock) LeaveCriticalSection(&lock)

#define SLEEP(x) Sleep(x)

#else

#if (defined(__SUNPRO_C) || defined(__SUNPRO_CC) || defined(__GNUC__) || defined(__IBMCPP__) || defined(__ghs)) && !defined(__sh__) && !defined(__VXWORKS__) && !defined(__DCC__) && !defined(__nios__) && !defined(__nios2__) && !(defined(__ghs) && (defined(__V850__) || defined(__ppc) || defined(__ARM64__))) && !defined(_ARC)

#include <pthread.h>
#include <unistd.h>

#else

typedef void* pthread_t;
typedef void* pthread_attr_t;
typedef void* pthread_mutex_t;

int pthread_create (pthread_t * __newthread,
                           pthread_attr_t *__attr,
                           void *(*__start_routine) (void *),
                           void * __arg);

int pthread_mutex_lock (pthread_mutex_t *__mutex);

int pthread_mutex_unlock (pthread_mutex_t *__mutex);

void sleep(int millis);

#endif

#define THREAD pthread_t
#define THREAD_RETURN_TYPE void*
#define THREAD_CREATE(thread, func) pthread_create(&thread, 0, &func, 0)

#define LOCK pthread_mutex_t
#define LOCK_ACQUIRE(lock) pthread_mutex_lock(&lock)
#define LOCK_RELEASE(lock) pthread_mutex_unlock(&lock)

#define SLEEP(x) sleep(x)

#endif

#define MAX_OBJECTS 100
#define STEP 20

void assertion(int condition, const char* message)
{
	if (!condition) {
		throw message;
	}
}

static LOCK changePositionMutex;

Circle ring(Point(0,0), 100);
Shape *participants[MAX_OBJECTS];
int participantsCount;
int exitGame;

THREAD_RETURN_TYPE GameLogic_Thread(void*)
{
	// Implementation of function GameLogic_Thread()
	return (THREAD_RETURN_TYPE)0;
}

THREAD_RETURN_TYPE Controller_Thread(void*)
{
	// Implementation of function Controller_Thread()
	return (THREAD_RETURN_TYPE)0;
}

namespace Physics
{
	Point *velocityArray[MAX_OBJECTS];

	THREAD_RETURN_TYPE Physics_Thread(void*)
	{
		while (!exitGame) {
			for(int i = 0; i < participantsCount; i++) {
				LOCK_ACQUIRE(changePositionMutex);
				Point& position = participants[i]->getPosition();
				position.translate(*velocityArray[i]);
				assertion(ring.contains(position), "Participant is out of ring");
												// Deadlock: when assertion fails (throws exception)
												//     which cause game freeze
				LOCK_RELEASE(changePositionMutex);
			}
			SLEEP(STEP);
		}
		return 0;
	}
}

namespace Render
{
	Point *currentCameraVelocity;

	void draw(const Shape *object)
	{
		// Implementation of function draw()
	}

	THREAD_RETURN_TYPE Render_Thread(void*)
	{
		while (!exitGame) {
			LOCK_ACQUIRE(changePositionMutex);
			ring.getPosition().translate(*currentCameraVelocity);
			LOCK_RELEASE(changePositionMutex);

			for(int i = 0; i < participantsCount; i++) {
				LOCK_ACQUIRE(changePositionMutex);
				participants[i]->getPosition().translate(*currentCameraVelocity);
				LOCK_RELEASE(changePositionMutex);
				draw(participants[i]);
			}
			SLEEP(STEP);
		}
		return 0;
	}
}

void runGameThreads()
{
	exitGame = 0;
	THREAD thread1, thread2, thread3, thread4;
	THREAD_CREATE(thread1, GameLogic_Thread);
	THREAD_CREATE(thread2, Controller_Thread);
	THREAD_CREATE(thread3, Physics::Physics_Thread);
	THREAD_CREATE(thread4, Render::Render_Thread);
}

