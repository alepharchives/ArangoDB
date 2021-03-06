////////////////////////////////////////////////////////////////////////////////
/// @brief application server
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2004-2012 triAGENS GmbH, Cologne, Germany
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
/// @author Copyright 2009-2012, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_APPLICATION_SERVER_APPLICATION_SERVER_H
#define TRIAGENS_APPLICATION_SERVER_APPLICATION_SERVER_H 1

#include "Basics/Common.h"

#include "Basics/ProgramOptions.h"

// -----------------------------------------------------------------------------
// --SECTION--                                              forward declarations
// -----------------------------------------------------------------------------

namespace triagens {
  namespace basics {
    class ProgramOptionsDescription;
  }

  namespace rest {
    class ApplicationFeature;

// -----------------------------------------------------------------------------
// --SECTION--                                           class ApplicationServer
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ApplicationServer
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief application server
////////////////////////////////////////////////////////////////////////////////

    class ApplicationServer {
      private:
        ApplicationServer (const ApplicationServer&);
        ApplicationServer& operator= (const ApplicationServer&);

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                  public constants
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ApplicationServer
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief Command Line Options
////////////////////////////////////////////////////////////////////////////////

        static string const OPTIONS_CMDLINE;

////////////////////////////////////////////////////////////////////////////////
/// @brief Hidden Options
////////////////////////////////////////////////////////////////////////////////

        static string const OPTIONS_HIDDEN;

////////////////////////////////////////////////////////////////////////////////
/// @brief Limit Options
////////////////////////////////////////////////////////////////////////////////

        static string const OPTIONS_LIMITS;

////////////////////////////////////////////////////////////////////////////////
/// @brief Logger Options
////////////////////////////////////////////////////////////////////////////////

        static string const OPTIONS_LOGGER;

////////////////////////////////////////////////////////////////////////////////
/// @brief Server Options
////////////////////////////////////////////////////////////////////////////////

        static string const OPTIONS_SERVER;

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ApplicationServer
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief constructor
////////////////////////////////////////////////////////////////////////////////

        ApplicationServer (string const& description, string const& version);

////////////////////////////////////////////////////////////////////////////////
/// @brief destructor
////////////////////////////////////////////////////////////////////////////////

        virtual ~ApplicationServer ();

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                    public methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ApplicationServer
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief adds a new feature
////////////////////////////////////////////////////////////////////////////////

        void addFeature (ApplicationFeature*);

////////////////////////////////////////////////////////////////////////////////
/// @brief sets the name of the system config file with a path
////////////////////////////////////////////////////////////////////////////////

        void setSystemConfigFile (string const& name, string const& path);

////////////////////////////////////////////////////////////////////////////////
/// @brief sets the name of the system config file without a path
////////////////////////////////////////////////////////////////////////////////

        void setSystemConfigFile (string const& name);

////////////////////////////////////////////////////////////////////////////////
/// @brief sets the name of the user config file
////////////////////////////////////////////////////////////////////////////////

        void setUserConfigFile (string const& name);

////////////////////////////////////////////////////////////////////////////////
/// @brief sets up the logging
////////////////////////////////////////////////////////////////////////////////

        void setupLogging ();

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the command line options
////////////////////////////////////////////////////////////////////////////////

        basics::ProgramOptions& programOptions ();

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the command line arguments
////////////////////////////////////////////////////////////////////////////////

        vector<string> programArguments ();

////////////////////////////////////////////////////////////////////////////////
/// @brief parses the arguments with empty options description
////////////////////////////////////////////////////////////////////////////////

        bool parse (int argc, char* argv[]);

////////////////////////////////////////////////////////////////////////////////
/// @brief parses the arguments
////////////////////////////////////////////////////////////////////////////////

        bool parse (int argc, char* argv[], map<string, triagens::basics::ProgramOptionsDescription>);

////////////////////////////////////////////////////////////////////////////////
/// @brief prepares the server
////////////////////////////////////////////////////////////////////////////////

        void prepare ();

////////////////////////////////////////////////////////////////////////////////
/// @brief starts the scheduler
////////////////////////////////////////////////////////////////////////////////

        void start ();

////////////////////////////////////////////////////////////////////////////////
/// @brief waits for shutdown
////////////////////////////////////////////////////////////////////////////////

        void wait ();

////////////////////////////////////////////////////////////////////////////////
/// @brief begins shutdown sequence
////////////////////////////////////////////////////////////////////////////////

