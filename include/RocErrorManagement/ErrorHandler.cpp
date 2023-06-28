#include "RocErrorManagement/ErrorHandler.h"

std::string ROCErrorHandler::errortext;
bool ROCErrorHandler::haserrortext = false;

const std::string& ROC_ErrorText()
{
    return ROCErrorHandler::_get_error_message();
}

void ROC_ClearError() {ROCErrorHandler::_clear_error_text();}

void ROC_SetErrorMessage(const std::string& message) {ROCErrorHandler::_set_error_text(message);}