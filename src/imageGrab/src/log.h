#define LOG_LEVEL_ERROR "ERROR "
#define LOG_LEVEL_WARNING "WARN  "
#define LOG_LEVEL_INFO "INFO  "
#define LOG_LEVEL_VERBOSE "TRACE "
#define LOG_LEVEL_DEBUG "DEBUG "
#define LOG_TAG "capture"


#define APPLOG(level, color, fmt, arg...)  \
    printf(color "[%s] %s <%s:%u>: "fmt" \033[0m\n", level, LOG_TAG, __FUNCTION__, __LINE__, ##arg)

#define loge(fmt, arg...) APPLOG(LOG_LEVEL_ERROR  , "\033[40;31m", fmt, ##arg)
#define logw(fmt, arg...) APPLOG(LOG_LEVEL_WARNING, "\033[40;33m", fmt, ##arg)
#define logi(fmt, arg...) APPLOG(LOG_LEVEL_INFO   , "\033[40;36m", fmt, ##arg)
#define logv(fmt, arg...) APPLOG(LOG_LEVEL_VERBOSE, "\033[40;37m", fmt, ##arg)

#define __LOG_TIME_START()
#define __LOG_TIME_END(name)