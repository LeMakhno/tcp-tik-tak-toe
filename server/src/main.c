#include "include/common.h"
#include "loop/event_consumers.h"
#include "loop/loop.h"
#include "server/server.h"
#include "clients/connset.h"
#include "storage/storage.h"
#include "utils/logger.h"

int main(int argc, char *argv[])
{
    if (argc == 1) {
        putlog("Invalid arguments amount");
        putlog("Expected usage: executable PORT");
        return EXIT_FAILURE;
    }

    srand(time(NULL));
    setbuf(stdout, NULL);
    
    server_init(argv[1], 100);
    connset_init();
    storage_init();

    run_loop();
    
    return EXIT_SUCCESS;
}
