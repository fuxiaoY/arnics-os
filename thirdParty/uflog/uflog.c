#include "uflog.h"

struct _uflog_t
{    
    int control;
    bool force_store;
    int len;
    void (*print)(char* out_str);
    void (*print_cache)(uflog_t *uflog_p, char *out_str);

    #ifndef UFLOG_STATIC_MEMORY
    char str_cache[];
    #else
    char str_cache[UFLOG_STORE_CACHE];
    #endif
};
static uflog_pri_e uflog_level = UFLOG_LEVEL_OFF;
uflog_t *uflog_default_p;
#ifdef UFLOG_STATIC_MEMORY
uflog_t uflog_default;
#endif


#define X(level, color, bold, underline, reverse, blink)  + 1
#define LEVEL_NUM (0 UFLOG_LEVEL_COLOR_MAP)

static uint32_t get_level_color_num()
{
    return (LEVEL_NUM);
}
#undef X
#define X(level, color, bold, underline, reverse, blink) \
         {level, color, bold, underline, reverse, blink }, 

static uflog_color_t uflog_color[] = 
{
    UFLOG_LEVEL_COLOR_MAP
};


void uflog_level_change(uflog_pri_e uflog_level_in)
{
    uflog_level = uflog_level_in;
}

static void print_cache(uflog_t *uflog_p, char *out_str)
{
    if (!uflog_p || !out_str) 
    {
        return;
    }

    size_t available = UFLOG_STORE_CACHE - uflog_p->len;

    if (available <= 0) 
    {
        return;  
    }
    int written = snprintf(uflog_p->str_cache + uflog_p->len, available, "%s", out_str);
    if (written > 0) 
    {
        uflog_p->len += (written < available) ? written : (available - 1);
    }

}

uflog_t* uflog_create(int control,uflog_print print)
{
#ifndef UFLOG_STATIC_MEMORY
    // if(UFLOG_HAS_FLAG(control,UFLOG_AUTO_STORE))
    {
        uflog_t *uflog_p = (uflog_t*)uflog_malloc(sizeof(uflog_t) + UFLOG_STORE_CACHE);
        uflog_p->control = control;
        uflog_p->print = print;
        uflog_p->len = 0;
        uflog_p->print_cache = print_cache;
        return uflog_p;
    }
    // else
    // {
    //     uflog_t *uflog_p = (uflog_t*)uflog_malloc(sizeof(uflog_t));
    //     uflog_p->control = control;
    //     uflog_p->print = print;
    //     return uflog_p;
    // }
#else
    // if(UFLOG_HAS_FLAG(control,UFLOG_AUTO_STORE))
    {
        uflog_default.control = control;
        uflog_default.print = print;
        uflog_default.len = 0;
        memset(uflog_default.str_cache,0,UFLOG_STORE_CACHE);
        uflog_default.print_cache = print_cache;
        return &uflog_default;
    }
    // else
    // {
    //     uflog_default.control = control;
    //     uflog_default.print = print;
    //     uflog_default.len = 0;
    //     memset(uflog_default.str_cache,0,UFLOG_STORE_CACHE);
    //     return &uflog_default;
    // }
#endif
}

void uflog_delete(uflog_t *uflog_p)
{
    #ifndef UFLOG_STATIC_MEMORY
    uflog_free(uflog_p);
    #endif
}

void uflog_control_config(uflog_t *uflog_p, int control)
{
    uflog_p->control = control;
}


#define SAFE_SNPRINTF(max_buf, buf, len, fmt, ...) do { \
    int remaining = max_buf - len; \
    if (remaining > 0) { \
        int written = snprintf(buf + len, remaining, fmt, ##__VA_ARGS__); \
        if (written > 0) len += (written < remaining ? written : remaining); \
    } \
} while(0)


