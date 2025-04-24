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
#if ARCH_EXE == 64
	u32 reserved;
#endif
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

	_afn_atstoreW(&me->next, 0);
	_afn_atstoreI(&me->locked, 0);

	pred = (MCSLock *)_afn_atswapW(lock, (isz)me);
	if (pred) {
		_afn_atstoreI(&me->locked, 1);
		_afn_atstoreW(&pred->next, (isz)me);
		spin = 300;
		while (me->locked) {
			if ((spin--) > 0)
				_afn_cpurelax();
			else
				thread_wait(&me->locked, 1);
		}
	}
}

local void
__mcs_unlock(MCSLock *lock, MCSLock *me)
{
	isz _me = (isz)me;
	if (!me->next) {
		if (_afn_atcasW(lock, _me, 0) == _me) {
			return;
		}

		do {
			_afn_cpurelax();
		} while (!me->next);
	}

	if (_afn_atloadI(&me->next->locked) == 1) {
		_afn_atstoreI(&me->next->locked, 0);
		thread_wake_one(&me->next->locked);
	}
	else
		_afn_atstoreI(&me->next->locked, 0);
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

// TODO: Slab Allocator w/ 32-bytes (16-bytes for x32) should suffice.
local AppLock
tlock_create_mcslock(MArena *a)
{
	return marena_alloc(a, sizeof(MCSLock), two_word_size);
}

local AppMLock
mlock_init_mcslock(MArena   *a,
                   AppLock  *_lock,
                   AppMLock *_mlock)
{
	MCSLock  *lock;
	MCSMutex *m;
	MCSMutex *r;

	lock = (MCSLock *)_lock; 
	m    = (MCSMutex *)_mlock;
	if (m && lock) {
		assert(m->lock == lock,
			   "Lock Mismatch: mtx->lock and lock do not match.\n"
			   "Hint: mlock_init doesn't need both lock parameters. "
			   "Without AppLock, it'll use AppMLock's lock. "
			   "Without AppMLock, it'll use the AppLock itself. "
			   "Lastly, without both will create a brand new AppLock.");
	}

	r = marena_alloc(a, sizeof(MCSMutex), two_word_size);

	if (lock) {
		r->lock = lock;
		return r;
	}

	if (m && m->lock) {
		r->lock = m->lock;
		return r;
	}

	r->lock = tlock_create_mcslock(a);
	return r;
}

local void
mlock_acquire_mcslock(AppMLock _lock)
{
	MCSMutex *m = (MCSMutex *)_lock;
	__mcs_lock(m->lock, &m->me);
}

local void
mlock_release_mcslock(AppMLock _lock)
{
	MCSMutex *m = (MCSMutex *)_lock;
	__mcs_unlock(m->lock, &m->me);
}

#endif // INCLUDE_NOSTD_PLATFORM_COMMON_MCS_LOCK_H