        void beginShutdown ();

////////////////////////////////////////////////////////////////////////////////
/// @brief shuts down everything
////////////////////////////////////////////////////////////////////////////////

        void shutdown ();

////////////////////////////////////////////////////////////////////////////////
/// @brief raises the privileges
////////////////////////////////////////////////////////////////////////////////

        void raisePrivileges ();

////////////////////////////////////////////////////////////////////////////////
/// @brief drops the privileges
////////////////////////////////////////////////////////////////////////////////

        void dropPrivileges ();

////////////////////////////////////////////////////////////////////////////////
/// @brief drops the privileges permanently
////////////////////////////////////////////////////////////////////////////////

        void dropPrivilegesPermanently ();

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                 protected methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ApplicationServer
/// @{
////////////////////////////////////////////////////////////////////////////////

      protected:

////////////////////////////////////////////////////////////////////////////////
/// @brief adds options to description
////////////////////////////////////////////////////////////////////////////////

        void setupOptions (map<string, basics::ProgramOptionsDescription>&);

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                   private methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ApplicationServer
/// @{
////////////////////////////////////////////////////////////////////////////////

      private:

////////////////////////////////////////////////////////////////////////////////
/// @brief checks if the parent is still alive
////////////////////////////////////////////////////////////////////////////////

        bool checkParent ();

////////////////////////////////////////////////////////////////////////////////
/// @brief reads the configuration files
////////////////////////////////////////////////////////////////////////////////

        bool readConfigurationFile ();

////////////////////////////////////////////////////////////////////////////////
/// @brief extracts the real privileges
////////////////////////////////////////////////////////////////////////////////

        void storeRealPrivileges ();

////////////////////////////////////////////////////////////////////////////////
/// @brief extracts the privileges from the command-line
////////////////////////////////////////////////////////////////////////////////

        void extractPrivileges ();

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                 private variables
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ApplicationServer
/// @{
////////////////////////////////////////////////////////////////////////////////

      private:

////////////////////////////////////////////////////////////////////////////////
/// @brief program options
///
/// @CMDOPT{--help}
///
/// @CMDOPT{-h}
///
/// Prints a list of the most common options available and then
/// exits. In order to see all options use @CODE{--help-all}.
////////////////////////////////////////////////////////////////////////////////

        basics::ProgramOptions _options;


////////////////////////////////////////////////////////////////////////////////
/// @brief command line option description
////////////////////////////////////////////////////////////////////////////////

        basics::ProgramOptionsDescription _description;

////////////////////////////////////////////////////////////////////////////////
/// @brief config file option description
////////////////////////////////////////////////////////////////////////////////

        basics::ProgramOptionsDescription _descriptionFile;

////////////////////////////////////////////////////////////////////////////////
/// @brief command line arguments
////////////////////////////////////////////////////////////////////////////////

        vector<string> _arguments;

////////////////////////////////////////////////////////////////////////////////
/// @brief defined features
////////////////////////////////////////////////////////////////////////////////

        vector<ApplicationFeature*> _features;

////////////////////////////////////////////////////////////////////////////////
/// @brief exit on parent death
////////////////////////////////////////////////////////////////////////////////

        bool _exitOnParentDeath;

////////////////////////////////////////////////////////////////////////////////
/// @brief watch parent
////////////////////////////////////////////////////////////////////////////////

        int _watchParent;

////////////////////////////////////////////////////////////////////////////////
/// @brief shutdown requested
////////////////////////////////////////////////////////////////////////////////

        volatile sig_atomic_t _stopping;

////////////////////////////////////////////////////////////////////////////////
/// @brief title of the application
////////////////////////////////////////////////////////////////////////////////

        string _title;

////////////////////////////////////////////////////////////////////////////////
/// @brief version of the application
///
/// @CMDOPT{--version}
///
/// @CMDOPT{-v}
///
/// Prints the version of the server and exits.
////////////////////////////////////////////////////////////////////////////////

