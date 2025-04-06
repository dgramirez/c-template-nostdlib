#ifndef INCLUDE_NOSTD_PLATFORM_COMMON_MCS_LOCK_H
#define INCLUDE_NOSTD_PLATFORM_COMMON_MCS_LOCK_H

// Note: This code came from a wonderful source in where it proved
//       the MCS contention-free lock solves the critical section problem
//
// Read: https://web.archive.org/web/20140411142823/http://www.cise.ufl.edu/tr/DOC/REP-1992-71.pdf

/////////////////////////////
// Structures & Prototypes //
/////////////////////////////
typedef struct _mcs_lock {
	struct _mcs_lock *next;
	u32 locked;
} MCSLock;

typedef struct {
	MCSLock me;
	MCSLock *lock;
} MCSMutex;

local void
__mcs_lock(MCSLock *lock,
           MCSLock *me);

local void
__mcs_unlock(MCSLock *lock,
             MCSLock *me);

local void
mcs_lock(MCSMutex *m);

local void
mcs_unlock(MCSMutex *m);

//////////////////////////
// Function Definitions //
//////////////////////////
local void
__mcs_lock(MCSLock *lock, MCSLock *me)
{
	MCSLock *pred;
	int spin;

	atomic_store(&me->next, 0);
	atomic_store(&me->locked, 0);

	pred = (MCSLock *)atomic_xchg64(lock, (isz)me);
	if (pred) {
		atomic_store(&me->locked, 1);
		atomic_store(&pred->next, (isz)me);
		spin = 300;
		while (me->locked) {
			if ((spin--) > 0)
				cpu_relax();
			else
				__thread_wait(&me->locked, 1);
		}
	}
}

local void
__mcs_unlock(MCSLock *lock, MCSLock *me)
{
	isz _me = (isz)me;
	if (!me->next) {
		if (atomic_cmpxchg(lock, _me, 0) == _me) {
			return;
		}

		do {
			cpu_relax();
		} while (!me->next);
	}

	if (atomic_load(&me->next->locked) == 1) {
		atomic_store(&me->next->locked, 0);
		__thread_wake_one(&me->next->locked);
	}
	else
		atomic_store(&me->next->locked, 0);
}

local void
mcs_lock(MCSMutex *m)
{
	__mcs_lock(m->lock, &m->me);
}

local void
mcs_unlock(MCSMutex *m)
{
	__mcs_unlock(m->lock, &m->me);
}

#endif // INCLUDE_NOSTD_PLATFORM_COMMON_MCS_LOCK_H

