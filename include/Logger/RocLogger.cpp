#include "RocLogger.hpp"

std::string Logger::FATALCC = "\033[39;41m";
std::string Logger::ERRORCC = "\033[31m";
std::string Logger::WARNCC  = "\033[33m";
std::string Logger::INFOCC  = "\033[32m";
std::string Logger::TRACECC = "\033[2m";
std::string Logger::RESETCC = "\033[0m";

bool Logger::out_to_cout = true;
bool Logger::out_to_file = false;
std::fstream Logger::outfile;

void Logger::EnableLogFile(const std::string& filename)
{
    if (outfile.is_open())
    {
        outfile.close();
    }
    outfile.open(filename, std::ios::out);
    out_to_file = true;
}

void Logger::DisableLogFile()
{
    if (outfile.is_open())
    {
        outfile.close();
    }
    out_to_file = false;
}

void Logger::log(LogLevels level, const std::string& message,
    std::string err_filename, int err_linenum)
{
    std::string output;
    std::string fileoutput;
    switch (level)
    {
    case TRACE:
        output += TRACECC + "[TRACE]: ";
        fileoutput += "[TRACE]: ";
        break;
    case INFO:
        output += INFOCC + "[INFO]: ";
        fileoutput += "[INFO]: ";
        break;
    case WARN:
        output += WARNCC + "[WARN]: ";
        fileoutput += "[WARN]: ";
        break;
    case ERROR:
        output += ERRORCC + "[ERROR]: ";
        fileoutput += "[ERROR]: ";
        break;
    case FATAL:
        output += FATALCC + "[FATAL]: ";
        fileoutput += "[FATAL]: ";
        break;
    default:
        output += RESETCC + "[LOG]: ";
        fileoutput += "[LOG]: ";
        break;
    }

    output += message;
    fileoutput += message;

    if (err_linenum != -1)
    {
        output += "\n\t";
        output += err_filename + " " + std::to_string(err_linenum);
        fileoutput += "\n\t";
        fileoutput += err_filename + " " + std::to_string(err_linenum);
    }

    if (out_to_cout)
        std::cout << output << RESETCC << std::endl;
    if (out_to_file)
        outfile << fileoutput << std::endl;

}