        string _version;

////////////////////////////////////////////////////////////////////////////////
/// @brief config file
///
/// @CMDOPT{--configuration @CA{filename}}
///
/// @CMDOPT{-c @CA{filename}}
///
/// Specifies the name of the configuration file to use.
///
/// If this command is not passed to the server, then by default, the server
/// will attempt to first locate a file named @CODE{~/SERVER/SERVER.conf} in the
/// user's home directory, where @CA{SERVER} is the name of the corresponding
/// server.
///
/// If no such file is found, the server will proceed to look for a file
/// @CODE{/etc/SERVER.conf} in the system configuration directory. The default
/// installation specifies the system configuration directory as
/// /etc. Therefore, in case that no configuration file is found in the user's
/// home directory, the server will proceed to look for a file named
/// @CODE{/etc/SERVER.conf}.
///
/// Only command line options with a value should be set within the
/// configuration file. Command line options which act as flags should be
/// entered on the command line when starting the server.
///
/// White space is ignored. Each option is specified on a separate line in the
/// form
///
/// @verbinclude conf1
///
/// Alternatively, a header section can be specified and options pertaining to
/// that section can be specified in a shorter form
///
/// @verbinclude conf2
///
/// rather than specifying
///
/// @verbinclude conf3
///
/// Comments can be placed in the configuration file, only if the line begins
/// with one or more hash symbols (#).
///
/// There may be occasions where a configuration file exists and the user wishes
/// to override configuration settings stored in a configuration file. Any
/// settings specified on the command line will overwrite the same setting when
/// it appears in a configuration file. If the user wishes to completely ignore
/// configuration files without necessarily deleting the file (or files), then
/// add the command line option
///
/// @verbinclude conf4
///
/// or
///
/// @verbinclude conf5
///
/// when starting up the server. Note that, the word @CODE{none} is
/// case-insensitive.
////////////////////////////////////////////////////////////////////////////////

        string _configFile;

////////////////////////////////////////////////////////////////////////////////
/// @brief user config file
////////////////////////////////////////////////////////////////////////////////

        string _userConfigFile;

////////////////////////////////////////////////////////////////////////////////
/// @brief system config file
////////////////////////////////////////////////////////////////////////////////

        string _systemConfigFile;

////////////////////////////////////////////////////////////////////////////////
/// @brief system config path
////////////////////////////////////////////////////////////////////////////////

        string _systemConfigPath;

////////////////////////////////////////////////////////////////////////////////
/// @brief the user id to use for the process
///
/// @CMDOPT{--uid @CA{uid}}
///
/// The name (identity) of the user the server will run as. If this parameter is
/// not specified, the server will not attempt to change its UID, so that the
/// UID used by the server will be the same as the UID of the user who started
/// the server. If this parameter is specified, then the server will change its
/// UID after opening ports and reading configuration files, but before
/// accepting connections or opening other files (such as recovery files). This
/// is useful when the server must be started with raised privileges (in certain
/// environments) but security considerations require that these privileges be
/// dropped once the server has started work.
///
/// Observe that this parameter cannot be used to bypass operating system
/// security. In general, this parameter (and its corresponding relative gid)
/// can lower privileges but not raise them.
////////////////////////////////////////////////////////////////////////////////

        string _uid;

////////////////////////////////////////////////////////////////////////////////
/// @brief real uid
////////////////////////////////////////////////////////////////////////////////

        uid_t _realUid;

////////////////////////////////////////////////////////////////////////////////
/// @brief effective uid
////////////////////////////////////////////////////////////////////////////////

        uid_t _effectiveUid;

////////////////////////////////////////////////////////////////////////////////
/// @brief the group id to use for the process
///
/// @CMDOPT{--gid @CA{gid}}
///
/// The name (identity) of the group the server will run as. If this parameter
/// is not specified, then the server will not attempt to change its GID, so
/// that the GID the server runs as will be the primary group of the user who
/// started the server. If this parameter is specified, then the server will
/// change its GID after opening ports and reading configuration files, but
/// before accepting connections or opening other files (such as recovery
/// files).
///
/// This parameter is related to the parameter uid.
////////////////////////////////////////////////////////////////////////////////

        string _gid;

////////////////////////////////////////////////////////////////////////////////
/// @brief real gid
////////////////////////////////////////////////////////////////////////////////

        gid_t _realGid;

////////////////////////////////////////////////////////////////////////////////
/// @brief effective gid
////////////////////////////////////////////////////////////////////////////////

        gid_t _effectiveGid;

////////////////////////////////////////////////////////////////////////////////
/// @brief log application name
///
/// @CMDOPT{--log.application @CA{name}}
///
/// Specifies the @CA{name} of the application which should be logged if this item of
/// information is to be logged.
////////////////////////////////////////////////////////////////////////////////

