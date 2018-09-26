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

#define LWCA_DB_CONFIG "./test-mutentcadb-config/test-mutentcadb-config.json"
#define TEST_CA_ID "testId"
#define TEST_PARENT_CA_ID "testParentId"

VOID
Test_LwCADbInitCtx(
    VOID **state
    )
{
    DWORD dwError = 0;
    PLWCA_JSON_OBJECT pJson =  NULL;

    dwError = LwCAJsonLoadObjectFromFile(LWCA_DB_CONFIG, &pJson);
    assert_int_equal(dwError, 0);

    dwError = LwCADbInitCtx(pJson);
    assert_int_equal(dwError, 0);

    LwCAJsonCleanupObject(pJson);
}

VOID
Test_LwCADbFreeCtx(
    VOID **state
    )
{
    LwCADbFreeCtx();
}

/*
 * Test helper method which creates db certificate array object with two certificates.
 */
VOID
TestCreateCertificates(
    PLWCA_DB_CERTIFICATE_ARRAY  *ppCertArray
    )
{
    DWORD dwError = 0;
    PSTR *ppCertificates = NULL;

    dwError = LwCAAllocateMemory(2 * sizeof(ppCertificates[0]), (PVOID*)&ppCertificates);
    assert_int_equal(dwError, 0);

    dwError = LwCAAllocateStringA("dummyCert1", &ppCertificates[0]);
    assert_int_equal(dwError, 0);

    dwError = LwCAAllocateStringA("dummyCert2", &ppCertificates[1]);
    assert_int_equal(dwError, 0);

    dwError = LwCADbCreateCertArray(ppCertificates, 2, ppCertArray);
    assert_int_equal(dwError, 0);

    assert_non_null(ppCertArray);

    LWCA_SAFE_FREE_MEMORY(ppCertificates[1]);
    LWCA_SAFE_FREE_MEMORY(ppCertificates[0]);
    LWCA_SAFE_FREE_MEMORY(ppCertificates);
}

VOID
Test_LwCADbCAData(
    VOID **state
    )
{
    DWORD dwError = 0;
    PLWCA_DB_CA_DATA pCAData = NULL;
    PLWCA_DB_CERTIFICATE_ARRAY pCertArray = NULL;
    PLWCA_DB_ENCRYPTED_KEY pEncryptedPrivateKey = NULL;
    PLWCA_DB_ENCRYPTED_KEY pEncryptedEncryptionKey = NULL;
    BYTE testKey1[20] = "testKey1";
    BYTE testKey2[20] = "testKey2";

    dwError = LwCADbCreateEncryptedKey(testKey1, 20, &pEncryptedPrivateKey);
    assert_int_equal(dwError, 0);

    dwError = LwCADbCreateEncryptedKey(testKey2, 20, &pEncryptedEncryptionKey);
    assert_int_equal(dwError, 0);

    TestCreateCertificates(&pCertArray);
    assert_non_null(pCertArray);
    assert_non_null(pEncryptedPrivateKey);
    assert_non_null(pEncryptedEncryptionKey);

    dwError = LwCADbCreateCAData("cn=CA",
                                "cn=CA",
                                pCertArray,
                                pEncryptedPrivateKey,
                                pEncryptedEncryptionKey,
                                "-1",
                                "-1",
                                LWCA_DB_CA_STATUS_ACTIVE,
                                &pCAData);
    assert_int_equal(dwError, 0);

    assert_non_null(pCAData);
    assert_string_equal(pCAData->pszIssuer, "cn=CA");
    assert_string_equal(pCAData->pszSubject, "cn=CA");

    assert_non_null(pCAData->pCertificates);
    assert_int_equal(pCAData->pCertificates->dwCount, 2);
    assert_non_null(pCAData->pCertificates->ppCertificates);
    assert_string_equal(pCAData->pCertificates->ppCertificates[0], "dummyCert1");
    assert_string_equal(pCAData->pCertificates->ppCertificates[1], "dummyCert2");

    assert_non_null(pCAData->pEncryptedPrivateKey);
    assert_memory_equal(pCAData->pEncryptedPrivateKey->pData, testKey1, 20);
    assert_non_null(pCAData->pEncryptedEncryptionKey);
    assert_memory_equal(pCAData->pEncryptedEncryptionKey->pData, testKey2, 20);

    assert_string_equal(pCAData->pszTimeValidFrom, "-1");
    assert_string_equal(pCAData->pszTimeValidTo, "-1");

    assert_int_equal(pCAData->status, LWCA_DB_CA_STATUS_ACTIVE);

    LwCADbFreeEncryptedKey(pEncryptedPrivateKey);
    LwCADbFreeEncryptedKey(pEncryptedEncryptionKey);
    LwCADbFreeCertificates(pCertArray);
    LwCADbFreeCAData(pCAData);
}

