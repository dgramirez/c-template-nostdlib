#ifndef INCLUDE_NOSTD_PLATFORM_COMMON_MCS_LOCK_H
#define INCLUDE_NOSTD_PLATFORM_COMMON_MCS_LOCK_H

// Note: This code came from a wonderful source in where it proved
//       the MCS contention-free lock solves the critical section problem
//
// Read: https://web.archive.org/web/20140411142823/http://www.cise.ufl.edu/tr/DOC/REP-1992-71.pdf

////////////////
// Prototypes //
////////////////
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

local TMutex
mlock_init_mcslock(TLock  _lock,
                   TMutex _mlock)
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
			   "Without TLock, it'll use TMutex's lock. "
			   "Without TMutex, it'll use the TLock itself. "
			   "Lastly, without both will create a brand new TLock.");
	}

	r = mfreelist_alloc(&_sysfl, sizeof(MCSMutex), two_word_size);

	if (lock) {
		r->lock = lock;
		return r;
	}

	if (m && m->lock) {
		r->lock = m->lock;
		return r;
	}

	r->lock = mfreelist_alloc(&_sysfl, sizeof(MCSLock), two_word_size);
	return r;
}

local void
mlock_acquire_mcslock(TMutex _lock)
{
	MCSMutex *m = (MCSMutex *)_lock;
	__mcs_lock(m->lock, &m->me);
}

local void
mlock_release_mcslock(TMutex _lock)
{
	MCSMutex *m = (MCSMutex *)_lock;
	__mcs_unlock(m->lock, &m->me);
}

local void
mlock_free_mcslock(TMutex _lock,
                   u32    free_flags)
{
	MCSMutex *m = (MCSMutex *)_lock;
	if (flag_has(free_flags, MCSLOCK_FREE_LOCK)) {
		mfreelist_free(&_sysfl, m->lock);
		m->lock = 0;
	}
	if (flag_has(free_flags, MCSLOCK_FREE_MUTEX))
		mfreelist_free(&_sysfl, m);
}

#endif // INCLUDE_NOSTD_PLATFORM_COMMON_MCS_LOCK_H