        string _logApplicationName;

////////////////////////////////////////////////////////////////////////////////
/// @brief log host name
///
/// @CMDOPT{--log.hostname @CA{name}}
///
/// Specifies the @CA{name} of the operating environment (the "hostname") which
/// should be logged if this item of information is to be logged. Note that
/// there is no default hostname.
////////////////////////////////////////////////////////////////////////////////

        string _logHostName;

////////////////////////////////////////////////////////////////////////////////
/// @brief log facility
///
/// @CMDOPT{--log.facility @CA{name}}
///
/// Specifies the name of the server instance which should be logged if this
/// item of information is to be logged.
////////////////////////////////////////////////////////////////////////////////

        string _logFacility;

////////////////////////////////////////////////////////////////////////////////
/// @brief log level
///
/// @CMDOPT{--log.level @CA{level}}
///
/// @CMDOPT{--log @CA{level}}
///
/// Allows the user to choose the level of information which is logged by the
/// server. The argument @CA{level} is specified as a string and can be one of
/// the values listed below. Note that, fatal errors, that is, errors which
/// cause the server to terminate, are always logged irrespective of the log
/// level assigned by the user. The variant @c log.level can be used in
/// configuration files, the variant @c log for command line options.
///
/// - fatal: Logs errors which cause the server to terminate.
///
/// Fatal errors generally indicate some inconsistency with the manner in which
/// the server has been coded. Fatal errors may also indicate a problem with the
/// platform on which the server is running. Fatal errors always cause the
/// server to terminate. For example,
///
/// @LIT{2010-09-20T07:32:12Z [4742] FATAL a http server has already been created}
///
/// - error: Logs errors which the server has encountered.
///
/// These errors may not necessarily result in the termination of the
/// server. For example,
///
/// @LIT{2010-09-17T13:10:22Z [13967] ERROR strange log level 'errors'\, going to 'warning'}
///
/// - warning: Provides information on errors encountered by the server,
/// which are not necessarily detrimental to it's continued operation.
///
/// For example,
///
/// @LIT{2010-09-20T08:15:26Z [5533] WARNING got corrupted HTTP request 'POS?'}
///
/// Note that, setting the log level to warning will also result in all errors
/// to be logged as well.
///
/// - info: Logs information about the status of the server.
///
/// For example,
///
/// @LIT{2010-09-20T07:40:38Z [4998] INFO SimpleVOC ready for business}
///
/// Note that, setting the log level to info will also result in all errors and
/// warnings to be logged as well.
///
/// - debug: Logs all errors, all warnings and debug information.
///
/// Debug log information is generally useful to find out the state of the
/// server in the case of an error. For example,
///
/// @LIT{2010-09-17T13:02:53Z [13783] DEBUG opened port 7000 for any}
///
/// Note that, setting the log level to debug will also result in all errors,
/// warnings and server status information to be logged as well.
///
/// - trace: As the name suggests, logs information which may be useful to trace
/// problems encountered with using the server.
///
/// For example,
///
/// @LIT{2010-09-20T08:23:12Z [5687] TRACE trying to open port 8000}
///
/// Note that, setting the log level to trace will also result in all errors,
/// warnings, status information, and debug information to be logged as well.
////////////////////////////////////////////////////////////////////////////////

        string _logLevel;

////////////////////////////////////////////////////////////////////////////////
/// @brief log format
///
/// @CMDOPT{--log.format @CA{format}}
///
/// @copydetails triagens::basics::Logger::setLogFormat
////////////////////////////////////////////////////////////////////////////////

        string _logFormat;

////////////////////////////////////////////////////////////////////////////////
/// @brief log severity
///
/// @CMDOPT{--log.severity @CA{severity}}
///
/// This parameter provides a set of standard log severities which can be
/// used. The currently accepted @CA{severities} are:
///
/// - exception
/// - technical
/// - functional
/// - development
/// - human
/// - all (human and non-human)
/// - non-human (exception, technical, functional, and development)
///
/// The default is all.
////////////////////////////////////////////////////////////////////////////////

