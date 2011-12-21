////////////////////////////////////////////////////////////////////////////////
/// @brief mutexes, locks and condition variables in posix
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2010-2011 triagens GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is triAGENS GmbH, Cologne, Germany
///
/// @author Dr. Frank Celler
/// @author Copyright 2011, triagens GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#include "locks.h"

#include <BasicsC/logging.h>

// -----------------------------------------------------------------------------
// --SECTION--                                                             MUTEX
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Threading
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief initialises a new mutex
////////////////////////////////////////////////////////////////////////////////

void TRI_InitMutex (TRI_mutex_t* mutex) {
  pthread_mutex_init(mutex, 0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief destroyes a mutex
////////////////////////////////////////////////////////////////////////////////

void TRI_DestroyMutex (TRI_mutex_t* mutex) {
  pthread_mutex_destroy(mutex);
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                  public functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Threading
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief locks mutex
////////////////////////////////////////////////////////////////////////////////

void TRI_LockMutex (TRI_mutex_t* mutex) {
  int rc;

  rc = pthread_mutex_lock(mutex);

  if (rc != 0) {
    LOG_ERROR("could not lock the mutex: %s", strerror(rc));
    exit(EXIT_FAILURE);
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief unlocks mutex
////////////////////////////////////////////////////////////////////////////////

void TRI_UnlockMutex (TRI_mutex_t* mutex) {
  int rc;

  rc = pthread_mutex_unlock(mutex);

  if (rc != 0) {
    LOG_ERROR("could not release the mutex: %s", strerror(rc));
    exit(EXIT_FAILURE);
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                              SPIN
// -----------------------------------------------------------------------------

#ifdef TRI_HAVE_POSIX_SPIN

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Threading
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief initialises a new spin-lock
////////////////////////////////////////////////////////////////////////////////

void TRI_InitSpin (TRI_spin_t* spinLock) {
  pthread_spin_init(spinLock, 0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief destroyes a spin-lock
////////////////////////////////////////////////////////////////////////////////

void TRI_DestroySpin (TRI_spin_t* spinLock) {
  pthread_spin_destroy(spinLock);
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                  public functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Threading
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief locks spin-lock
////////////////////////////////////////////////////////////////////////////////

void TRI_LockSpin (TRI_spin_t* spinLock) {
  int rc;

  rc = pthread_spin_lock(spinLock);

 if (rc != 0) {
    LOG_ERROR("could not lock the spin-lock: %s", strerror(rc));
    exit(EXIT_FAILURE);
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief unlocks spin-lock
////////////////////////////////////////////////////////////////////////////////

void TRI_UnlockSpin (TRI_spin_t* spinLock) {
  int rc;

  rc = pthread_spin_unlock(spinLock);

  if (rc != 0) {
    LOG_ERROR("could not release the spin-lock: %s", strerror(rc));
    exit(EXIT_FAILURE);
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

#endif

// -----------------------------------------------------------------------------
// --SECTION--                                                   READ-WRITE LOCK
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Threading
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief initialises a new read-write lock
////////////////////////////////////////////////////////////////////////////////

void TRI_InitReadWriteLock (TRI_read_write_lock_t* lock) {
  pthread_rwlock_init(lock, 0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief destroyes a read-write lock
////////////////////////////////////////////////////////////////////////////////

void TRI_DestroyReadWriteLock (TRI_read_write_lock_t* lock) {
  pthread_rwlock_destroy(lock);
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                  public functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Threading
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief read locks read-write lock
////////////////////////////////////////////////////////////////////////////////

void TRI_ReadLockReadWriteLock (TRI_read_write_lock_t* lock) {
  int rc;

  rc = pthread_rwlock_rdlock(lock);

  if (rc != 0) {
    LOG_ERROR("could not read-lock the read-write lock: %s", strerror(rc));
    exit(EXIT_FAILURE);
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief read unlocks read-write lock
////////////////////////////////////////////////////////////////////////////////

void TRI_ReadUnlockReadWriteLock (TRI_read_write_lock_t* lock) {
  int rc;

  rc = pthread_rwlock_unlock(lock);

  if (rc != 0) {
    LOG_ERROR("could not read-unlock the read-write lock: %s", strerror(rc));
    exit(EXIT_FAILURE);
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief write locks read-write lock
////////////////////////////////////////////////////////////////////////////////

void TRI_WriteLockReadWriteLock (TRI_read_write_lock_t* lock) {
  int rc;

  rc = pthread_rwlock_wrlock(lock);

  if (rc != 0) {
    LOG_ERROR("could not read-lock the read-write lock: %s", strerror(rc));
    exit(EXIT_FAILURE);
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief write unlocks read-write lock
////////////////////////////////////////////////////////////////////////////////

void TRI_WriteUnlockReadWriteLock (TRI_read_write_lock_t* lock) {
  int rc = pthread_rwlock_unlock(lock);

  if (rc != 0) {
    LOG_ERROR("could not read-unlock the read-write lock: %s", strerror(rc));
    exit(EXIT_FAILURE);
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                CONDITION VARIABLE
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Threading
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief initialises a new condition variable
////////////////////////////////////////////////////////////////////////////////

void TRI_InitCondition (TRI_condition_t* cond) {
  pthread_cond_init(&cond->_cond, 0);

  cond->_ownMutex = true;
  cond->_mutex = TRI_Allocate(sizeof(pthread_mutex_t));

  pthread_mutex_init(cond->_mutex, 0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief initialises a new condition variable with existing mutex
////////////////////////////////////////////////////////////////////////////////

void TRI_Init2Condition (TRI_condition_t* cond, TRI_mutex_t* mutex) {
  pthread_cond_init(&cond->_cond, 0);

  cond->_ownMutex = false;
  cond->_mutex = mutex;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief destroys a condition variable
////////////////////////////////////////////////////////////////////////////////

void TRI_DestroyCondition (TRI_condition_t* cond) {
  pthread_cond_destroy(&cond->_cond);

  if (cond->_ownMutex) {
    pthread_mutex_destroy(cond->_mutex);
    TRI_Free(cond->_mutex);
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                  public functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Threading
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief signals a condition variable
///
/// Note that you must hold the lock.
////////////////////////////////////////////////////////////////////////////////

void TRI_SignalCondition (TRI_condition_t* cond) {
  pthread_cond_signal(&cond->_cond);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief broad casts a condition variable
///
/// Note that you must hold the lock.
////////////////////////////////////////////////////////////////////////////////

void TRI_BroadcastCondition (TRI_condition_t* cond) {
  pthread_cond_broadcast(&cond->_cond);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief waits for a signal on a condition variable
///
/// Note that you must hold the lock.
////////////////////////////////////////////////////////////////////////////////

void TRI_WaitCondition (TRI_condition_t* cond) {
  pthread_cond_wait(&cond->_cond, cond->_mutex);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief locks the mutex of a condition variable
////////////////////////////////////////////////////////////////////////////////

void TRI_LockCondition (TRI_condition_t* cond) {
  pthread_mutex_lock(cond->_mutex);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief unlocks the mutex of a condition variable
////////////////////////////////////////////////////////////////////////////////

void TRI_UnlockCondition (TRI_condition_t* cond) {
  pthread_mutex_unlock(cond->_mutex);
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// Local Variables:
// mode: outline-minor
// outline-regexp: "^\\(/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|// --SECTION--\\|/// @\\}\\)"
// End: