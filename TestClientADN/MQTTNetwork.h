#ifndef _MQTTNETWORK_H_
#define _MQTTNETWORK_H_

#include "NetworkInterface.h"
#include "MQTT_server_setting.h"
#include "TLSSocket.h"

class MQTTNetwork {
public:
    MQTTNetwork(NetworkInterface* aNetwork) : network(aNetwork) {
        //printf("Starting the MQTTNetwork(network) constructor\n");
        socket = new TLSSocket(aNetwork, NULL);
    }

    ~MQTTNetwork() {
        delete socket;
    }

    int read(unsigned char* buffer, int len, int timeout) {
        nsapi_size_or_error_t rc = 0;
        socket->set_timeout(timeout);
        rc = socket->recv(buffer, len);
        if (rc == NSAPI_ERROR_WOULD_BLOCK){
            // time out and no data
            // MQTTClient.readPacket() requires 0 on time out and no data.
            return 0;
        }
        return rc;
    }

    int write(unsigned char* buffer, int len, int timeout) {
        // TODO: handle time out
        return socket->send(buffer, len);
    }

    int connect1(const char* hostname, uint16_t port) {   
        //printf("Hello from the MQTT::Network().\n");     
        int ret;
        #ifndef USE_NO_CERT   
        if((ret = socket->set_root_ca_cert(Gateway_rootCA)) != 0)
            printf("Root-CA Setting ... failed\n");
        if((ret = socket->set_client_cert_key(mbed_certKey, mbed_privateKey)) != 0)
            printf("Cert-Key and Pvt-Key Setting ... fialed\n");  
#endif
#ifndef USE_NO_PSK
        if((ret = socket->set_client1_kpsa_kpsaID_cipher()) != 0) { 
            printf("[-] Psk & Psk-ID setting ... failed\n"); 
            return -1;
        }
        printf("[+] Psk & Psk-ID setting ... ok\n"); 
#endif
        if((ret = socket->connect1(hostname, port)) != 0) {
            //printf("MQTTNetwork::connect failed\n"); 
            return -1;
        }
        
        return 0; 
    } 
    
    int connect2(const char* hostname, uint16_t port) {   
        //printf("Hello from the MQTT::Network().\n");     
        int ret;
        #ifndef USE_NO_CERT   
        if((ret = socket->set_root_ca_cert(Gateway_rootCA)) != 0)
            printf("Root-CA Setting ... failed\n");
        if((ret = socket->set_client_cert_key(mbed_certKey, mbed_privateKey)) != 0)
            printf("Cert-Key and Pvt-Key Setting ... fialed\n");  
#endif
#ifndef USE_NO_PSK
        if((ret = socket->set_client2_kpsa_kpsaID_cipher()) != 0) { 
            printf("[-] Psk & Psk-ID setting ... failed\n"); 
            return -1;
        }
        printf("[+] Psk & Psk-ID setting ... ok\n"); 
#endif
        if((ret = socket->connect2(hostname, port)) != 0) {
            //printf("MQTTNetwork::connect failed\n"); 
            return -1;
        }
        
        return 0; 
    } 
    int disconnect() {
        return socket->close();
    }

private:
    NetworkInterface* network;
    TLSSocket *socket;
};

#endif // _MQTTNETWORK_H_
