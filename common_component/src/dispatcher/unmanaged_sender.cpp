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
#include "send_thread.h"
#include "unmanaged_sender.h"
MY_NAMESPACE_BEGIN

CUnmanagedSender::CUnmanagedSender(CSendThreadPool* thread_pool, int32_t node_id, uint32_t queue_max, IReplyHandler* reply_handler)
    :CSender(thread_pool, node_id, queue_max, reply_handler)
{
}

void CUnmanagedSender::set_object(void* object)
{
    do_set_object(object);
}

bool CUnmanagedSender::send_message(dispach_message_t* message)
{
    return push_message(message);
}

net::epoll_event_t CUnmanagedSender::handle_epoll_event(void* ptr, uint32_t events)
{
    CSendThread* send_thread = static_cast<CSendThread*>(ptr);
    net::CTimeoutManager<CUnmanagedSender>* timeout_manager = send_thread->get_timeout_manager();
    timeout_manager->remove(this);

    net::epoll_event_t retval = do_handle_epoll_event(ptr, events);
    timeout_manager->push(this, send_thread->get_current_time());

    return retval;
}

MY_NAMESPACE_END