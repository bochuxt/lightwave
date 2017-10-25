/*
 * Copyright © 2017 VMware, Inc.  All Rights Reserved.
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

/*
 * We provide this function as callback to c-rest-engine,
 * c-rest-engine will use this callback upon receiving a request
 */
DWORD
VmDirHTTPRequestHandler(
    PVMREST_HANDLE  pRESTHandle,
    PREST_REQUEST   pRequest,
    PREST_RESPONSE* ppResponse,
    uint32_t        paramsCount
    )
{
    DWORD   dwError = 0;

    dwError = VmDirRESTRequestHandlerInternal(
            pRESTHandle, pRequest, ppResponse, paramsCount, TRUE);//TRUE - if HTTP request
    BAIL_ON_VMDIR_ERROR(dwError);

cleanup:
    return dwError;
error:
    VMDIR_LOG_ERROR(
            VMDIR_LOG_MASK_ALL,
            "%s failed, error (%d)",
            __FUNCTION__,
            dwError);
    goto cleanup;
}

DWORD
VmDirHTTPSRequestHandler(
    PVMREST_HANDLE  pRESTHandle,
    PREST_REQUEST   pRequest,
    PREST_RESPONSE* ppResponse,
    uint32_t        paramsCount
    )
{
    DWORD   dwError = 0;

    dwError = VmDirRESTRequestHandlerInternal(
            pRESTHandle, pRequest, ppResponse, paramsCount, FALSE);//FALSE - if HTTPS request
    BAIL_ON_VMDIR_ERROR(dwError);

cleanup:
    return dwError;
error:
    VMDIR_LOG_ERROR(
            VMDIR_LOG_MASK_ALL,
            "%s failed, error (%d)",
            __FUNCTION__,
            dwError);
    goto cleanup;
}

DWORD
VmDirRESTRequestHandlerInternal(
    PVMREST_HANDLE  pRESTHandle,
    PREST_REQUEST   pRequest,
    PREST_RESPONSE* ppResponse,
    uint32_t        paramsCount,
    BOOLEAN         bHttpRequest
    )
{
    DWORD   dwError = 0;
    DWORD   dwRestOpErr = 0;    // don't bail on this
    PVDIR_REST_OPERATION    pRestOp = NULL;
    VDIR_RAFT_ROLE  role = VDIR_RAFT_ROLE_CANDIDATE;

    if (!pRESTHandle || !pRequest || !ppResponse)
    {
        BAIL_WITH_VMDIR_ERROR(dwError, VMDIR_ERROR_INVALID_PARAMETER);
    }

    if (VmDirdState() == VMDIRD_STATE_SHUTDOWN)
    {
        goto cleanup;
    }

    dwRestOpErr = VmDirRESTOperationCreate(&pRestOp);
    if (dwRestOpErr)
    {
        dwError = VmDirRESTWriteSimpleErrorResponse(
                pRESTHandle, ppResponse, 500);  // 500 = Internal Server Error
        BAIL_ON_VMDIR_ERROR(dwError);
    }
    else
    {
        // Get the client IP
        dwError = VmRESTGetConnectionInfo(pRequest, &pRestOp->pszClientIP, &pRestOp->dwPort);
        BAIL_ON_VMDIR_ERROR(dwError);

        VmDirRaftGetRole(&role);
        if (role == VDIR_RAFT_ROLE_LEADER)
        {
            dwRestOpErr = VmDirRESTProcessRequest(
                    pRestOp, pRESTHandle, pRequest, paramsCount);

            dwError = VmDirRESTOperationWriteResponse(
                    pRestOp, pRESTHandle, ppResponse);
            BAIL_ON_VMDIR_ERROR(dwError);
        }
        else if (role == VDIR_RAFT_ROLE_FOLLOWER)
        {
            dwRestOpErr = VmDirRESTForwardRequest(
                    pRestOp, paramsCount, pRequest, pRESTHandle, bHttpRequest);

            dwError = VmDirRESTWriteProxyResponse(
                    pRestOp, ppResponse, pRESTHandle);
            BAIL_ON_VMDIR_ERROR(dwError);
        }
        else  // role == VDIR_RAFT_ROLE_CANDIDATE
        {
            dwError = VmDirRESTWriteSimpleErrorResponse(
                    pRESTHandle, ppResponse, 503);  // 503 = Service Unavailable
            BAIL_ON_VMDIR_ERROR(dwError);

            VMDIR_LOG_ERROR(
                    VMDIR_LOG_MASK_ALL,
                    "Error: %d Node in invalid state no leader. Status returned: 503 to client: %s",
                    VMDIR_ERROR_NO_LEADER,
                    VDIR_SAFE_STRING(pRestOp->pszClientIP));
        }
    }

cleanup:
    VmDirFreeRESTOperation(pRestOp);
    return dwError;

error:
    VMDIR_LOG_ERROR(
            VMDIR_LOG_MASK_ALL,
            "%s failed, error (%d), rest operation error (%d) for client: %s",
            __FUNCTION__,
            dwError,
            dwRestOpErr,
            pRestOp ? VDIR_SAFE_STRING(pRestOp->pszClientIP) : "");

    goto cleanup;
}

