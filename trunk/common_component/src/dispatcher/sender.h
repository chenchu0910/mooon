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
#ifndef SENDER_H
#define SENDER_H
#include <net/tcp_client.h>
#include "send_queue.h"
#include "send_thread_pool.h"
MY_NAMESPACE_BEGIN

class CSender: public net::CTcpClient
{    
public:
    ~CSender();
    CSender(CSendThreadPool* thread_pool, int32_t node_id, uint32_t queue_max, IReplyHandler* reply_handler);          
    bool push_message(dispach_message_t* message);    
    
private:
    virtual void before_close();

private:
    void clear_message();    
    bool do_handle_reply();    
    dispach_message_t* get_current_message();
    void reset_current_message(bool delete_message);    
    net::epoll_event_t do_send_message(void* ptr, uint32_t events);

protected:
    int32_t get_node_id() const;
    net::epoll_event_t do_handle_epoll_event(void* ptr, uint32_t events);
    
private:
    CSendThreadPool* _thread_pool;
    
private:    
    int32_t _node_id;
    CSendQueue _send_queue;      
    IReplyHandler* _reply_handler;

private:
    uint32_t _current_offset;            // ��ǰ��Ϣ�Ѿ����͵��ֽ���
    dispach_message_t* _current_message; // ��ǰ���ڷ��͵���Ϣ�����ΪNULL����Ҫ�Ӷ�����ȡһ��
};

MY_NAMESPACE_END
#endif // SENDER_H