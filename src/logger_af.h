#ifndef LOGGER_AF_LOGGER_AF_H
#define LOGGER_AF_LOGGER_AF_H

#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <utility>
#include <string_view>

// switch this to <format> once implemented by g++
#include <fmt/core.h>
#include <fmt/chrono.h>


namespace Logger_AF {
    enum class Level : std::int8_t {
        NONE = -1, // Used to indicate when no error levels are requested.  Shouldn't ever be used as an actual output level.
        FATAL,
        ERROR,
        WARN,
        INFO,
        DEBUG,
        TRACE
    };

    namespace details {
        constexpr std::string_view level_to_string(Level l) {
            switch (l) {
                // Make all these constant width
                case Level::FATAL:
                    return "FATAL";
                case Level::ERROR:
                    return "ERROR";
                case Level::WARN:
                    return "WARN ";
                case Level::INFO:
                    return "INFO ";
                case Level::DEBUG:
                    return "DEBUG";
                case Level::TRACE:
                    return "TRACE";
            }
        }

        enum class Stream_Type : std::int8_t {
            COUT,
            CERR
        };

        constexpr Stream_Type level_to_stream_type(Level l) {
            switch (l) {
                case Level::FATAL:
                case Level::ERROR:
                    return Stream_Type::CERR;
                case Level::WARN:
                case Level::INFO:
                case Level::DEBUG:
                case Level::TRACE:
                    return Stream_Type::COUT;
            }
        }


        constexpr bool flush_from_level(Level l) {
            switch (l) {
                case Level::FATAL:
                case Level::ERROR:
                    return true;
                case Level::WARN:
                case Level::INFO:
                case Level::DEBUG:
                case Level::TRACE:
                    return false;
            }
        }
    }

    class Logger {

        static std::atomic<Level> m_level;
        static std::mutex m_mutex;

        template<bool FLUSH>
        static void inline output_line(std::ostream &stream, std::string_view str) {
            stream << str << '\n';
            if (FLUSH) {
                stream << std::flush;
            }
        }

        template<Level LEVEL>
        static void output_line(std::string_view str) {
            switch (details::level_to_stream_type(LEVEL)) {
                case (details::Stream_Type::COUT):
                    output_line<details::flush_from_level(LEVEL)>(std::cout, str);
                    break;
                case (details::Stream_Type::CERR):
                    output_line<details::flush_from_level(LEVEL)>(std::cerr, str);
                    break;
            }
        }


    public:

        static void set_level(Level new_level) { m_level = new_level; }

        template<Level LEVEL = Level::INFO, typename... ARGS>
        static void put(std::string_view str, ARGS... args) {
            if (LEVEL == Level::NONE){
                return; // attempt to compile-time optimize away this function for NONE case
            }
            if (LEVEL <= m_level) {
                std::time_t t = std::time(nullptr);

                auto tag = fmt::format("[{}] {:%Y-%m-%d} {:%T}", details::level_to_string(LEVEL),
                                       fmt::localtime(t), fmt::localtime(t));
                auto content = fmt::format(str, args...);

                auto output = fmt::format("{}| {}", tag, content);

                {
                    std::scoped_lock printing_lock(m_mutex);
                    output_line<LEVEL>(output);
                }
            }
        }
    };

    // needed to instantiate because static singleton
    std::atomic<Level> Logger::m_level = Level::TRACE; // default show every Level
    std::mutex Logger::m_mutex = {};
}


#endif //LOGGER_AF_LOGGER_AF_H
