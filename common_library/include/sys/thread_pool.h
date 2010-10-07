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
 * Author: jian yi, eyjian@qq.com
 */
#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include "sys/sys_util.h"
SYS_NAMESPACE_BEGIN

/***
  * �̳߳�ģ���࣬ģ�����Ϊ�߳���
  */
template <class ThreadClass>
class CThreadPool
{
public:
    /** ����һ���̳߳� */
    CThreadPool()
        :_next_thread(0)
        ,_thread_count(0)
        ,_thread_array(NULL)
    {
    }

    /** �����̳߳أ��������̳߳��е������̣߳�
      * ���̴߳����ɹ��󣬲�����������������״̬�����Ǵ��ڵȴ�״̬��
      * ������Ҫ�������ǣ��÷���μ������ʾ��
      * @thread_count: �̳߳��е��̸߳���
      * @return: �ɹ�����true�����򷵻�false
      * @exception: ���׳�CSyscallException�쳣
      */
    bool create(uint16_t thread_count)
    {
        _thread_array = new ThreadClass*[thread_count];
        for (uint16_t i=0; i<thread_count; ++i)
        {
            _thread_array[i] = new ThreadClass;
            _thread_array[i]->inc_refcount();
        }
        for (uint16_t i=0; i<thread_count; ++i)
        {
            try
            {                
                if (!_thread_array[i]->start())
                {
                    destroy();
                    return false;
                }

				_thread_array[i]->set_index(i);
                ++_thread_count;
            }
            catch (...)
            {
                destroy();
                throw;
            }
        }

        return true;
    }

    /** �����̳߳أ������ȴ������߳��˳���Ȼ��ɾ���߳� */
    void destroy()
    {
        uint16_t thread_count = _thread_count;
        for (uint16_t i=thread_count; i>0; --i)
        {
            _thread_array[i-1]->stop();
            _thread_array[i-1]->dec_refcount();
            --_thread_count;
        }

        delete []_thread_array;
        _thread_array = NULL;
    }

    /***
      * �����̳߳أ������е������̻߳��ѣ�
      * Ҳ���Ե������ø����̵߳�wakeup�����ǻ���
      */
    void activate ()
    {
        for (uint16_t i=0; i<_thread_count; ++i)
            _thread_array[i-1]->wakeup();
    }

    /** �õ��̳߳��е��̸߳��� */
    uint16_t get_thread_count() const { return _thread_count; }
    /** �õ��̳߳��е��߳����� */
    ThreadClass** get_thread_array() const { return _thread_array; }

    /***
      * �õ�ָ���¸��̵߳�ָ�룬�ӵ�һ����ʼѭ�����������ս�㣬���ﵽ���һ��ʱ����ָ���һ����
      * ��ҪӦ���ڲ�����ѯ��ʽ��һ�������������������̡߳�
      */
    ThreadClass* get_next_thread()
    {
        if (_next_thread >= _thread_count)
            _next_thread = 0;

        return _thread_array[_next_thread++];
    }

private:    
    uint16_t _next_thread;
    uint16_t _thread_count;    
    ThreadClass** _thread_array;
};

SYS_NAMESPACE_END
#endif // THREAD_POOL_H