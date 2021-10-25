#ifndef LOGGER_AF_LIBRARY_H
#define LOGGER_AF_LIBRARY_H

#include <cstdint>

namespace logger_af {
    enum class level : std::uint8_t {
        FATAL,
        ERROR,
        WARN,
        INFO,
        DEBUG,
        TRACE
    };
}
#endif //LOGGER_AF_LIBRARY_H
