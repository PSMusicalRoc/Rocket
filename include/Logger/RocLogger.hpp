#ifndef _ROC_LOGGER_HPP_
#define _ROC_LOGGER_HPP_

/**
 * This file uses the define ROC_DEBUG to actually
 * load in the logging files.
*/

#define LogTrace(message) Logger::_LogTrace(message)
#define LogInfo(message) Logger::_LogInfo(message)
#define LogWarn(message) Logger::_LogWarn(message, __FILE__, __LINE__)
#define LogError(message) Logger::_LogError(message, __FILE__, __LINE__)
#define LogFatal(message) Logger::_LogFatal(message, __FILE__, __LINE__)

#define LogAssert(expr) if (!(expr)) {\
    Logger::_LogFatal("Assertion Failed\n\t" #expr, __FILE__, __LINE__);\
    throw "ROC_ASSERTION_FAILED";\
}

#include <string>
#include <iostream>
#include <fstream>

enum LogLevels
{
    FATAL = 8,
    ERROR = 4,
    WARN = 2,
    INFO = 1,
    TRACE = 0
};

class Logger
{
private:
    static std::string FATALCC;
    static std::string ERRORCC;
    static std::string WARNCC;
    static std::string INFOCC;
    static std::string TRACECC;
    static std::string RESETCC;

    static bool out_to_cout;
    static bool out_to_file;
    static std::fstream outfile;

protected:
    static void log(LogLevels level, const std::string& message,
        std::string err_filename = "", int err_linenum = -1);

public:
    static void _LogTrace(const std::string& message) { log(TRACE, message); }
    static void _LogInfo(const std::string& message) { log(INFO, message); }
    static void _LogWarn(const std::string& message,
        std::string err_filename, int err_linenum) { log(WARN, message, err_filename, err_linenum); }
    static void _LogError(const std::string& message,
        std::string err_filename, int err_linenum) { log(ERROR, message, err_filename, err_linenum); }
    static void _LogFatal(const std::string& message,
        std::string err_filename, int err_linenum) { log(FATAL, message, err_filename, err_linenum); }

    static void LogConsole(bool yesno) { out_to_cout = yesno; }
    static void EnableLogFile(const std::string& filename);
    static void DisableLogFile();
    
};

#endif