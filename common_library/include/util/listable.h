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
#ifndef MOOON_UTIL_LISTABLE_H
#define MOOON_UTIL_LISTABLE_H
#include "util/util_config.h"
UTIL_NAMESPACE_BEGIN

/***
  * 可链表对象的基类
  */
class CListable
{
public:
    CListable()
        :_in_list(false)
        ,_next(NULL)
        ,_prev(NULL)
    {
    }

    /** 是否在链表中 */
    bool is_in_list() const
    {
        return _in_list;
    }

    /** 设置是否在链表中 */
    void set_in_list(bool in_list)
    {
        _in_list = in_list;
    }

    /** 得到下一个可链表对象 */
    CListable* get_next() const { return _next; }

    /** 得到前一个可链表对象 */
    CListable* get_prev() const { return _prev; }

    /** 关联下一个可链表对象 */
    void set_next(CListable* next) { _next = next; }

    /** 关联前一个可链表对象 */
    void set_prev(CListable* prev) { _prev = prev; }

private:
    bool _in_list; /** 是否在链表中 */
    CListable* _next;
    CListable* _prev;
};

UTIL_NAMESPACE_END
#endif // MOOON_UTIL_LISTABLE_H
