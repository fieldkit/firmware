#include <inttypes.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "fk.h"
#include "hal/linux/linux.h"
#include "protobuf.h"
#include "gs_progress_callbacks.h"

FK_DECLARE_LOGGER("main");

using namespace fk;

__int32_t main(__int32_t argc, const char **argv) {
    log_configure_level(LogLevels::DEBUG);

    for (auto i = 1; i < argc; ++i) {
        auto fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            logerror("error opening %s", argv[i]);
            continue;
        }

        loginfo("opened");

        struct stat statbuf;
        if (fstat(fd, &statbuf) < 0) {
            return 0;
        }

        LinuxDataMemory memory;
        if (!memory.begin()) {
            logerror("memory begin failed");
            return 0;
        }

        loginfo("mmap and copy (%ld)...", statbuf.st_size);

        auto ptr = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
        if (ptr == (caddr_t)-1) {
            return 0;
        }

        memcpy(memory.memory(), ptr, statbuf.st_size);

        loginfo("initializing");

        StandardPool pool{ "fstool" };
        Storage storage{ &memory, pool, false };
        if (!storage.begin()) {
            logerror("storage begin failed");
            return 0;
        }

        GlobalStateProgressCallbacks progress;
        storage.fsck(&progress);

        close(fd);
    }

    loginfo("done");

    return 0;
}
