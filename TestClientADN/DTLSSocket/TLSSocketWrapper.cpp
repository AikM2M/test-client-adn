/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "TLSSocketWrapper.h"
#include "drivers/Timer.h"

#define TRACE_GROUP "TLSW"
#include "mbed-trace/mbed_trace.h"
#include "mbed_mem_trace.h"
#include "mbedtls/debug.h"
//#include "mbedtls/config.h"
//#include "mbedtls/timing.h"
//#include "timing_alt.h"

TLSSocketWrapper::TLSSocketWrapper(Socket *transport, const char *hostname) :
    _client_auth(false),
    _keep_transport_open(false),
    _handshake_completed(false),
    _transport(transport)
{
    tls_init();
    if (hostname) {
        set_hostname(hostname);
    }
}

TLSSocketWrapper::~TLSSocketWrapper() {
    if (_transport) {
        close();
    }
}

void TLSSocketWrapper::set_hostname(const char *hostname)
{
    if (is_tls_allocated()) {
        pc.printf("ssl hostname is set\n");
        mbedtls_ssl_set_hostname(_ssl, hostname);
    }
}

void TLSSocketWrapper::keep_transport_open()
{
    _keep_transport_open = true;
}

nsapi_error_t TLSSocketWrapper::set_root_ca_cert(const void *root_ca, size_t len)
{
    if (!is_tls_allocated()) {
        return NSAPI_ERROR_NO_SOCKET;
    }
    /* Parse CA certification */
    int ret;
    if ((ret = mbedtls_x509_crt_parse(_cacert, static_cast<const unsigned char *>(root_ca),
                        len)) != 0) {
        print_mbedtls_error("mbedtls_x509_crt_parse", ret);
        return NSAPI_ERROR_PARAMETER;
    }
        pc.printf("Root CA Certificate Setting ... OK\n");                                     
    return NSAPI_ERROR_OK;

}
nsapi_error_t TLSSocketWrapper::set_root_ca_cert(const char *root_ca_pem)
{
    return set_root_ca_cert(root_ca_pem, strlen(root_ca_pem) + 1);
}

nsapi_error_t TLSSocketWrapper::set_client_cert_key(const char *client_cert_pem, const char *client_private_key_pem)
{
    return set_client_cert_key(client_cert_pem, strlen(client_cert_pem) + 1, client_private_key_pem, strlen(client_private_key_pem) + 1);
}

