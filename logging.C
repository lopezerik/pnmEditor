#include <logging.h>
#include <string.h>
#include <cstddef>

DataFlowException::DataFlowException(const char *type, const char *error){
    sprintf(this->msg, "Throwing exception: (%s): %s", type, error);
    Logger::LogEvent(msg);
}

FILE *Logger::logger = NULL;

void Logger::LogEvent(const char *event){
    if(logger == NULL){
        logger = fopen("execution_log", "w");
    }
    // Log event
    fprintf(logger, event, strlen(event));
    fprintf(logger, "\n");
}

void Logger::Finalize(){
    // Finalize aka close file
    fclose(logger);
}

