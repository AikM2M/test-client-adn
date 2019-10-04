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

#ifndef _MBED_HTTPS_TLS_TCP_SOCKET_H_
#define _MBED_HTTPS_TLS_TCP_SOCKET_H_

//#include "netsocket/TCPSocket.h"
#include "netsocket/UDPSocket.h"
#include "TLSSocketWrapper.h"

//#include "mbedtls/platform.h"
//#include "mbedtls/ssl.h"
//#include "mbedtls/entropy.h"
//#include "mbedtls/ctr_drbg.h"
//#include "mbedtls/error.h"

/**
 * \brief TLSSocket a wrapper around TCPSocket for interacting with TLS servers
 */
class TLSSocket : public TLSSocketWrapper {
public:

    /** Create an uninitialized socket
     *
     *  Must call open to initialize the socket on a network stack.
     */
    TLSSocket() : TLSSocketWrapper(&udp_socket) {
        
        }

    /** Create a socket on a network interface
     *
     *  Creates and opens a socket on the network stack of the given
     *  network interface.
     *
     *  @param stack    Network stack as target for socket
     */
    template <typename S>
    TLSSocket(S *stack, const char *hostname = NULL) : TLSSocketWrapper(&udp_socket, hostname)
    {
        nsapi_error_t ret;
        if((ret = udp_socket.open(stack)) < 0)
            printf("[.] UDP Socket Opening Failure\n");
        else {
            MBED_ASSERT(ret == NSAPI_ERROR_OK);
            printf("[.] UDP Socket Opening Successful\n");
        }
    }

    /** Opens a socket
     *
     *  Creates a network socket on the network stack of the given
     *  network interface. Not needed if stack is passed to the
     *  socket's constructor.
     *
     *  @param stack    Network stack as target for socket
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t open(NetworkStack *stack) {
        return udp_socket.open(stack);
    }

    template <typename S>
    nsapi_error_t open(S *stack) {
        return open(nsapi_create_stack(stack));
    }

    using TLSSocketWrapper::connect;

    /** Connects TCP socket to a remote host
     *
     *  Initiates a connection to a remote server specified by either
     *  a domain name or an IP address and a port.
     *
     *  @param host     Hostname of the remote host
     *  @param port     Port of the remote host
     *  @return         0 on success, negative error code on failure
     */
    nsapi_error_t connect1(const char *host, uint16_t port);
    nsapi_error_t connect2(const char *host, uint16_t port);
    //SocketAddress getsock();
    SocketAddress sock_addr;
private:
    UDPSocket udp_socket;
};

#endif // _MBED_HTTPS_TLS_TCP_SOCKET_H_