        string _logSeverity;

////////////////////////////////////////////////////////////////////////////////
/// @brief log level
///
/// @CMDOPT{--log.file @CA{filename}}
///
/// This option allows the user to specify the name of a file to which
/// information is logged. By default, if no log file is specified, the standard
/// output is used. Note that if the file named by @CA{filename} does not
/// exist, it will be created. If the file cannot be created (e.g. due to 
/// missing file privileges), the server will refuse to start. If the specified 
/// file already exists, output is appended to that file. 
///
/// Use @LIT{+} to log to standard error. Use @LIT{-} to log to standard output. 
/// Use @LIT{""} to disable logging to file.
////////////////////////////////////////////////////////////////////////////////

        string _logFile;

////////////////////////////////////////////////////////////////////////////////
/// @brief log prefix
///
/// @CMDOPT{--log.prefix @CA{prefix}}
///
/// This option is used specify an prefix to logged text.
////////////////////////////////////////////////////////////////////////////////

        string _logPrefix;

////////////////////////////////////////////////////////////////////////////////
/// @brief log level
///
/// @CMDOPT{--log.syslog @CA{arg}}
///
/// If this option is set, then in addition to output being directed to the
/// standard output (or to a specified file, in the case that the command line
/// log.file option was set), log output is also sent to the system logging
/// facility. The @CA{arg} is the system log facility to use. See syslog for
/// further details.
///
/// The value of @CA{arg} depends on your syslog configuration. In general it
/// will be @LIT{user}. Fatal messages are mapped to @LIT{crit}, so if @CA{arg}
/// is @LIT{user}, these messages will be logged as @LIT{user.crit}.  Error
/// messages are mapped to @LIT{err}.  Warnings are mapped to @LIT{warn}.  Info
/// messages are mapped to @LIT{notice}.  Debug messages are mapped to
/// @LIT{info}.  Trace messages are mapped to @LIT{debug}.
////////////////////////////////////////////////////////////////////////////////

        string _logSyslog;

////////////////////////////////////////////////////////////////////////////////
/// @brief log thread identifier
///
/// @CMDOPT{--log.thread}
///
/// Whenever log output is generated, the process ID is written as part of the
/// log information. Setting this option appends the thread id of the calling
/// thread to the process id. For example,
///
/// @LIT{2010-09-20T13:04:01Z [19355] INFO ready for business}
///
/// when no thread is logged and
///
/// @LIT{2010-09-20T13:04:17Z [19371-18446744072487317056] ready for business}
///
/// when this command line option is set.
////////////////////////////////////////////////////////////////////////////////

        bool _logThreadId;

////////////////////////////////////////////////////////////////////////////////
/// @brief log line number
///
/// @CMDOPT{--log.line-number}
///
/// Normally, if an human readable fatal, error, warning or info message is
/// logged, no information about the file and line number is provided. The file
/// and line number is only logged for debug and trace message. This option can
/// be use to always log these pieces of information.
////////////////////////////////////////////////////////////////////////////////

        bool _logLineNumber;

////////////////////////////////////////////////////////////////////////////////
/// @brief log filter
///
/// @CMDOPT{--log.filter @CA{arg}}
///
/// For debug and trace messages, only log those messages occurring in the
/// file @CA{arg}.
////////////////////////////////////////////////////////////////////////////////

        vector<string> _logFilter;

////////////////////////////////////////////////////////////////////////////////
/// @brief random number generator to use
///
/// @CMDOPT{--random.generator @CA{arg}}
///
/// The argument is an integer (1,2,3 or 4) which sets the manner in which
/// random numbers are generated. The default method (3) is to use the a
/// non-blocking random (or pseudorandom) number generator supplied by the
/// operating system. Specifying an argument of 2, uses a blocking random (or
/// pseudorandom) number generator. Specifying an argument 1 sets a pseudorandom
/// number generator using an implication of the Mersenne Twister MT19937
/// algorithm. Algorithm 4 is a combination of the blocking random number
/// generator and the Mersenne Twister.
///
/// @CMDOPT{--random.no-seed}
///
/// By default, the random generator is seeded. Setting this option causes the
/// random number generator not to be seeded. (Seeding the random number
/// generator only occurs if the generator is set to Mersenne, refer to
/// random.generator for details.)
////////////////////////////////////////////////////////////////////////////////

        uint32_t _randomGenerator;
    };
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

#endif

// -----------------------------------------------------------------------------
// --SECTION--                                                       END-OF-FILE
// -----------------------------------------------------------------------------

// Local Variables:
// mode: outline-minor
// outline-regexp: "^\\(/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|/// @page\\|// --SECTION--\\|/// @\\}\\)"
// End:
