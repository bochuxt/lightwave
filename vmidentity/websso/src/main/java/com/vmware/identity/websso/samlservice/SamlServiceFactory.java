/*
 *  Copyright (c) 2012-2015 VMware, Inc.  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may not
 *  use this file except in compliance with the License.  You may obtain a copy
 *  of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, without
 *  warranties or conditions of any kind, EITHER EXPRESS OR IMPLIED.  See the
 *  License for the specific language governing permissions and limitations
 *  under the License.
 */
package com.vmware.identity.websso.samlservice;

import java.security.PrivateKey;
import java.security.cert.CertPath;

import com.vmware.identity.saml.SignatureAlgorithm;

/**
 * Saml Service interface.
 *
 */
public interface SamlServiceFactory {
    /**
     * Create an instance of Saml Service
     *
     * @return
     */
    SamlService createSamlService(PrivateKey privateKey,
            SignatureAlgorithm signAlgorithm,
            SignatureAlgorithm checkAlgorithm,
            String issuer, CertPath certPath);
}