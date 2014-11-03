#ifndef DECRYPTION_SERVER_H_
#define DECRYPTION_SERVER_H_

#include "queue.h"
#include "protocol.h"
#include "util.h"

#ifdef __unix__
# include <unistd.h>
#elif defined _WIN32
# include <windows.h>
#define sleep(x) Sleep(1000 * x)
#endif

#endif /* decpryption_server.h */