nsapi_error_t TLSSocketWrapper::set_client_cert_key(const void *client_cert, size_t client_cert_len,
        const void *client_private_key_pem, size_t client_private_key_len)
{
    if (!is_tls_allocated()) {
        return NSAPI_ERROR_NO_SOCKET;
    }
    int ret;
    
    if((NULL != client_cert) && (NULL != client_private_key_pem)) {
        mbedtls_x509_crt_init(_clicert);
        if((ret = mbedtls_x509_crt_parse(_clicert, static_cast<const unsigned char *>(client_cert),
                client_cert_len)) != 0) {
            print_mbedtls_error("mbedtls_x509_crt_parse", ret);
            return NSAPI_ERROR_PARAMETER;
        }
                pc.printf("Client Certificate Setting ... ok\n");              
        mbedtls_pk_init(_pkctx);
        if((ret = mbedtls_pk_parse_key(_pkctx, static_cast<const unsigned char *>(client_private_key_pem),
                client_private_key_len, NULL, 0)) != 0) {
            print_mbedtls_error("mbedtls_pk_parse_key", ret);
            return NSAPI_ERROR_PARAMETER;
        }
                pc.printf("Client pvt. Key Setting ... ok\n");                 
        _client_auth = true;
    }
    return NSAPI_ERROR_OK;
}

     /* The supported CERT ciphersuites are as follows:     
             CipherSuite TLS_RSA_WITH_AES_128_CCM       = {0xC0,0x9C}
             CipherSuite TLS_RSA_WITH_AES_256_CCM       = {0xC0,0x9D)
             CipherSuite TLS_DHE_RSA_WITH_AES_128_CCM   = {0xC0,0x9E}
             CipherSuite TLS_DHE_RSA_WITH_AES_256_CCM   = {0xC0,0x9F}
             CipherSuite TLS_RSA_WITH_AES_128_CCM_8     = {0xC0,0xA0}
             CipherSuite TLS_RSA_WITH_AES_256_CCM_8     = {0xC0,0xA1)
             CipherSuite TLS_DHE_RSA_WITH_AES_128_CCM_8 = {0xC0,0xA2}
             CipherSuite TLS_DHE_RSA_WITH_AES_256_CCM_8 = {0xC0,0xA3}
        // The supported PSK ciphersuites are as follows:
             CipherSuite TLS_PSK_WITH_AES_128_CCM       = {0xC0,0xA4}
             CipherSuite TLS_PSK_WITH_AES_256_CCM       = {0xC0,0xA5)
             CipherSuite TLS_DHE_PSK_WITH_AES_128_CCM   = {0xC0,0xA6}
             CipherSuite TLS_DHE_PSK_WITH_AES_256_CCM   = {0xC0,0xA7}
             CipherSuite TLS_PSK_WITH_AES_128_CCM_8     = {0xC0,0xA8}
             CipherSuite TLS_PSK_WITH_AES_256_CCM_8     = {0xC0,0xA9)
             CipherSuite TLS_PSK_DHE_WITH_AES_128_CCM_8 = {0xC0,0xAA}
             CipherSuite TLS_PSK_DHE_WITH_AES_256_CCM_8 = {0xC0,0xAB} 
        // The supported PSK Cipher is (TLS)
             TLS_PSK_WITH_RC4_128_SHA          = { 0x00, 0x8A };
             TLS_PSK_WITH_3DES_EDE_CBC_SHA     = { 0x00, 0x8B };
             TLS_PSK_WITH_AES_128_CBC_SHA      = { 0x00, 0x8C };
             TLS_PSK_WITH_AES_256_CBC_SHA      = { 0x00, 0x8D };    */

int TLSSocketWrapper::set_client1_kpsa_kpsaID_cipher(){ 
    const unsigned char _kpsa[5] = { 0x01, 0x02, 0x03, 0x04, 0x05 }; 
    const unsigned char *_kpsaID = "AE123-LOCK@in.provider.com"; // 15 Bytes PSK-ID     
    
    int kpsa_len = strlen((const char*) _kpsa);
    int kpsaID_len = strlen((const char*) _kpsaID);
    int ret;
    
    mbedtls_ssl_set_hs_psk(_ssl, _kpsa, kpsa_len);
    if( (ret = mbedtls_ssl_conf_psk(_ssl_conf, _kpsa, kpsa_len , _kpsaID, kpsaID_len)) != 0){
        print_mbedtls_error("mbedtls_psk_error", ret);
        // return NSAPI_ERROR_PARAMETER;
        return -1;
    } 
    pc.printf("Ok.\n"); 
    return 0; 
}

int TLSSocketWrapper::set_client2_kpsa_kpsaID_cipher(){ 
    //const unsigned char _kpsa[5] = { 0x05, 0x04, 0x03, 0x02, 0x01 }; 
    //const unsigned char *_kpsaID = "AE456-LOCK@in.provider.com"; // 15 Bytes PSK-ID     
    const unsigned char _kpsa[5] = { 0x01, 0x02, 0x03, 0x04, 0x05 }; 
    const unsigned char *_kpsaID = "AE123-LOCK@in.provider.com"; // 15 Bytes PSK-ID  
    int kpsa_len = strlen((const char*) _kpsa);
    int kpsaID_len = strlen((const char*) _kpsaID);
    int ret;
    
    mbedtls_ssl_set_hs_psk(_ssl, _kpsa, kpsa_len);
    if( (ret = mbedtls_ssl_conf_psk(_ssl_conf, _kpsa, kpsa_len , _kpsaID, kpsaID_len)) != 0){
        print_mbedtls_error("mbedtls_psk_error", ret);
        // return NSAPI_ERROR_PARAMETER;
        return -1;
    } 
    pc.printf("Ok.\n"); 
    return 0; 
}

