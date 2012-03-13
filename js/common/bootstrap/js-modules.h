static string JS_common_bootstrap_modules = 
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief JavaScript server functions\n"
  "///\n"
  "/// @file\n"
  "///\n"
  "/// DISCLAIMER\n"
  "///\n"
  "/// Copyright 2010-2011 triagens GmbH, Cologne, Germany\n"
  "///\n"
  "/// Licensed under the Apache License, Version 2.0 (the \"License\");\n"
  "/// you may not use this file except in compliance with the License.\n"
  "/// You may obtain a copy of the License at\n"
  "///\n"
  "///     http://www.apache.org/licenses/LICENSE-2.0\n"
  "///\n"
  "/// Unless required by applicable law or agreed to in writing, software\n"
  "/// distributed under the License is distributed on an \"AS IS\" BASIS,\n"
  "/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
  "/// See the License for the specific language governing permissions and\n"
  "/// limitations under the License.\n"
  "///\n"
  "/// Copyright holder is triAGENS GmbH, Cologne, Germany\n"
  "///\n"
  "/// @author Dr. Frank Celler\n"
  "/// @author Copyright 2011, triAGENS GmbH, Cologne, Germany\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @page JSModulesTOC\n"
  "///\n"
  "/// <ol>\n"
  "///   <li>@ref JSModulesRequire \"require\"</li>\n"
  "/// </ol>\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @page JSModules JavaScript Modules\n"
  "///\n"
  "/// The AvocadoDB uses a <a\n"
  "/// href=\"http://wiki.commonjs.org/wiki/Modules\">CommonJS</a> compatible module\n"
  "/// concept. You can use the function @FN{require} in order to load a\n"
  "/// module. @FN{require} returns the exported variables and functions of the\n"
  "/// module. You can use the option @CO{startup.modules-path} to specify the\n"
  "/// location of the JavaScript files.\n"
  "///\n"
  "/// <hr>\n"
  "/// @copydoc JSModulesTOC\n"
  "/// <hr>\n"
  "///\n"
  "/// @anchor JSModulesRequire\n"
  "/// @copydetails JSF_require\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "// -----------------------------------------------------------------------------\n"
  "// --SECTION--                                                            Module\n"
  "// -----------------------------------------------------------------------------\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @addtogroup V8Module\n"
  "/// @{\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief module cache\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "ModuleCache = {};\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief module constructor\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "function Module (id) {\n"
  "  this.id = id;\n"
  "  this.exports = {};\n"
  "}\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief loads a file and creates a new module descriptor\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "Module.prototype.require = function (path) {\n"
  "  var raw;\n"
  "  var content;\n"
  "  var sandbox;\n"
  "  var paths;\n"
  "  var module;\n"
  "  var f;\n"
  "\n"
  "  // first get rid of any \"..\" and \".\"\n"
  "  path = this.normalise(path);\n"
  "\n"
  "  // check if you already know the module, return the exports\n"
  "  if (path in ModuleCache) {\n"
  "    return ModuleCache[path].exports;\n"
  "  }\n"
  "\n"
  "  // locate file and read content\n"
  "  raw = internal.readFile(path);\n"
  "\n"
  "  // create a new sandbox and execute\n"
  "  ModuleCache[path] = module = new Module(path);\n"
  "\n"
  "  content = \"(function (module, exports, require, print) {\" + raw.content + \"\\n/* end-of-file '\" + raw.path + \"' */ });\";\n"
  "\n"
  "  try {\n"
  "    f = SYS_EXECUTE(content, undefined, path);\n"
  "  }\n"
  "  catch (err) {\n"
  "    CONSOLE_ERROR(\"in file %s: %o\", path, err.stack);\n"
  "    throw err;\n"
  "  }\n"
  "\n"
  "  if (f == undefined) {\n"
  "    throw \"cannot create context function\";\n"
  "  }\n"
  "\n"
  "  f(module, module.exports, function(path) { return module.require(path); }, print);\n"
  "\n"
  "  return module.exports;\n"
  "};\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief normalises a path\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "Module.prototype.normalise = function (path) {\n"
  "  var p;\n"
  "  var q;\n"
  "  var x;\n"
  "\n"
  "  if (path == \"\") {\n"
  "    return this.id;\n"
  "  }\n"
  "\n"
  "  p = path.split('/');\n"
  "\n"
  "  // relative path\n"
  "  if (p[0] == \".\" || p[0] == \"..\") {\n"
  "    q = this.id.split('/');\n"
  "    q.pop();\n"
  "    q = q.concat(p);\n"
  "  }\n"
  "\n"
  "  // absolute path\n"
  "  else {\n"
  "    q = p;\n"
  "  }\n"
  "\n"
  "  // normalize path\n"
  "  n = [];\n"
  "\n"
  "  for (var i = 0;  i < q.length;  ++i) {\n"
  "    x = q[i];\n"
  "\n"
  "    if (x == \"\") {\n"
  "    }\n"
  "    else if (x == \".\") {\n"
  "    }\n"
  "    else if (x == \"..\") {\n"
  "      if (n.length == 0) {\n"
  "        throw \"cannot cross module top\";\n"
  "      }\n"
  "\n"
  "      n.pop();\n"
  "    }\n"
  "    else {\n"
  "      n.push(x);\n"
  "    }\n"
  "  }\n"
  "\n"
  "  return \"/\" + n.join('/');\n"
  "};\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief top-level module\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "ModuleCache[\"/\"] = module = new Module(\"/\");\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief global require function\n"
  "///\n"
  "/// @FUN{require(@FA{path})}\n"
  "///\n"
  "/// @FN{require} checks if the file specified by @FA{path} has already been\n"
  "/// loaded.  If not, the content of the file is executed in a new\n"
  "/// context. Within the context you can use the global variable @CODE{exports}\n"
  "/// in order to export variables and functions. This variable is returned by\n"
  "/// @FN{require}.\n"
  "///\n"
  "/// Assume that your module file is @CODE{test1.js} and contains\n"
  "///\n"
  "/// @verbinclude modules1\n"
  "///\n"
  "/// Then you can use require to load the file and access the exports.\n"
  "///\n"
  "/// @verbinclude modules2\n"
  "///\n"
  "/// @FN{require} follows the specification\n"
  "/// <a href=\"http://wiki.commonjs.org/wiki/Modules/1.1.1\">Modules/1.1.1</a>.\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "function require (path) {\n"
  "  return module.require(path);\n"
  "}\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @}\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "// -----------------------------------------------------------------------------\n"
  "// --SECTION--                                                       Module \"fs\"\n"
  "// -----------------------------------------------------------------------------\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @page JSModuleFsTOC\n"
  "///\n"
  "/// <ol>\n"
  "///   <li>@ref JSModuleFsExists \"fs.exists\"</li>\n"
  "/// </ol>\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @page JSModuleFs Module \"fs\"\n"
  "///\n"
  "/// The implementation follows the CommonJS specification\n"
  "/// <a href=\"http://wiki.commonjs.org/wiki/Filesystem/A/0\">Filesystem/A/0</a>.\n"
  "///\n"
  "/// <hr>\n"
  "/// @copydoc JSModuleFsTOC\n"
  "/// <hr>\n"
  "///\n"
  "/// @anchor JSModuleFsExists\n"
  "/// @copydetails JS_Exists\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @addtogroup V8ModuleFS\n"
  "/// @{\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief fs module\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "ModuleCache[\"/fs\"] = new Module(\"/fs\");\n"
  "ModuleCache[\"/fs\"].exports.exists = FS_EXISTS;\n"
  "fs = ModuleCache[\"/fs\"].exports;\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @}\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "// -----------------------------------------------------------------------------\n"
  "// --SECTION--                                                 Module \"internal\"\n"
  "// -----------------------------------------------------------------------------\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @page JSModuleInternalTOC\n"
  "///\n"
  "/// <ol>\n"
  "///   <li>@ref JSModuleInternalExecute \"internal.execute\"</li>\n"
  "///   <li>@ref JSModuleInternalLoad \"internal.load\"</li>\n"
  "///   <li>@ref JSModuleInternalLogLevel \"internal.log\"</li>\n"
  "///   <li>@ref JSModuleInternalLogLevel \"internal.logLevel\"</li>\n"
  "///   <li>@ref JSModuleInternalOutput \"internal.output\"</li>\n"
  "///   <li>@ref JSModuleInternalProcessStat \"internal.processStat\"</li>\n"
  "///   <li>@ref JSModuleInternalRead \"internal.read\"</li>\n"
  "///   <li>@ref JSModuleInternalSPrintF \"internal.sprintf\"</li>\n"
  "///   <li>@ref JSModuleInternalTime \"internal.time\"</li>\n"
  "/// </ol>\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @page JSModuleInternal Module \"internal\"\n"
  "///\n"
  "/// The following functions are used internally.\n"
  "///\n"
  "/// <hr>\n"
  "/// @copydoc JSModuleInternalTOC\n"
  "/// <hr>\n"
  "///\n"
  "/// @anchor JSModuleInternalExecute\n"
  "/// @copydetails JS_Execute\n"
  "///\n"
  "/// @anchor JSModuleInternalLoad\n"
  "/// @copydetails JS_Load\n"
  "///\n"
  "/// @anchor JSModuleInternalLog\n"
  "/// @copydetails JS_Log\n"
  "///\n"
  "/// @anchor JSModuleInternalLogLevel\n"
  "/// @copydetails JS_LogLevel\n"
  "///\n"
  "/// @anchor JSModuleInternalOutput\n"
  "/// @copydetails JS_Output\n"
  "///\n"
  "/// @anchor JSModuleInternalRead\n"
  "/// @copydetails JS_Read\n"
  "///\n"
  "/// @anchor JSModuleInternalSPrintF\n"
  "/// @copydetails JS_SPrintF\n"
  "///\n"
  "/// @anchor JSModuleInternalTime\n"
  "/// @copydetails JS_Time\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @addtogroup V8ModuleInternal\n"
  "/// @{\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief internal module\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "ModuleCache[\"/internal\"] = new Module(\"/internal\");\n"
  "ModuleCache[\"/internal\"].exports.execute = SYS_EXECUTE;\n"
  "ModuleCache[\"/internal\"].exports.load = SYS_LOAD;\n"
  "ModuleCache[\"/internal\"].exports.log = SYS_LOG;\n"
  "ModuleCache[\"/internal\"].exports.logLevel = SYS_LOG_LEVEL;\n"
  "ModuleCache[\"/internal\"].exports.output = SYS_OUTPUT;\n"
  "ModuleCache[\"/internal\"].exports.processStat = SYS_PROCESS_STAT;\n"
  "ModuleCache[\"/internal\"].exports.read = SYS_READ;\n"
  "ModuleCache[\"/internal\"].exports.sprintf = SYS_SPRINTF;\n"
  "ModuleCache[\"/internal\"].exports.time = SYS_TIME;\n"
  "internal = ModuleCache[\"/internal\"].exports;\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief reads a file\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "internal.readFile = function (path) {\n"
  "\n"
  "  // try to load the file\n"
  "  paths = MODULES_PATH.split(\";\");\n"
  "\n"
  "  for (var i = 0;  i < paths.length;  ++i) {\n"
  "    var p = paths[i];\n"
  "    var n;\n"
  "\n"
  "    if (p == \"\") {\n"
  "      n = \".\" + path + \".js\"\n"
  "    }\n"
  "    else {\n"
  "      n = p + \"/\" + path + \".js\";\n"
  "    }\n"
  "\n"
  "    if (FS_EXISTS(n)) {\n"
  "      return { path : n, content : SYS_READ(n) };\n"
  "    }\n"
  "  }\n"
  "\n"
  "  throw \"cannot find a file named '\" + path + \"' using the module path(s) '\" + MODULES_PATH + \"'\";\n"
  "}\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief loads a file\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "internal.loadFile = function (path) {\n"
  "\n"
  "  // try to load the file\n"
  "  paths = MODULES_PATH.split(\";\");\n"
  "\n"
  "  for (var i = 0;  i < paths.length;  ++i) {\n"
  "    var p = paths[i];\n"
  "    var n;\n"
  "\n"
  "    if (p == \"\") {\n"
  "      n = \".\" + path + \".js\"\n"
  "    }\n"
  "    else {\n"
  "      n = p + \"/\" + path + \".js\";\n"
  "    }\n"
  "\n"
  "    if (FS_EXISTS(n)) {\n"
  "      return SYS_LOAD(n);\n"
  "    }\n"
  "  }\n"
  "\n"
  "  throw \"cannot find a file named '\" + path + \"' using the module path(s) '\" + MODULES_PATH + \"'\";\n"
  "}\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @}\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "// -----------------------------------------------------------------------------\n"
  "// --SECTION--                                                  Module \"console\"\n"
  "// -----------------------------------------------------------------------------\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @page JSModuleConsoleTOC\n"
  "///\n"
  "/// <ol>\n"
  "///   <li>@ref JSModuleConsoleDebug \"console.debug\"</li>\n"
  "///   <li>@ref JSModuleConsoleError \"console.error\"</li>\n"
  "///   <li>@ref JSModuleConsoleInfo \"console.info\"</li>\n"
  "///   <li>@ref JSModuleConsoleLog \"console.log\"</li>\n"
  "///   <li>@ref JSModuleConsoleWarn \"console.warn\"</li>\n"
  "/// </ol>\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @page JSModuleConsole Module \"console\"\n"
  "///\n"
  "/// The implementation follows the CommonJS specification\n"
  "/// <a href=\"http://wiki.commonjs.org/wiki/Console\">Console</a>.\n"
  "///\n"
  "/// <hr>\n"
  "/// @copydoc JSModuleConsoleTOC\n"
  "/// <hr>\n"
  "///\n"
  "/// @anchor JSModuleConsoleDebug\n"
  "/// @copydetails JSF_CONSOLE_DEBUG\n"
  "///\n"
  "/// @anchor JSModuleConsoleError\n"
  "/// @copydetails JSF_CONSOLE_ERROR\n"
  "///\n"
  "/// @anchor JSModuleConsoleInfo\n"
  "/// @copydetails JSF_CONSOLE_INFO\n"
  "///\n"
  "/// @anchor JSModuleConsoleLog\n"
  "/// @copydetails JSF_CONSOLE_LOG\n"
  "///\n"
  "/// @anchor JSModuleConsoleWarn\n"
  "/// @copydetails JSF_CONSOLE_WARN\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @addtogroup V8ModuleConsole\n"
  "/// @{\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief logs debug message\n"
  "///\n"
  "/// @FUN{console.debug(@FA{format}, @FA{argument1}, ...)}\n"
  "///\n"
  "/// Formats the arguments according to @FA{format} and logs the result as\n"
  "/// debug message.\n"
  "///\n"
  "/// String substitution patterns, which can be used in @FA{format}.\n"
  "///\n"
  "/// - @LIT{\\%s} string\n"
  "/// - @LIT{\\%d}, @LIT{\\%i} integer\n"
  "/// - @LIT{\\%f} floating point number\n"
  "/// - @LIT{\\%o} object hyperlink\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "function CONSOLE_DEBUG () {\n"
  "  var msg;\n"
  "\n"
  "  msg = internal.sprintf.apply(internal.sprintf, arguments);\n"
  "  internal.log(\"debug\", msg);\n"
  "}\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief logs error message\n"
  "///\n"
  "/// @FUN{console.error(@FA{format}, @FA{argument1}, ...)}\n"
  "///\n"
  "/// Formats the arguments according to @FA{format} and logs the result as\n"
  "/// error message.\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "function CONSOLE_ERROR () {\n"
  "  var msg;\n"
  "\n"
  "  msg = internal.sprintf.apply(internal.sprintf, arguments);\n"
  "  internal.log(\"error\", msg);\n"
  "}\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief logs info message\n"
  "///\n"
  "/// @FUN{console.info(@FA{format}, @FA{argument1}, ...)}\n"
  "///\n"
  "/// Formats the arguments according to @FA{format} and logs the result as\n"
  "/// info message.\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "function CONSOLE_INFO () {\n"
  "  var msg;\n"
  "\n"
  "  msg = internal.sprintf.apply(internal.sprintf, arguments);\n"
  "  internal.log(\"info\", msg);\n"
  "}\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief logs log message\n"
  "///\n"
  "/// @FUN{console.log(@FA{format}, @FA{argument1}, ...)}\n"
  "///\n"
  "/// Formats the arguments according to @FA{format} and logs the result as\n"
  "/// log message.\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "function CONSOLE_LOG () {\n"
  "  var msg;\n"
  "\n"
  "  msg = internal.sprintf.apply(internal.sprintf, arguments);\n"
  "  internal.log(\"info\", msg);\n"
  "}\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief logs warn message\n"
  "///\n"
  "/// @FUN{console.warn(@FA{format}, @FA{argument1}, ...)}\n"
  "///\n"
  "/// Formats the arguments according to @FA{format} and logs the result as\n"
  "/// warn message.\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "function CONSOLE_WARN () {\n"
  "  var msg;\n"
  "\n"
  "  msg = internal.sprintf.apply(internal.sprintf, arguments);\n"
  "  internal.log(\"warn\", msg);\n"
  "}\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief console module\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "ModuleCache[\"/console\"] = new Module(\"/console\");\n"
  "ModuleCache[\"/console\"].exports.debug = CONSOLE_DEBUG;\n"
  "ModuleCache[\"/console\"].exports.error = CONSOLE_ERROR;\n"
  "ModuleCache[\"/console\"].exports.info = CONSOLE_INFO;\n"
  "ModuleCache[\"/console\"].exports.log = CONSOLE_LOG;\n"
  "ModuleCache[\"/console\"].exports.warn = CONSOLE_WARN;\n"
  "console = ModuleCache[\"/console\"].exports;\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @}\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "// Local Variables:\n"
  "// mode: outline-minor\n"
  "// outline-regexp: \"^\\\\(/// @brief\\\\|/// @addtogroup\\\\|// --SECTION--\\\\|/// @page\\\\|/// @}\\\\)\"\n"
  "// End:\n"
;