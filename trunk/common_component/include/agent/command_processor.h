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
#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H
#include <util/util_config.h>
MOOON_NAMESPACE_BEGIN

/***
  * ������������ڴ���Master�·��ĸ�������
  */
class CALLBACK_INTERFACE ICommandProcessor
{    
public:
    /** �����������������α��������� */
    virtual ~ICommandProcessor() {}

    /***
      * ���������
      * @command: Master�·�������
      */
    virtual void handle(void* command, char* body, uint32_t body_size) = 0;
};

MOOON_NAMESPACE_END
#endif // COMMAND_PROCESSOR_H