nsapi_error_t TLSSocketWrapper::do_handshake() {
    nsapi_error_t _error;
    const char DRBG_PERS[] = "mbed TLS client";
   
        const int *cipherArray;
    cipherArray = mbedtls_ssl_list_ciphersuites();
    pc.printf("[+] The allowed ciphersuites are: \n");
    int i = 0;
    while (cipherArray[i] != 0) {
        pc.printf("[.] cipherArray[%d] = %d\n", i, cipherArray[i]);
        i++;
    } 
    
    if (!_transport) {
        pc.printf("[-] transport/socket not available\n");
        return NSAPI_ERROR_NO_SOCKET;
    }
    if (!is_tls_allocated()) {
        pc.printf("[-] no tls allocated\n");
        return NSAPI_ERROR_NO_SOCKET;
    }

    _transport->set_blocking(false);
    /*
     * Initialize TLS-related stuf.
     */
    int ret;
    if ((ret = mbedtls_ctr_drbg_seed(_ctr_drbg, mbedtls_entropy_func, _entropy,
                        (const unsigned char *) DRBG_PERS,
                        sizeof (DRBG_PERS))) != 0) {
        print_mbedtls_error("mbedtls_crt_drbg_init", ret);
        _error = ret;
        return _error;
    }

    pc.printf("[+] Configuring a dtls-client session initiation ... ");
    mbedtls_ssl_conf_endpoint(_ssl_conf, MBEDTLS_SSL_IS_CLIENT);
    mbedtls_ssl_conf_transport(_ssl_conf, MBEDTLS_SSL_TRANSPORT_DATAGRAM);

    tr_info("mbedtls_ssl_config_defaults()");
    if ((ret = mbedtls_ssl_config_defaults(_ssl_conf,
                    MBEDTLS_SSL_IS_CLIENT,
                    MBEDTLS_SSL_TRANSPORT_DATAGRAM,
                    MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
        print_mbedtls_error("mbedtls_ssl_config_defaults", ret);
        _error = ret;
        return _error;
    } 
    tr_info("mbedtls_ssl_conf_ca_chain()");
    mbedtls_ssl_conf_ca_chain(_ssl_conf, _cacert, NULL);     
    tr_info("mbedtls_ssl_conf_rng()");
    mbedtls_ssl_conf_rng(_ssl_conf, mbedtls_ctr_drbg_random, _ctr_drbg);
    pc.printf("ok\n");
    
    /* It is possible to disable authentication by passing
      MBEDTLS_SSL_VERIFY_NONE in the call to mbedtls_ssl_conf_authmode()
     */
    pc.printf("[+] Configuring the Session Information ... ");
    tr_info("mbedtls_ssl_conf_authmode()");
    mbedtls_ssl_conf_authmode(_ssl_conf, MBEDTLS_SSL_VERIFY_NONE);
    
    mbedtls_ssl_conf_verify(_ssl_conf, my_verify, NULL);
    mbedtls_ssl_conf_dbg(_ssl_conf, my_debug, NULL);
    mbedtls_debug_set_threshold(3);

    /*  configure the timers for retransmission of lost handshake messages
            1,000 milli-sec minimum time to 6,000 milli-sec maximum delay   */  
        mbedtls_ssl_conf_handshake_timeout(_ssl_conf, 1000, 6000);
    
    /* Anti-Replay Protection */
        mbedtls_ssl_conf_dtls_anti_replay(_ssl_conf, MBEDTLS_SSL_ANTI_REPLAY_ENABLED);

    /* Bad Mac Limit for 10 Packets */
        mbedtls_ssl_conf_dtls_badmac_limit(_ssl_conf, 10);

    /* Set the limit for the Maximum Transmission Unit for DTLS Payload 
         {1500 Bits Set} */
        mbedtls_ssl_set_mtu( _ssl , 1500 );

    tr_info("mbedtls_ssl_setup()");
    if ((ret = mbedtls_ssl_setup(_ssl, _ssl_conf)) != 0) {
        print_mbedtls_error("mbedtls_ssl_setup", ret);
        _error = ret;
        return _error;
    }
    pc.printf("ok\n");
    
    pc.printf("[+] Configuring the Bio ... ");
    mbedtls_ssl_set_bio(_ssl, this, ssl_send, ssl_recv, NULL );
    pc.printf("ok\n");
    
    if(_client_auth) {
                pc.printf("[+] Configuring Client's pvtKey & certKey ... "); 
        if((ret = mbedtls_ssl_conf_own_cert(_ssl_conf, _clicert, _pkctx)) != 0) {
            print_mbedtls_error("mbedtls_ssl_conf_own_cert", ret);
            _error = ret;
            return _error;
        }
                pc.printf("ok\n");
    } 
    
    pc.printf("[+] Timing Call-Back Setting ... "); 
    mbedtls_ssl_set_timer_cb(_ssl, _ssl_timer, mbedtls_timing_set_delay, mbedtls_timing_get_delay);
    pc.printf("ok\n");

    /* Start the handshake, the rest will be done in onReceive() */ 
    //uint32_t init, final, TCPH, TTPH;
    pc.printf("[+] DTLS Handshake Connecting \n");    
    //init = osKernelGetTickCount(); 
    if ((ret = mbedtls_ssl_handshake(_ssl)) != 0) {
        pc.printf("[-] mbedtls_ssl_handshake error %d\n", ret);
        return ret;
    } 
    else {
        //final = osKernelGetTickCount();
        pc.printf("[+] DTLS Hanshake Connected \n");
    } 
    
    //TCPH = final - init;
    //TTPH = TCPH/osKernelGetTickFreq();
    
    //pc.printf("\nTCPH: %d\n", TCPH);
    //pc.printf("\nTTPH: %d\n", TTPH); 
    //pc.printf("\nTickFreq: %d\n", osKernelGetTickFreq()); 
    
    /* It also means the handshake is done, time to print info */
    tr_info("[+] TLS connection to %s established\r\n", _ssl->hostname);
    // Prints the server certificate and verify it. 
    
    const size_t buf_size = 1024;
    char* buf = new char[buf_size];
    mbedtls_x509_crt_info(buf, buf_size, "\r    ",  mbedtls_ssl_get_peer_cert(_ssl));
    tr_debug("[+] Server certificate:\r\n%s\r\n", buf);

    uint32_t flags = mbedtls_ssl_get_verify_result(_ssl);
    if( flags != 0 ) { 
    //  Verification failed.   
        mbedtls_x509_crt_verify_info(buf, buf_size, "\r  ! ", flags); 
        tr_error("[+] Certificate verification failed:\r\n%s", buf);
    } else { 
        // Verification succeeded.   
        tr_info("[+] Certificate verification passed"); 
    }  
    delete[] buf; 
    _handshake_completed = true;

    return 0;
}

nsapi_error_t TLSSocketWrapper::send(const void *data, nsapi_size_t size) {
    int ret;

    if (!is_tls_allocated()) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    tr_debug("send %d", size);
    //uint32_t init, final, TCPH, TTPH;
    //init = osKernelGetTickCount();
    ret = mbedtls_ssl_write(_ssl, (const unsigned char *) data, size);
    //final = osKernelGetTickCount();
        
    /*TCPH = final - init;
    TTPH = TCPH/osKernelGetTickFreq();
    
    pc.printf("\nTCPH: %d\n", TCPH);
    pc.printf("\nTTPH: %d\n", TTPH); 
    pc.printf("\nTickFreq: %d\n", osKernelGetTickFreq()); */

    if (ret == MBEDTLS_ERR_SSL_WANT_WRITE ||
        ret == MBEDTLS_ERR_SSL_WANT_READ) {
        // translate to socket error
        return NSAPI_ERROR_WOULD_BLOCK;
    }
    if (ret < 0) {
        print_mbedtls_error("mbedtls_ssl_write", ret);
    }
    return ret; // Assume "non negative errorcode" to be propagated from Socket layer
}

nsapi_size_or_error_t TLSSocketWrapper::sendto(const SocketAddress &, const void *data, nsapi_size_t size)
{
    // Ignore the SocketAddress
    return send(data, size);
}

nsapi_size_or_error_t TLSSocketWrapper::recv(void *data, nsapi_size_t size) {
    int ret;

    if (!is_tls_allocated()) {
        return NSAPI_ERROR_NO_SOCKET;
    }
    
    ret = mbedtls_ssl_read(_ssl, (unsigned char *) data, size);
    
    if (ret == MBEDTLS_ERR_SSL_WANT_WRITE ||
        ret == MBEDTLS_ERR_SSL_WANT_READ) {
        // translate to socket error
        return NSAPI_ERROR_WOULD_BLOCK;
    } else if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY) {
        /* MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY is not considered as error.
         * Just ignre here. Once connection is closed, mbedtls_ssl_read()
         * will return 0.
         */
        return 0;
    } else if (ret < 0) {
        print_mbedtls_error("mbedtls_ssl_read", ret);
        // TODO: Should I translate SSL errors to some socket error?
    }
    return ret;
}

nsapi_size_or_error_t TLSSocketWrapper::recvfrom(SocketAddress *address, void *data, nsapi_size_t size)
{
    //TODO: Need Socket::getpeername() to get address
    return recv(data, size);
}

void TLSSocketWrapper::print_mbedtls_error(const char *name, int err) {
    char *buf = new char[128];
    mbedtls_strerror(err, buf, 128);
    tr_err("%s() failed: -0x%04x (%d): %s", name, -err, err, buf);
    delete[] buf;
}


#if MBED_CONF_TLS_SOCKET_DEBUG_LEVEL > 0

void TLSSocketWrapper::my_debug(void *ctx, int level, const char *file, int line,
                        const char *str)
{
    const char *p, *basename;
    (void) ctx;

    /* Extract basename from file */
    for(p = basename = file; *p != '\0'; p++) {
        if(*p == '/' || *p == '\\') {
            basename = p + 1;
        }
    }

    tr_debug("%s:%04d: |%d| %s", basename, line, level, str);
}

int TLSSocketWrapper::my_verify(void *data, mbedtls_x509_crt *crt, int depth, uint32_t *flags)
{
    const uint32_t buf_size = 1024;
    char *buf = new char[buf_size];
    (void) data;

    tr_debug("\nVerifying certificate at depth %d:\n", depth);
    mbedtls_x509_crt_info(buf, buf_size - 1, "  ", crt);
    tr_debug("%s", buf);

    if (*flags == 0)
        tr_info("No verification issue for this certificate\n");
    else
    {
        mbedtls_x509_crt_verify_info(buf, buf_size, "  ! ", *flags);
        tr_info("%s\n", buf);
    }

    delete[] buf;

    return 0;
}
#endif /* MBED_CONF_TLS_SOCKET_DEBUG_LEVEL > 0 */


int TLSSocketWrapper::ssl_recv(void *ctx, unsigned char *buf, size_t len) {
    int recv;

    TLSSocketWrapper *my = static_cast<TLSSocketWrapper *>(ctx);

    if (!my->_transport) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    recv = my->_transport->recv(buf, len);

    if (NSAPI_ERROR_WOULD_BLOCK == recv) {
        return MBEDTLS_ERR_SSL_WANT_READ;
    } else if(recv < 0) {
        tr_error("Socket recv error %d", recv);
    }
    // Propagate also Socket errors to SSL, it allows negative error codes to be returned here.
    return recv;
}

int TLSSocketWrapper::ssl_send(void *ctx, const unsigned char *buf, size_t len) {
    int size = -1;
    TLSSocketWrapper *my = static_cast<TLSSocketWrapper *>(ctx);

    if (!my->_transport) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    size = my->_transport->send(buf, len);

    if (NSAPI_ERROR_WOULD_BLOCK == size) {
        return MBEDTLS_ERR_SSL_WANT_WRITE;
    } else if(size < 0){
        tr_error("Socket send error %d", size);
    }
    // Propagate also Socket errors to SSL, it allows negative error codes to be returned here.
    return size;
 }

void TLSSocketWrapper::tls_init() {
    _entropy = new mbedtls_entropy_context;
    _ctr_drbg = new mbedtls_ctr_drbg_context;
    _cacert = new mbedtls_x509_crt;
    _clicert = new mbedtls_x509_crt;
    _pkctx = new mbedtls_pk_context;
    _ssl = new mbedtls_ssl_context;
    _ssl_conf = new mbedtls_ssl_config;
    _ssl_timer = new mbedtls_timing_delay_context;
    
    mbedtls_entropy_init(_entropy);
    mbedtls_ctr_drbg_init(_ctr_drbg);
    mbedtls_x509_crt_init(_cacert);
    mbedtls_x509_crt_init(_clicert);
    mbedtls_pk_init(_pkctx);
    mbedtls_ssl_init(_ssl);
    mbedtls_ssl_config_init(_ssl_conf);    
}

void TLSSocketWrapper::tls_free() {
    mbedtls_entropy_free(_entropy);
    mbedtls_ctr_drbg_free(_ctr_drbg);
    mbedtls_x509_crt_free(_cacert);
    mbedtls_x509_crt_free(_clicert);
    mbedtls_pk_free(_pkctx);
    mbedtls_ssl_free(_ssl);
    mbedtls_ssl_config_free(_ssl_conf);

    delete _entropy;
    delete _ctr_drbg;
    delete _cacert;
    delete _clicert;
    delete _pkctx;
    delete _ssl;
    delete _ssl_conf;
    delete _ssl_timer;
    _ssl = NULL; // Marks that TLS context is freed
}

bool TLSSocketWrapper::is_tls_allocated() {
    return _ssl != NULL;
}

nsapi_error_t TLSSocketWrapper::close()
{
    if (!_transport) {
        return NSAPI_ERROR_NO_SOCKET;
    }
    if (!is_tls_allocated()) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    tr_info("Closing TLS");

    int ret = 0;
    if (_handshake_completed) {
        _transport->set_blocking(true);
        ret = mbedtls_ssl_close_notify(_ssl);
        if (ret) {
            print_mbedtls_error("mbedtls_ssl_close_notify", ret);
        }
        _handshake_completed = false;
    }

    if (!_keep_transport_open) {
        int ret2 = _transport->close();
        if (!ret) {
            ret = ret2;
        }
    }

    _transport = NULL;

    tls_free();

    return ret;
}

nsapi_error_t TLSSocketWrapper::connect(const SocketAddress &address)
{
    pc.printf("Hello from TLSSocketWrapper::connect().\n");
    if (!_transport) {
        pc.printf("transport not available\n");
        return NSAPI_ERROR_NO_SOCKET;
    }
    //TODO: We could initiate the hanshake here, if there would be separate function call to set the target hostname
    nsapi_error_t ret = _transport->connect(address);
    if (ret) {
        pc.printf("transport->connect() failed, %d\n", ret);
        pc.printf("error return from TLSSocketWrapper::connect %d\n", ret);
        return ret;
    } 
    return do_handshake();
}

nsapi_error_t TLSSocketWrapper::bind(const SocketAddress &address)
{
    if (!_transport) {
        return NSAPI_ERROR_NO_SOCKET;
    }
    return _transport->bind(address);
}

void TLSSocketWrapper::set_blocking(bool blocking)
{
    if (!_transport) {
        return;
    }
    _transport->set_blocking(blocking);
}

void TLSSocketWrapper::set_timeout(int timeout)
{
    if (!_transport) {
        return;
    }
    _transport->set_timeout(timeout);
}

void TLSSocketWrapper::sigio(mbed::Callback<void()> func)
{
    if (!_transport) {
        return;
    }
    // Allow sigio() to propagate to upper level and handle errors on recv() and send()
    _transport->sigio(func);
}

nsapi_error_t TLSSocketWrapper::setsockopt(int level, int optname, const void *optval, unsigned optlen)
{
    if (!_transport) {
        return NSAPI_ERROR_NO_SOCKET;
    }
    return _transport->setsockopt(level, optname, optval, optlen);
}

nsapi_error_t TLSSocketWrapper::getsockopt(int level, int optname, void *optval, unsigned *optlen)
{
    if (!_transport) {
        return NSAPI_ERROR_NO_SOCKET;
    }
    return _transport->getsockopt(level, optname, optval, optlen);
}

Socket *TLSSocketWrapper::accept(nsapi_error_t *err)
{
    if (err) {
        *err = NSAPI_ERROR_UNSUPPORTED;
    }
    return NULL;
}

nsapi_error_t TLSSocketWrapper::listen(int)
{
    return NSAPI_ERROR_UNSUPPORTED;
}
