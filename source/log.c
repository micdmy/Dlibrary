#include "DConfig.h"
#ifdef D_COMPONENT_LOG

logEntry logBuffer[LOG_SIZE];
volatile uint8_t logLast = 0xff;

void logReset (void) {
	logLast = 0xff;
}

void logAdd(logEntry newEntry) {
	logLast = (logLast + 1) & (LOG_SIZE-1);
	logBuffer[logLast] = newEntry;
}
#endif //D_COMPONENT_LOG
