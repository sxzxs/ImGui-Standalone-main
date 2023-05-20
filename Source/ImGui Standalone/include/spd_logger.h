#ifndef __LOGGER_H__
#define __LOGGER_H__

#define  SPDLOG_ACTIVE_LEVEL 0
#include "spdlog/spdlog.h"
#include "spdlog/stopwatch.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/async.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/cfg/env.h" // support for loading levels from the environment variable
#include "json.hpp"
#define TRACE(...) SPDLOG_LOGGER_TRACE(Spd_logger::instance()->get_m_logger(), __VA_ARGS__)
#define DEBUG(...) SPDLOG_LOGGER_DEBUG(Spd_logger::instance()->get_m_logger(), __VA_ARGS__)
#define INFO(...) SPDLOG_LOGGER_INFO(Spd_logger::instance()->get_m_logger(), __VA_ARGS__)
#define WAR(...) SPDLOG_LOGGER_WARN(Spd_logger::instance()->get_m_logger(), __VA_ARGS__)
#define ERR(...) SPDLOG_LOGGER_ERROR(Spd_logger::instance()->get_m_logger(), __VA_ARGS__)
#define CRITICAL(...) SPDLOG_LOGGER_CRITICAL(Spd_logger::instance()->get_m_logger(), __VA_ARGS__)
using namespace nlohmann;

class Spd_logger
{
public:
    static Spd_logger *instance()
    {
        static Spd_logger obj;
        return &obj;
    }
#ifdef ASYNC_LOGGER
    std::shared_ptr<spdlog::async_logger> get_m_logger()
#else
    std::shared_ptr<spdlog::logger> get_m_logger()
#endif // ASYNC_LOGGER
    {
        return m_logger;
    }
    void set_level(spdlog::level::level_enum log_level)
    {
        m_logger->set_level(log_level);
    }
    void set_pattern(const std::string &pattern)
    {
        m_logger->set_pattern(pattern);
    }
private:
    Spd_logger()
    {
        //DllCall("SetConsoleOutputCP", "int", 65001)
        SetConsoleOutputCP(65001);
        spdlog::init_thread_pool(8192 * 10, 1);
        auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >();
        auto rotating_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("logs/log.txt", 2, 30);

        stdout_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%F] [pid %P] [thread %t] [%s %! %#] %^[%l]%$ %v (%ius)");
        rotating_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%F] [pid %P] [thread %t] [%s %! %#] %^[%l]%$ %v (%ius)");

        std::vector<spdlog::sink_ptr> sinks {stdout_sink, rotating_sink};

#ifdef ASYNC_LOGGER
        m_logger = std::make_shared<spdlog::async_logger>("console_and_file_log", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
#else
        spdlog::logger logger("multi_sink", {stdout_sink, rotating_sink});
        m_logger = std::make_shared<spdlog::logger>(logger);
#endif
        spdlog::register_logger(m_logger);
        m_logger->flush_on(spdlog::level::trace);
        m_logger->set_level(spdlog::level::trace);
        spdlog::flush_every(std::chrono::seconds(1)); // 每隔1秒刷新一次日志
    }
    ~Spd_logger()
    {
        spdlog::drop_all();
        spdlog::shutdown();
    }
#ifdef ASYNC_LOGGER
    std::shared_ptr<spdlog::async_logger> m_logger;
#else
    std::shared_ptr<spdlog::logger> m_logger;
#endif // ASYNC_LOGGER
};

#endif