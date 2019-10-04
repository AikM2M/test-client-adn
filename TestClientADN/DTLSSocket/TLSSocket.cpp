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

#include "TLSSocket.h"

#define TRACE_GROUP "TLSS"
#include "mbed-trace/mbed_trace.h"


/*TLSSocket::SocketAddress getsock() {
    return sock_addr;
}*/


nsapi_error_t TLSSocket::connect1(const char *host, uint16_t port)
{
    pc.printf("[.] Hello from TLSSocket::connect() \n");
    int ret;
    
    sock_addr.set_ip_address(host);
    sock_addr.set_port(port);
    set_hostname(host);
    
    return TLSSocketWrapper::connect(sock_addr);
}

nsapi_error_t TLSSocket::connect2(const char *host, uint16_t port)
{
    pc.printf("[.] Hello from TLSSocket::connect() \n");
    int ret;
    
    sock_addr.set_ip_address(host);
    sock_addr.set_port(port);
    set_hostname(host);
    
    uint32_t init, final, TCPH, TTPH;
    init = osKernelGetTickCount();
    /* Send a 'Client Hello' buffer in order to start a thread on the server */ 
    const char* buffer = "Client Hello";
    if ((ret = udp_socket.sendto(host, port, (const char*) buffer, strlen(buffer))) <= 0)
        pc.printf("Couldn't send 'Client Hello' \n");
    else
        pc.printf("sent 'Client Hello' \n"); 
    wait(1);     
    final = osKernelGetTickCount();
    
    TCPH = final - init;
    TTPH = TCPH/osKernelGetTickFreq();
    
    pc.printf("\nTCPH: %d\n", TCPH);
    pc.printf("\nTTPH: %d\n", TTPH); 
    pc.printf("\nTickFreq: %d\n", osKernelGetTickFreq()); 
    
    return TLSSocketWrapper::connect(sock_addr);
}