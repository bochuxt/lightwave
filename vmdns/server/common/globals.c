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

#include "includes.h"

VMW_DNS_DRIVER_GLOBALS gDNSDriverGlobals;
PVMW_DNS_DRIVER_GLOBALS gpSrvContext = &gDNSDriverGlobals;

PVM_METRICS_COUNTER gVmDnsRcodeErrorMetrics[METRICS_VDNS_RCODE_DOMAIN_COUNT]
                                            [METRICS_VDNS_RCODE_OP_COUNT]
                                             [METRICS_VDNS_RCODE_ERROR_COUNT];
PVM_METRICS_COUNTER gVmDnsCounterMetrics[VDNS_COUNTER_COUNT];
PVM_METRICS_HISTOGRAM gVmDnsHistogramMetrics[VDNS_HISTOGRAM_COUNT];
PVM_METRICS_GAUGE gVmDnsGaugeMetrics[VDNS_GAUGE_COUNT];
PVM_METRICS_CONTEXT gVmDnsMetricsContext = NULL;