static void uflog_printf(uflog_t *uflog_p, char *out_str)
{
    uflog_p->print(out_str);
    if(UFLOG_HAS_FLAG(uflog_p->control,UFLOG_AUTO_STORE)||(uflog_p->force_store))
    {
        if(uflog_p->print_cache != NULL)
        {
            uflog_p->print_cache(uflog_p,out_str);
        }
    }

}
/*---------------------------------------------------------------------*/
static void color_control(uflog_t *uflog_p,uflog_pri_e log_level,bool is_start)
{
    if (!UFLOG_HAS_FLAG(uflog_p->control, UFLOG_USE_COLOR))
    {
        return;
    }
    if(is_start)
    {
        for (uint8_t i = 0; i < get_level_color_num(); i++) 
        {
            if (i == log_level) 
            {
                uflog_p->print(uflog_color[i].color);
                if(uflog_color[i].bold)
                {
                    uflog_p->print(UFLOG_COLOR_FORMAT_BOLD);
                }
                if(uflog_color[i].underline)
                {
                    uflog_p->print(UFLOG_COLOR_FORMAT_UNDERLINE);
                }
                if(uflog_color[i].reverse)
                {
                    uflog_p->print(UFLOG_COLOR_FORMAT_REVERSE);
                }
                if(uflog_color[i].blink)
                {
                    uflog_p->print(UFLOG_COLOR_FORMAT_BLINK);
                }
                
                break;
            }
        }
    }
    else
    {
        uflog_p->print(UFLOG_COLOR_RESET);
    }
}
// [pri]
static int add_priority(char *buffer, int len, uflog_pri_e control)
{
    switch(control) 
    {
        case UFLOG_PRI_DBG:
            SAFE_SNPRINTF(UFLOG_MAX_PREFIX_LEN,buffer, len, "[DBG]");
            break;
        case UFLOG_PRI_INF:
            SAFE_SNPRINTF(UFLOG_MAX_PREFIX_LEN,buffer, len, "[INF]");
            break;
        case UFLOG_PRI_WAR:
            SAFE_SNPRINTF(UFLOG_MAX_PREFIX_LEN,buffer, len, "[WAR]");
            break;
        case UFLOG_PRI_ERR:
            SAFE_SNPRINTF(UFLOG_MAX_PREFIX_LEN,buffer, len, "[ERR]");
            break;
        case UFLOG_PRI_ALT:
            SAFE_SNPRINTF(UFLOG_MAX_PREFIX_LEN,buffer, len, "[ALT]");
            break;
        case UFLOG_PRI_NOT:
            SAFE_SNPRINTF(UFLOG_MAX_PREFIX_LEN,buffer, len, "[NOT]");
            break;
        case UFLOG_PRI_ALW:
            SAFE_SNPRINTF(UFLOG_MAX_PREFIX_LEN,buffer, len, "[ALW]");
            break;
        default:
            break;
    }
    return len;
}

// [facility]
static int add_facility(char *buffer, int len, int control, const char *facility) 
{
    SAFE_SNPRINTF(UFLOG_MAX_PREFIX_LEN,buffer, len, " ");
    if(UFLOG_HAS_FLAG(control, UFLOG_SHOW_FACILITY)) 
    {
        SAFE_SNPRINTF(UFLOG_MAX_PREFIX_LEN,buffer, len, "[%s]", facility);
    } 
    else 
    {
        SAFE_SNPRINTF(UFLOG_MAX_PREFIX_LEN,buffer, len, "-");
    }
    return len;
}
// [timestamp]
static int add_timestamp(char *buffer, int len, int control) 
{
    char timestamp[32];
    memset(timestamp, 0, sizeof(timestamp));
    uflog_get_timestamp(timestamp,sizeof(timestamp));

    SAFE_SNPRINTF(UFLOG_MAX_PREFIX_LEN,buffer, len, " ");
    if (UFLOG_HAS_FLAG(control, UFLOG_SHOW_TIMESTAMP)) 
    {
        SAFE_SNPRINTF(UFLOG_MAX_PREFIX_LEN,buffer, len, "%s",timestamp);
    } 
    else 
    {
        SAFE_SNPRINTF(UFLOG_MAX_PREFIX_LEN,buffer, len, "-");
    }
    return len;
}

// [version]
static int add_version(char *buffer, int len) 
{
    SAFE_SNPRINTF(UFLOG_MAX_PREFIX_LEN,buffer, len, " ");
    SAFE_SNPRINTF(UFLOG_MAX_PREFIX_LEN,buffer, len, "%d>", UFLOG_VERSION);
    return len;
}


static int log_prefix(uflog_t *uflog_p,uflog_pri_e log_level, const char *facility) 
{
    int len = 0;
    char prefix[UFLOG_MAX_PREFIX_LEN];

    len = add_priority(prefix, len, log_level);
    len = add_facility(prefix, len, uflog_p->control, facility);
    len = add_timestamp(prefix, len, uflog_p->control); 
    len = add_version(prefix, len);

    if (len < UFLOG_MAX_PREFIX_LEN) 
    {
        prefix[len] = '\0';
    }
    else
    {
        prefix[UFLOG_MAX_PREFIX_LEN - 1] = '\0';
    }

    uflog_printf(uflog_p,prefix);
    return len;
}
static int log_kind(uflog_t *uflog_p,const char *kind)
{
    char kind_str[32];
    int len = 0;

    if(UFLOG_HAS_FLAG(uflog_p->control, UFLOG_SHOW_KIND)) 
    {
        SAFE_SNPRINTF(32,kind_str, len, " ");
        SAFE_SNPRINTF(32,kind_str, len, "[%s]", kind);
    } 
    if (len < 32) 
    {
        kind_str[len] = '\0';
    }
    else
    {
        kind_str[32 - 1] = '\0';
    }

    uflog_printf(uflog_p,kind_str);
    return len;
}


static bool uflog_msg_hex(uflog_t *uflog_p,uint8_t *data,size_t data_len)
{
    if(NULL != data && data_len > 0)
    {
        int len = 0;
        char msg_str[UFLOG_MSG_STRUCT_LEN];

        SAFE_SNPRINTF(UFLOG_MSG_STRUCT_LEN, msg_str, len, " ");

        SAFE_SNPRINTF(UFLOG_MSG_STRUCT_LEN, msg_str, len, "\r\n|>");
        for(uint32_t i = 0; i < data_len; i++)
        {
            SAFE_SNPRINTF(UFLOG_MSG_STRUCT_LEN, msg_str, len, " 0x%x",data[i]);
        }
        SAFE_SNPRINTF(UFLOG_MSG_STRUCT_LEN, msg_str, len, "<|\r\n");
        uflog_printf(uflog_p,msg_str);
        return true;
    }
    else
    {
        return false;
    }
}

static void uflog_fun_line(uflog_t *uflog_p,const char *func, const char *file, int line)
{
    if (!UFLOG_HAS_FLAG(uflog_p->control, UFLOG_SHOW_FUNTION))
    {
        return;
    }

    int len = 0;
    char fun_line[UFLOG_FUN_LINE_LEN];

    uflog_printf(uflog_p,"(");

    SAFE_SNPRINTF(UFLOG_FUN_LINE_LEN, fun_line, len, "%s",func);
    SAFE_SNPRINTF(UFLOG_FUN_LINE_LEN, fun_line, len, "|");
    SAFE_SNPRINTF(UFLOG_FUN_LINE_LEN, fun_line, len, "%s",file);
    SAFE_SNPRINTF(UFLOG_FUN_LINE_LEN, fun_line, len, "|");
    SAFE_SNPRINTF(UFLOG_FUN_LINE_LEN, fun_line, len, "%d)",line);

    if (len < UFLOG_FUN_LINE_LEN) 
    {
        fun_line[len] = '\0';
    }
    else
    {
        fun_line[UFLOG_FUN_LINE_LEN - 1] = '\0';
    }

    uflog_printf(uflog_p,fun_line); 
    uflog_printf(uflog_p,")");

}

/**
 * @brief 主日志函数
 * 
 * @param uflog_p     指向日志控制结构体的指针
 * @param log_level   日志级别，决定日志的重要程度
 * @param facility    日志设施名称，用于标识日志来源
 * @param kind        日志种类，用于进一步分类日志
 * @param data        指向要以十六进制格式输出的数据的指针
 * @param len         数据长度
 * @param force_store 强制存储标志，为true时强制存储日志
 * @param func        函数名称，__FUNCTION__
 * @param file        文件名称，__FILENAME__
 * @param line        行号，__LINE__
 * @param fmt         格式化字符串，类似于printf函数的格式
 * @param ...         
 * 
 * @note 该函数会检查日志级别，只有当日志级别允许时才会输出日志
 * @note 如果启用了自动存储或强制存储，日志信息会被存储到缓存中
 * @note 函数会自动处理格式化字符串和十六进制数据的输出
 */ 
void uflog_log(uflog_t *uflog_p,uflog_pri_e log_level, const char *facility, 
               const char *kind, 
               uint8_t *data, size_t len, bool force_store,
               const char *func, const char *file, int line, const char *fmt, ...) 
{
    uflog_p->force_store = force_store;

    if (!(uflog_p && uflog_p->print)) 
    {
        return;
    }

    if (!UFLOG_HAS_FLAG(uflog_level, log_level)) 
    {
        return;
    }
    if(UFLOG_HAS_FLAG(uflog_p->control,UFLOG_AUTO_STORE)||uflog_p->force_store)
    {
        if(uflog_p->print_cache != NULL)
        {
            uflog_p->len = 0;
        }
    }


    char message[UFLOG_USER_MSG_MAX_LEN];  
    int msg_len = 0;
    //color
    color_control(uflog_p,log_level,true);

    // perfix
    log_prefix(uflog_p,log_level, facility);
    
    // kind
    log_kind(uflog_p,kind);

    // print
    uflog_printf(uflog_p," ");
    va_list args;
    va_start(args, fmt);
    msg_len = vsnprintf(message, sizeof(message), fmt, args);
    va_end(args);
    if (msg_len >= sizeof(message))
    {
        message[sizeof(message) - 1] = '\0';
        msg_len = sizeof(message) - 1;
    }
    else
    {
        message[msg_len] = '\0';
    }
    if (msg_len > 0) 
    {
        uflog_printf(uflog_p,message);
    }
    // msg 
    if(!uflog_msg_hex(uflog_p,data,len))
    {
        uflog_printf(uflog_p," ");
    }

    // end
    uflog_fun_line(uflog_p,func,file,line);

    uflog_printf(uflog_p,UFLOG_END);

    //color
    color_control(uflog_p,log_level,false);

    if(UFLOG_HAS_FLAG(uflog_p->control,UFLOG_AUTO_STORE))
    {
        log_auto_store(uflog_p->str_cache);
        memset(uflog_p->str_cache,0,UFLOG_STORE_CACHE);
    }
    else if(uflog_p->force_store)
    {
        log_auto_store(uflog_p->str_cache);
        memset(uflog_p->str_cache,0,UFLOG_STORE_CACHE);
    }
    else
    {
        //do nothing
    }

}



/* user --------------------------------------------------------------*/

void uflog_init(uflog_pri_e uflog_level_in,int control)
{
   uflog_level_change(uflog_level_in);
   uflog_default_p = uflog_create(control, log_printf);
}

void uflog_config(int control)
{
    uflog_control_config(uflog_default_p,control);
}

void uflog_close()
{
    #ifndef UFLOG_STATIC_MEMORY
    uflog_delete(uflog_default_p);
    #endif
}

/*port --------------------------------------------------------------*/
__weak void uflog_get_timestamp(char *timestamp, int timestamp_len)
{
    if (timestamp && timestamp_len > 0) 
    {
        snprintf(timestamp, timestamp_len, "9999-01-01 00:00:00");
    }
}

__weak void log_printf(char *out_str)
{
    printf("%s", out_str);
}

__weak void log_auto_store(char * str)
{

}
