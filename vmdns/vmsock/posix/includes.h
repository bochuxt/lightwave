/*
 * Copyright © 2012-2015 VMware, Inc.  All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the “License”); you may not
 * use this file except in compliance with the License.  You may obtain a copy
 * of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an “AS IS” BASIS, without
 * warranties or conditions of any kind, EITHER EXPRESS OR IMPLIED.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */


#include <config.h>

#include <vmdnssys.h>

#ifdef HAVE_SYS_EPOLL_H
#include <sys/epoll.h>
#endif

#include <sys/timerfd.h>

#include <vmdns.h>
#include <vmdnsdefines.h>

#include <vmdnscommon.h>
#include <vmsock.h>
#include <vmsockapi.h>
#include <vmsockposix.h>

#include "defines.h"
#include "structs.h"
#include "prototypes.h"
#include "externs.h"
