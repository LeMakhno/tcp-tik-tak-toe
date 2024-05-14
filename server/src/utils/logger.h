#ifndef _LOGGER
#define _LOGGER 1

#define LOGDATA_MAXLEN 256

void initialize_logger();
void putlog(char *format, ...);

#endif