DWORD
VmDirRESTProcessRequest(
    PVDIR_REST_OPERATION    pRestOp,
    PVMREST_HANDLE          pRESTHandle,
    PREST_REQUEST           pRequest,
    uint32_t                paramsCount
    )
{
    DWORD   dwError = 0;
    PREST_API_METHOD    pMethod = NULL;

    if (!pRestOp || !pRESTHandle || !pRequest)
    {
        BAIL_WITH_VMDIR_ERROR(dwError, VMDIR_ERROR_INVALID_PARAMETER);
    }

    dwError = VmDirRESTOperationReadRequest(
            pRestOp, pRESTHandle, pRequest, paramsCount);
    BAIL_ON_VMDIR_ERROR(dwError);

    dwError = VmDirRESTAuth(pRestOp);
    BAIL_ON_VMDIR_ERROR(dwError);

    dwError = coapi_find_handler(
            gpVdirRestApiDef,
            pRestOp->pszPath,
            pRestOp->pszMethod,
            &pMethod);
    BAIL_ON_VMDIR_ERROR(dwError);

    dwError = pMethod->pFnImpl((void*)pRestOp, NULL);
    BAIL_ON_VMDIR_ERROR(dwError);

    VMDIR_LOG_INFO(
            VMDIR_LOG_MASK_ALL,
            "Leader received REST request from: %s request type: %s request URI: %s",
            VDIR_SAFE_STRING(pRestOp->pszClientIP),
            VDIR_SAFE_STRING(pRestOp->pszMethod),
            VDIR_SAFE_STRING(pRestOp->pszPath));

cleanup:
    VMDIR_SET_REST_RESULT(pRestOp, NULL, dwError, NULL);
    return dwError;

error:
    VMDIR_LOG_ERROR(
            VMDIR_LOG_MASK_ALL,
            "%s failed, error (%d) for client: %s",
            __FUNCTION__,
            dwError,
            VDIR_SAFE_STRING(pRestOp->pszClientIP));

    goto cleanup;
}

DWORD
VmDirRESTWriteSimpleErrorResponse(
    PVMREST_HANDLE  pRESTHandle,
    PREST_RESPONSE* ppResponse,
    int             httpStatus
    )
{
    DWORD   dwError = 0;
    PVDIR_HTTP_ERROR    pHttpError = NULL;

    if (!pRESTHandle || !ppResponse)
    {
        BAIL_WITH_VMDIR_ERROR(dwError, VMDIR_ERROR_INVALID_PARAMETER);
    }

    dwError = VmRESTSetHttpStatusVersion(ppResponse, "HTTP/1.1");
    BAIL_ON_VMDIR_ERROR(dwError);

    pHttpError = VmDirRESTGetHttpError(httpStatus);

    dwError = VmRESTSetHttpStatusCode(ppResponse, pHttpError->pszHttpStatus);
    BAIL_ON_VMDIR_ERROR(dwError);

    dwError = VmRESTSetHttpReasonPhrase(ppResponse, pHttpError->pszHttpReason);
    BAIL_ON_VMDIR_ERROR(dwError);

    dwError = VmRESTSetHttpHeader(ppResponse, "Connection", "close");
    BAIL_ON_VMDIR_ERROR(dwError);

cleanup:
    return dwError;

error:
    VMDIR_LOG_ERROR(
            VMDIR_LOG_MASK_ALL,
            "%s failed, error (%d)",
            __FUNCTION__,
            dwError);

    goto cleanup;
}
