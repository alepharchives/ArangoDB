////////////////////////////////////////////////////////////////////////////////
/// @brief any server
///
/// @file
/// This file contains a server template.
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
/// @author Copyright 2010-2011, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#include "AnyServer.h"

#include <sys/wait.h>

#ifdef TRI_HAVE_SYS_PRCTL_H
#include <sys/prctl.h>
#endif

#include <fstream>

#include "ApplicationServer/ApplicationServer.h"
#include "Basics/FileUtils.h"
#include "Basics/safe_cast.h"
#include "BasicsC/process-utils.h"
#include "Logger/Logger.h"

using namespace std;
using namespace triagens;
using namespace triagens::basics;
using namespace triagens::rest;

// -----------------------------------------------------------------------------
// --SECTION--                                                 private functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Rest
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief checks a pid file
////////////////////////////////////////////////////////////////////////////////

static void CheckPidFile (string const& pidFile) {

  // check if the pid-file exists
  if (! pidFile.empty()) {
    if (FileUtils::isDirectory(pidFile)) {
      LOGGER_FATAL << "pid-file '" << pidFile << "' is a directory";
      TRI_FlushLogging();
      exit(EXIT_FAILURE);
    }
    else if (FileUtils::exists(pidFile) && FileUtils::size(pidFile) > 0) {
      LOGGER_INFO << "pid-file '" << pidFile << "' already exists, verifying pid";

      ifstream f(pidFile.c_str());

      // file can be opened
      if (f) {
        int oldPid;

        f >> oldPid;

        if (oldPid == 0) {
          LOGGER_FATAL << "pid-file '" << pidFile << "' is unreadable";
          TRI_FlushLogging();
          exit(EXIT_FAILURE);
        }

        LOGGER_DEBUG << "found old pid: " << oldPid;

        int r = kill(oldPid, 0);

        if (r == 0) {
          LOGGER_FATAL << "pid-file '" << pidFile << "' exists and process with pid " << oldPid << " is still running";
          TRI_FlushLogging();
          exit(EXIT_FAILURE);
        }
        else if (errno == EPERM) {
          LOGGER_FATAL << "pid-file '" << pidFile << "' exists and process with pid " << oldPid << " is still running";
          TRI_FlushLogging();
          exit(EXIT_FAILURE);
        }
        else if (errno == ESRCH) {
          LOGGER_ERROR << "pid-file '" << pidFile << "' exists, but no process with pid " << oldPid << " exists";

          if (! FileUtils::remove(pidFile)) {
            LOGGER_FATAL << "pid-file '" << pidFile << "' exists, no process with pid " << oldPid << " exists, but pid-file cannot be removed";
            TRI_FlushLogging();
            exit(EXIT_FAILURE);
          }

          LOGGER_INFO << "removed stale pid-file '" << pidFile << "'";
        }
        else {
          LOGGER_FATAL << "pid-file '" << pidFile << "' exists and kill " << oldPid << " failed";
          TRI_FlushLogging();
          exit(EXIT_FAILURE);
        }
      }

      // failed to open file
      else {
        LOGGER_FATAL << "pid-file '" << pidFile << "' exists, but cannot be opened";
        TRI_FlushLogging();
        exit(EXIT_FAILURE);
      }
    }

    LOGGER_DEBUG << "using pid-file '" << pidFile << "'";
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief writes a pid file
////////////////////////////////////////////////////////////////////////////////

static void WritePidFile (string const& pidFile, int pid) {
  ofstream out(pidFile.c_str(), ios::trunc);

  if (! out) {
    cerr << "cannot write pid-file \"" << pidFile << "\"\n";
    exit(EXIT_FAILURE);
  }

  out << pid;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief forks a new process
////////////////////////////////////////////////////////////////////////////////

static int forkProcess (string const& workingDirectory, string& current, ApplicationServer* applicationServer) {

  // fork off the parent process
  pid_t pid = fork();

  if (pid < 0) {
    LOGGER_FATAL << "cannot fork";
    exit(EXIT_FAILURE);
  }

  // Upon successful completion, fork() shall return 0 to the child process and 
  // shall return the process ID of the child process to the parent process.

  // if we got a good PID, then we can exit the parent process
  if (pid > 0) {
    LOGGER_DEBUG << "started child process with pid " << pid;
    return pid;
  }

  // reset the logging
  TRI_InitialiseLogging(TRI_ResetLogging());
  applicationServer->setupLogging();

  // change the file mode mask
  umask(0);

  // create a new SID for the child process
  pid_t sid = setsid();

  if (sid < 0) {
    cerr << "cannot create sid\n";
    exit(EXIT_FAILURE);
  }

  // store current working directory
  int err = 0;
  current = FileUtils::currentDirectory(&err);

  if (err != 0) {
    cerr << "cannot get current directory\n";
    exit(EXIT_FAILURE);
  }

  // change the current working directory (TODO must be configurable)
  if (! workingDirectory.empty()) {
    if (! FileUtils::changeDirectory(workingDirectory)) {
      cerr << "cannot change into directory '" << workingDirectory << "'\n";
      exit(EXIT_FAILURE);
    }
    else {
      LOGGER_INFO << "changed into directory '" << workingDirectory << "'";
    }
  }

  // close the standard file descriptors
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Rest
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief constructor
////////////////////////////////////////////////////////////////////////////////

AnyServer::AnyServer ()
  : _daemonMode(false),
    _supervisorMode(false),
    _pidFile(""),
    _workingDirectory(""),
    _applicationServer(0) {
}

////////////////////////////////////////////////////////////////////////////////
/// @brief destructor
////////////////////////////////////////////////////////////////////////////////

AnyServer::~AnyServer () {
  if (_applicationServer != 0) {
    delete _applicationServer;
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                    public methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Rest
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief start the server
////////////////////////////////////////////////////////////////////////////////

int AnyServer::start () {
  if (_applicationServer == 0) {
    buildApplicationServer();
  }
  
  if (_supervisorMode) {
    return startupSupervisor();
  }
  else if (_daemonMode) {
    return startupDaemon();
  }
  else {
    if (! _pidFile.empty()) {
      CheckPidFile(_pidFile);
      WritePidFile(_pidFile, TRI_CurrentProcessId());
    }

    prepareServer();
    int res = startupServer();

    if (! _pidFile.empty()) {
      if (! FileUtils::remove(_pidFile)) {
        LOGGER_ERROR << "cannot unlink pid file '" << _pidFile << "'";
      }
    }

    return res;
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                 protected methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Rest
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief prepares the server for startup
////////////////////////////////////////////////////////////////////////////////

void AnyServer::prepareServer () {
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                   private methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup Rest
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief starts a supervisor
////////////////////////////////////////////////////////////////////////////////

int AnyServer::startupSupervisor () {
  static time_t MIN_TIME_ALIVE_IN_SEC = 30;

  LOGGER_INFO << "starting up in supervisor mode";
  
  CheckPidFile(_pidFile);

  string current;
  int result = forkProcess(_workingDirectory, current, safe_cast<ApplicationServer*>(_applicationServer));
  
  // main process
  if (result != 0) {
    return 0;
  }
  
  // child process
  else {
    time_t startTime = time(0);
    time_t t;
    bool done = false;
    result = 0;
    
    while (! done) {
      
      // fork of the server
      pid_t pid = fork();
      
      if (pid < 0) {
        exit(EXIT_FAILURE);
      }
      
      // parent
      if (0 < pid) {
        char const* title = "arangodb [supervisor]";

        TRI_SetProcessTitle(title);

#ifdef TRI_HAVE_SYS_PRCTL_H
        prctl(PR_SET_NAME, title, 0, 0, 0);
#endif

        int status;
        waitpid(pid, &status, 0);
        bool horrible = true;
        
        if (WIFEXITED(status)) {

          // give information about cause of death
          if (WEXITSTATUS(status) == 0) {
            LOGGER_INFO << "child " << pid << " died of natural causes";
            done = true;
            horrible = false;
          }
          else {
            t = time(0) - startTime;

            LOGGER_ERROR << "child " << pid << " died a horrible death, exit status " << WEXITSTATUS(status);
            

            if (t < MIN_TIME_ALIVE_IN_SEC) {
              LOGGER_FATAL << "child only survived for " << t << " seconds, this will not work - please fix the error first";
              done = true;
            }
            else {
              done = false;
            }
          }
        }
        else if (WIFSIGNALED(status)) {
          switch (WTERMSIG(status)) {
            case 2:
            case 9:
            case 15:
              LOGGER_INFO << "child " << pid << " died of natural causes " << WTERMSIG(status);
              done = true;
              horrible = false;
              break;
              
            default:
              t = time(0) - startTime;

              LOGGER_ERROR << "child " << pid << " died a horrible death, signal " << WTERMSIG(status);

              if (t < MIN_TIME_ALIVE_IN_SEC) {
                LOGGER_FATAL << "child only survived for " << t << " seconds, this will not work - please fix the error first";
                done = true;
              }
              else {
                done = false;
              }

              break;
          }
        }
        else {
          LOGGER_ERROR << "child " << pid << " died a horrible death, unknown cause";
          done = false;
        }

        // remove pid file
        if (horrible) {
          if (FileUtils::changeDirectory(current)) {
            if (! FileUtils::remove(_pidFile)) {
              LOGGER_ERROR << "cannot unlink pid file '" << _pidFile << "'";
            }
          }
          else {
            LOGGER_ERROR << "cannot unlink pid file '" << _pidFile << "', because directory '"
                         << current << "' is missing";
          }
        }
      }
      
      // child
      else {
        
        // write the pid file
        WritePidFile(_pidFile, TRI_CurrentProcessId());

        // reset logging
        TRI_InitialiseLogging(TRI_ResetLogging());
        safe_cast<ApplicationServer*>(_applicationServer)->setupLogging();
        
        // force child to stop if supervisor dies
#ifdef TRI_HAVE_PRCTL
        prctl(PR_SET_PDEATHSIG, SIGTERM, 0, 0, 0);
#endif
        
        // startup server
        prepareServer();
        result = startupServer();
        
        // remove pid file
        if (FileUtils::changeDirectory(current)) {
          if (! FileUtils::remove(_pidFile)) {
            LOGGER_ERROR << "cannot unlink pid file '" << _pidFile << "'";
          }
        }
        else {
          LOGGER_ERROR << "cannot unlink pid file '" << _pidFile << "', because directory '"
                       << current << "' is missing";
        }

        // and stop
        exit(result);
      }
    }
  }
  
  return result;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief starts a daemon
////////////////////////////////////////////////////////////////////////////////

int AnyServer::startupDaemon () {
  LOGGER_INFO << "starting up in daemon mode";
  
  CheckPidFile(_pidFile);

  string current;
  int result = forkProcess(_workingDirectory, current, safe_cast<ApplicationServer*>(_applicationServer));
  
  // main process
  if (result != 0) {
#ifdef TRI_HAVE_SYS_PRCTL_H
    prctl(PR_SET_NAME, "arangodb [daemon]", 0, 0, 0);
#endif

    WritePidFile(_pidFile, result);
  }
  
  // child process
  else {
    
    // and startup server
    prepareServer();
    result = startupServer();
    
    // remove pid file
    if (FileUtils::changeDirectory(current)) {
      if (! FileUtils::remove(_pidFile)) {
        LOGGER_ERROR << "cannot unlink pid file '" << _pidFile << "'";
      }
    }
    else {
      LOGGER_ERROR << "cannot unlink pid file '" << _pidFile << "', because directory '"
                   << current << "' is missing";
    }
  }
  
  return result;
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// Local Variables:
// mode: outline-minor
// outline-regexp: "^\\(/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|// --SECTION--\\|/// @\\}\\)"
// End:
