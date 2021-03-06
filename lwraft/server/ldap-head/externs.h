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



/*
 * Module Name: Directory ldap-head
 *
 * Filename: externs.h
 *
 * Abstract:
 *
 *
 * Externs
 *
 */

extern Sockbuf_IO* gpVdirBerSockbufIOOpenssl;

extern VMDIR_OP_STATISTIC_GLOBALS gVmdirOPStatisticGlobals;

extern VMDIR_OPENSSL_GLOBALS gVmdirOpensslGlobals;

extern BOOLEAN VmDirRaftNeedReferral(PCSTR pszReqDn);

extern VOID VmDirSendLdapReferralResult( VDIR_OPERATION * op, PCSTR pszRefSuffix, PBOOLEAN pbRefSent);

extern DWORD VmDirRaftGetLeader(PSTR *);

extern PVM_METRICS_HISTOGRAM gpLdapMetrics[METRICS_LDAP_OP_COUNT]
                                           [METRICS_LDAP_OP_TYPE_COUNT]
                                            [METRICS_LDAP_ERROR_COUNT]
                                             [METRICS_LAYER_COUNT];
