#include <pb_encode.h>
#include <fk-data.pb.h>

#include <iostream>

int32_t main() {
    std::cout << "Hello, world!" << std::endl;

    uint8_t buffer[128];
    size_t message_length;

    fk_data_LogMessage log = fk_data_LogMessage_init_default;
    log.uptime = 32434;
    log.time = 394038802;

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    if (!pb_encode(&stream, fk_data_LogMessage_fields, &log)) {
        std::cout << "Failed!" << std::endl;
    }
    else {
        std::cout << "Success " << stream.bytes_written << std::endl;
    }

    return 0;
}