VOID
Test_LwCADbCAData_Invalid(
    VOID **state
    )
{
    DWORD dwError = 0;
    PLWCA_DB_CA_DATA pCAData = NULL;
    PLWCA_DB_CERTIFICATE_ARRAY pCertArray = NULL;
    PLWCA_DB_ENCRYPTED_KEY pEncryptedPrivateKey = NULL;
    PLWCA_DB_ENCRYPTED_KEY pEncryptedEncryptionKey = NULL;
    BYTE testKey1[20] = "testKey1";
    BYTE testKey2[20] = "testKey2";

    dwError = LwCADbCreateEncryptedKey(testKey1, 20, &pEncryptedPrivateKey);
    assert_int_equal(dwError, 0);

    dwError = LwCADbCreateEncryptedKey(testKey2, 20, &pEncryptedEncryptionKey);
    assert_int_equal(dwError, 0);

    TestCreateCertificates(&pCertArray);
    assert_non_null(pCertArray);
    assert_non_null(pEncryptedPrivateKey);
    assert_non_null(pEncryptedEncryptionKey);

    dwError = LwCADbCreateCAData(NULL,
                                NULL,
                                pCertArray,
                                pEncryptedPrivateKey,
                                pEncryptedEncryptionKey,
                                NULL,
                                NULL,
                                LWCA_DB_CA_STATUS_ACTIVE,
                                &pCAData);
    assert_int_equal(dwError, LWCA_ERROR_INVALID_PARAMETER);
    assert_null(pCAData);

    dwError = LwCADbCreateCAData("cn=CA",
                                "cn=CA",
                                pCertArray,
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                LWCA_DB_CA_STATUS_ACTIVE,
                                &pCAData);
    assert_int_equal(dwError, LWCA_ERROR_INVALID_PARAMETER);
    assert_null(pCAData);

    dwError = LwCADbCreateCAData("cn=CA",
                                "cn=CA",
                                NULL,
                                pEncryptedPrivateKey,
                                pEncryptedEncryptionKey,
                                NULL,
                                NULL,
                                LWCA_DB_CA_STATUS_ACTIVE,
                                &pCAData);
    assert_int_equal(dwError, LWCA_ERROR_INVALID_PARAMETER);
    assert_null(pCAData);

    LwCADbFreeEncryptedKey(pEncryptedPrivateKey);
    LwCADbFreeEncryptedKey(pEncryptedEncryptionKey);
    LwCADbFreeCertificates(pCertArray);
    LwCADbFreeCAData(pCAData);
}


VOID
Test_LwCADbCertData(
    VOID **state
    )
{
    DWORD dwError = 0;
    PLWCA_DB_CERT_DATA pCertData = NULL;

    dwError = LwCADbCreateCertData("dummySerial",
                                    "cn=CA",
                                    "-1",
                                    "-1",
                                    "dummyReason",
                                    "dummyDate",
                                    LWCA_DB_CERT_STATUS_ACTIVE,
                                    &pCertData);
    assert_int_equal(dwError, 0);

    assert_non_null(pCertData);
    assert_string_equal(pCertData->pszSerialNumber, "dummySerial");
    assert_string_equal(pCertData->pszIssuer, "cn=CA");
    assert_string_equal(pCertData->pszTimeValidFrom, "-1");
    assert_string_equal(pCertData->pszTimeValidTo, "-1");
    assert_string_equal(pCertData->pszRevokedReason, "dummyReason");
    assert_string_equal(pCertData->pszRevokedDate, "dummyDate");
    assert_int_equal(pCertData->status, LWCA_DB_CERT_STATUS_ACTIVE);

    LwCADbFreeCertData(pCertData);
}

VOID
Test_LwCADbCertData_Invalid(
    VOID **state
    )
{
    DWORD dwError = 0;
    PLWCA_DB_CERT_DATA pCertData = NULL;

    dwError = LwCADbCreateCertData(NULL,
                                    "cn=CA",
                                    NULL,
                                    NULL,
                                    "dummyReason",
                                    "dummyDate",
                                    LWCA_DB_CERT_STATUS_ACTIVE,
                                    &pCertData);
    assert_int_equal(dwError, LWCA_ERROR_INVALID_PARAMETER);
    assert_null(pCertData);

    dwError = LwCADbCreateCertData("dummySerial",
                                    "cn=CA",
                                    "-1",
                                    "-1",
                                    NULL,
                                    NULL,
                                    LWCA_DB_CERT_STATUS_ACTIVE,
                                    &pCertData);
    assert_int_equal(dwError, LWCA_ERROR_INVALID_PARAMETER);
    assert_null(pCertData);

    LwCADbFreeCertData(pCertData);
}
