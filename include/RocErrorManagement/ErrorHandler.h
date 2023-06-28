#ifndef _ROC_ERROR_HANDLER_H_
#define _ROC_ERROR_HANDLER_H_

#include <string>

class ROCErrorHandler
{
private:
    static std::string errortext;
    static bool haserrortext;

public:
    static const std::string& _get_error_message()
    {
        if(haserrortext) return errortext;
        else return "No Error to report!";
    }
    static void _set_error_text(const std::string& text) {errortext = text; haserrortext = true;}

    static void _clear_error_text() {errortext = ""; haserrortext = false;}
};

const std::string& ROC_ErrorText();

void ROC_ClearError();

void ROC_SetErrorMessage(const std::string& message);

#endif