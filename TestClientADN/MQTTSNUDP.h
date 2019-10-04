#ifndef _MQTTSNUDP_H_
#define _MQTTSNUDP_H_

#include "MQTTNetwork.h"

class MQTTSNUDP {
public:
    MQTTSNUDP(NetworkInterface* aNetwork) : network(aNetwork) {
        printf("starting the MQTTSNUDP(network) constructor\n");
        mqttnetwork = new MQTTNetwork(aNetwork);
    }

    ~MQTTSNUDP() {
        delete mqttnetwork;
    }

    int read(unsigned char* buffer, int len, int timeout) {
        /*nsapi_size_or_error_t rc = 0;
        socket->set_timeout(timeout);
        rc = socket->recv(buffer, len);
        if (rc == NSAPI_ERROR_WOULD_BLOCK){
            // time out and no data
            // MQTTClient.readPacket() requires 0 on time out and no data.
            return 0;
        }
        return rc; */
        return mqttnetwork->read(buffer, len, timeout);
    }

    int write(unsigned char* buffer, int len, int timeout) {
        // TODO: handle time out
        // return socket->send(buffer, len);
        return mqttnetwork->write(buffer, len, timeout);
    }

    int connect1(const char* hostname, int port) {        
        int ret;
        printf("Starting MQTTSNUDP::connect()\n");
        if ((ret = mqttnetwork->connect1(hostname, port)) != 0) {
            printf("MQTTSNUDP::connect() failed\n");
            return -1;
        }
        
        printf("MQTTSNUDP::connect() ok\n");
        return ret;
    }
    
    int connect2(const char* hostname, int port) {        
        int ret;
        printf("Starting MQTTSNUDP::connect()\n");
        if ((ret = mqttnetwork->connect2(hostname, port)) != 0) {
            printf("MQTTSNUDP::connect() failed\n");
            return -1;
        }
        
        printf("MQTTSNUDP::connect() ok\n");
        return ret;
    }

    int disconnect() {
        return mqttnetwork->disconnect();
    }

private:
    NetworkInterface* network;
    //DTLSSocket* socket;
    MQTTNetwork *mqttnetwork;
};

#endif // _MQTTSNUDP_H_
