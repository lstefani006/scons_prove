#ifndef __leakfinder_h__
#define __leakfinder_h__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LEAKFINDER_API
#ifdef LEAKFINDER_BUILD
#define LEAKFINDER_API __attribute__((visibility("default")))
#else
#define LEAKFINDER_API
#endif
#endif

LEAKFINDER_API void LeakFinder_StartTrace();
LEAKFINDER_API void LeakFinder_StopTrace(); 
LEAKFINDER_API void LeakFinder_SetCallbacks(void (*cbFreeFailed)(), void (*cbAlloc)(int));

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
struct LEAKFINDER_API LeakFinder {
	LeakFinder() { LeakFinder_StartTrace(); }
	~LeakFinder() { LeakFinder_StopTrace(); }
};
#endif


#endif
