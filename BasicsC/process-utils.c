////////////////////////////////////////////////////////////////////////////////
/// @brief collection of process functions
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2004-2012 triagens GmbH, Cologne, Germany
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
/// @author Esteban Lombeyda
/// @author Copyright 2011-2012, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#include "process-utils.h"

// -----------------------------------------------------------------------------
// --SECTION--                                                     private types
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SystemProcess
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief contains all data documented by "proc"
///
/// @see man 5 proc for the state of a process
////////////////////////////////////////////////////////////////////////////////

#ifdef TRI_HAVE_LINUX_PROC

typedef struct process_state_s {
    pid_t pid;
    /* size was choosen arbitrary */
    char comm[128];
    char state;
    int ppid;
    int pgrp;
    int session;
    int tty_nr;
    int tpgid;
    unsigned flags;
    /* lu */
    unsigned long minflt;
    unsigned long cminflt;
    unsigned long majflt;
    unsigned long cmajflt;
    unsigned long utime;
    unsigned long stime;
    unsigned long cutime;
    /* ld */
    long cstime;
    long priority;
    long nice;
    long num_threads;
    long itrealvalue;
    /* llu */
    long long unsigned int starttime;
    /* lu */
    unsigned long vsize;
    /* ld */
    long rss;
    /* lu */
    unsigned long rsslim;
    unsigned long startcode;
    unsigned long endcode;
    unsigned long startstack;
    unsigned long kstkesp;
    unsigned long signal;
    /* obsolete lu*/
    unsigned long blocked;
    unsigned long sigignore;
    unsigned int sigcatch;
    unsigned long wchan;
    /* no maintained lu */
    unsigned long nswap;
    unsigned long cnswap;
    /* d */
    int exit_signal;
    int processor;
    /* u */
    unsigned rt_priority;
    unsigned policy;
    /* llu */
    long long unsigned int delayacct_blkio_ticks;
    /* lu */
    unsigned long guest_time;
    /* ld */
    long cguest_time;
}
process_state_t;

#endif

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                  public functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup SystemProcess
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the size of the current process
////////////////////////////////////////////////////////////////////////////////

unsigned int TRI_ProcessSizeSelf () {
  return TRI_ProcessSize(TRI_CurrentProcessId());
}

////////////////////////////////////////////////////////////////////////////////
/// @brief gets the size of an process
////////////////////////////////////////////////////////////////////////////////

#ifdef TRI_HAVE_LINUX_PROC

unsigned int TRI_ProcessSize (TRI_pid_t pid) {
  char fn[1024];
  int fd;
  unsigned int vsize;

  snprintf(fn, sizeof(fn), "/proc/%d/stat", pid);
  fd = open(fn, O_RDONLY);
  vsize = 0;

  if (fd > 0) {
    char str[1024];
    process_state_t st;

    read(fd, str, 1024);
    close(fd);

    sscanf(str, "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %llu %lu", &st.pid,
           (char*) &st.comm, &st.state, &st.ppid, &st.pgrp, &st.session, &st.tty_nr, &st.tpgid,
           &st.flags, &st.minflt, &st.cminflt, &st.majflt, &st.cmajflt, &st.utime, &st.stime,
           &st.cutime, &st.cstime, &st.priority, &st.nice, &st.num_threads, &st.itrealvalue,
           &st.starttime, &st.vsize);

    vsize = st.vsize;
  }

  return vsize;
}

#else

unsigned int TRI_ProcessSize (TRI_pid_t pid) {
  return 0;
}

#endif

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// Local Variables:
// mode: outline-minor
// outline-regexp: "^\\(/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|// --SECTION--\\|/// @\\}\\)"
// End: