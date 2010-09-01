/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: eyjian@qq.com or eyjian@gmail.com
 */
#ifndef SYS_LOGGER_H
#define SYS_LOGGER_H
#include "sys/log.h"
#include "sys/event.h"
#include "sys/thread.h"
#include "util/array_queue.h"
SYS_NAMESPACE_BEGIN

class CLogger: public sys::ILogger
{
public:
    CLogger(uint16_t log_line_size=512);
    virtual ~CLogger();
    
    void destroy();
    bool create(const char* log_path, const char* log_filename, uint32_t log_queue_size=1000, uint16_t log_queue_number=3);

    virtual void enable_screen(bool enabled);
    virtual void enable_trace_log(bool enabled);
    virtual void enable_auto_newline(bool auto_newline);
    virtual void set_log_level(log_level_t log_level);
    virtual void set_single_filesize(uint32_t filesize);
    virtual void set_backup_number(uint16_t backup_number);

    virtual bool enabled_debug();
    virtual bool enabled_info();
    virtual bool enabled_warn();
    virtual bool enabled_error();
    virtual bool enabled_fatal();
    virtual bool enabled_trace();

    virtual void log_debug(const char* format, ...);
    virtual void log_info(const char* format, ...);
    virtual void log_warn(const char* format, ...);
    virtual void log_error(const char* format, ...);
    virtual void log_fatal(const char* format, ...);
    virtual void log_trace(const char* format, ...);

private:
    void do_log(log_level_t log_level, const char* format, va_list& args);
    
private:    
    bool _auto_newline;
    uint16_t _log_line_size;
    log_level_t _log_level;
    bool _trace_log_enabled;

private: // 内部内
    class CLogThread: public CThread
    {
    public:
        CLogThread(const char* log_path, const char* log_filename, uint32_t queue_size, uint16_t queue_number);
        ~CLogThread();

        void push_log(const char* log);
        void enable_screen(bool enabled);
        void set_single_filesize(uint32_t filesize);
        void set_backup_number(uint16_t backup_number);

    public:
        virtual void stop (bool wait_stop=true);

    private:
        virtual void run();
        virtual bool before_start();

    private:
        bool write_log();
        int choose_queue(); 
        void close_logfile();
        void create_logfile(bool truncate);
        bool need_roll_file() const { return _current_bytes > _max_bytes; }
        void roll_file();
        bool need_create_file() const;

    private:    
        int _log_fd;
        atomic_t _log_number;
        volatile bool _waiting;                     /** 写日志线程是否正在等待有日志可写？ */
        volatile uint32_t _queue_index;             /** 日志队列索引 */
        uint16_t _queue_number;                     /** 日志队列个数 */
        util::CArrayQueue<const char*>** _queue_array;    /** 日志队列数组 */
        CLock* _lock_array;
        CEvent _event;
        CLock _lock;

    private:        
        bool _screen_enabled;        
        uint32_t _max_bytes;
        uint32_t _current_bytes; 
        uint16_t _backup_number;
        char _log_path[PATH_MAX];
        char _log_filename[FILENAME_MAX];
    }*_log_thread;
};

SYS_NAMESPACE_END
#endif // SYS_LOGGER_H
