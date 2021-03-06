/*
 * Copyright © 2018 VMware, Inc.  All Rights Reserved.
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

int main(VOID)
{
    int ret = 0;

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(Test_LwCADbAddCA),
        cmocka_unit_test(Test_LwCADbAddCertData),
        cmocka_unit_test(Test_LwCADbCheckCA),
        cmocka_unit_test(Test_LwCADbCheckCertData),
        cmocka_unit_test(Test_LwCADbGetCA),
        cmocka_unit_test(Test_LwCADbGetCACertificates),
        cmocka_unit_test(Test_LwCADbGetCertData),
        cmocka_unit_test(Test_LwCADbGetCACRLNumber),
        cmocka_unit_test(Test_LwCADbGetCAStatus),
        cmocka_unit_test(Test_LwCADbGetCAAuthBlob),
        cmocka_unit_test(Test_LwCADbLockCA),
        cmocka_unit_test(Test_LwCADbLockCert),
        cmocka_unit_test(Test_LwCADbUnlockCA),
        cmocka_unit_test(Test_LwCADbUnlockCert),
        cmocka_unit_test(Test_LwCADbGetParentCAId),
        cmocka_unit_test(Test_LwCADbUpdateCA),
        cmocka_unit_test(Test_LwCADbUpdateCAStatus),
        cmocka_unit_test(Test_LwCADbUpdateCertData),
        cmocka_unit_test(Test_LwCADbUpdateCACRLNumber),
    };

    ret = cmocka_run_group_tests(tests, Test_LwCADbInitCtx, Test_LwCADbFreeCtx);
    if (ret)
    {
        fail_msg("%s", "MutentCA DB tests failed");
    }

    const struct CMUnitTest tests1[] = {
        cmocka_unit_test(Test_LwCADbCAData),
        cmocka_unit_test(Test_LwCADbCAData_Invalid),
        cmocka_unit_test(Test_LwCADbCertData),
        cmocka_unit_test(Test_LwCADbCertData_Invalid),
        cmocka_unit_test_setup_teardown(Test_LwCADbInitCtx_Invalid, Test_LwCADbInitCtx, Test_LwCADbFreeCtx),
        cmocka_unit_test(Test_LwCADbAddCA_Invalid),
        cmocka_unit_test(Test_LwCADbAddCertData_Invalid),
        cmocka_unit_test(Test_LwCADbCheckCA_Invalid),
        cmocka_unit_test(Test_LwCADbCheckCertData_Invalid),
        cmocka_unit_test(Test_LwCADbGetCA_Invalid),
        cmocka_unit_test(Test_LwCADbGetCACertificates_Invalid),
        cmocka_unit_test(Test_LwCADbGetCertData_Invalid),
        cmocka_unit_test(Test_LwCADbGetCAStatus_Invalid),
        cmocka_unit_test(Test_LwCADbGetCAAuthBlob_Invalid),
        cmocka_unit_test(Test_LwCADbLockCA_Invalid),
        cmocka_unit_test(Test_LwCADbLockCert_Invalid),
        cmocka_unit_test(Test_LwCADbUnlockCA_Invalid),
        cmocka_unit_test(Test_LwCADbUnlockCert_Invalid),
        cmocka_unit_test(Test_LwCADbGetCACRLNumber_Invalid),
        cmocka_unit_test(Test_LwCADbGetParentCAId_Invalid),
        cmocka_unit_test(Test_LwCADbUpdateCA_Invalid),
        cmocka_unit_test(Test_LwCADbUpdateCAStatus_Invalid),
        cmocka_unit_test(Test_LwCADbUpdateCertData_Invalid),
        cmocka_unit_test(Test_LwCADbUpdateCACRLNumber_Invalid),
    };

    ret = cmocka_run_group_tests(tests1, NULL, NULL);
    if (ret)
    {
        fail_msg("%s", "MutentCA DB tests failed");
    }

    const struct CMUnitTest postTests[] = {
        cmocka_unit_test_setup_teardown(Test_LwCAPostDbInitCtx, NULL, NULL),
        cmocka_unit_test_setup_teardown(Test_LwCAPostDbFreeCtx, NULL, NULL),
        cmocka_unit_test_setup_teardown(Test_LwCAPostDbAddCA,
                                        PreTest_LwCAPostPlugin,
                                        PostTest_LwCAPostPlugin),
        cmocka_unit_test_setup_teardown(Test_LwCASerializeRootCAToJson,
                                        PreTest_LwCAPostPlugin,
                                        PostTest_LwCAPostPlugin),
        cmocka_unit_test_setup_teardown(Test_LwCASerializeIntermediateCAToJson,
                                        PreTest_LwCAPostPlugin,
                                        PostTest_LwCAPostPlugin),
        cmocka_unit_test_setup_teardown(Test_LwCASerializeConfigRootCAToJson,
                                        PreTest_LwCAPostPlugin,
                                        PostTest_LwCAPostPlugin),
        cmocka_unit_test_setup_teardown(Test_LwCADeserializeJsonToRootCA,
                                        PreTest_LwCAPostPlugin,
                                        PostTest_LwCAPostPlugin),
        cmocka_unit_test_setup_teardown(Test_LwCADeserializeJsonToIntrCA,
                                        PreTest_LwCAPostPlugin,
                                        PostTest_LwCAPostPlugin),
        cmocka_unit_test_setup_teardown(Test_LwCAPostDbCheckCA,
                                        PreTest_LwCAPostPlugin,
                                        PostTest_LwCAPostPlugin),
        cmocka_unit_test_setup_teardown(Test_LwCAPostDbGetCA,
                                        PreTest_LwCAPostPlugin,
                                        PostTest_LwCAPostPlugin),
        cmocka_unit_test_setup_teardown(Test_LwCAPostDbUpdateCA,
                                        PreTest_LwCAPostPlugin,
                                        PostTest_LwCAPostPlugin),
        cmocka_unit_test_setup_teardown(Test_LwCAUpdateRootCARequestBody,
                                        PreTest_LwCAPostPlugin,
                                        PostTest_LwCAPostPlugin),
        cmocka_unit_test_setup_teardown(Test_LwCAPostGetParentCAId,
                                        PreTest_LwCAPostPlugin,
                                        PostTest_LwCAPostPlugin),
        cmocka_unit_test_setup_teardown(Test_LwCAAddCertData,
                                        PreTest_LwCAPostPlugin,
                                        PostTest_LwCAPostPlugin),
        cmocka_unit_test_setup_teardown(Test_LwCASerializeCertDataToJson,
                                        PreTest_LwCAPostPlugin,
                                        PostTest_LwCAPostPlugin),
        cmocka_unit_test_setup_teardown(Test_LwCAGetCertData,
                                        PreTest_LwCAPostPlugin,
                                        PostTest_LwCAPostPlugin),
        cmocka_unit_test_setup_teardown(Test_LwCADeserializeCertData,
                                        PreTest_LwCAPostPlugin,
                                        PostTest_LwCAPostPlugin),
        cmocka_unit_test_setup_teardown(Test_LwCAUpdateCertData,
                                        PreTest_LwCAPostPlugin,
                                        PostTest_LwCAPostPlugin),
        cmocka_unit_test_setup_teardown(Test_LwCALockData,
                                        PreTest_LwCAPostPlugin,
                                        PostTest_LwCAPostPlugin),
    };

    ret = cmocka_run_group_tests(postTests, NULL, NULL);
    if (ret)
    {
        fail_msg("%s", "MutentCA POST DB tests failed");
    }

    return ret;
}
