int connack_rc = 0;
int retryAttempt = 0;
int connectTimeout = 1000;
int rc = 0;

enum connack_return_codes
{
    MQTTSN_CONNECTION_ACCEPTED = 0,
    MQTTSN_REJECTED_CONGESTION = 1,
    MQTTSN_REJECTED_INVALID_TOPIC_ID = 2,
    MQTTSN_REJECTED_NOT_SUPPORTED= 3,
};

static volatile bool isMessageArrived = false;
int arrivedcount = 0;

int connect(MQTTSN::Client<MQTTSNUDP, Countdown> *client, MQTTSNUDP* ipstack)
{ 
    int rc;
    MQTTSNPacket_connectData data = MQTTSNPacket_connectData_initializer;       
    data.clientID.cstring = MQTT_CLIENT_ID;
    data.duration = 600;
    if ((rc = client->connect(data)) == 0) {
        printf ("--->MQTT-SN Connected\n\r");
    }
    else {
        printf("MQTT-SN connect returned %d\n", rc);        
    }
    if (rc >= 0)
        connack_rc = rc;
    return rc;
}

int getConnTimeout(int attemptNumber)
{  // First 10 attempts try within 3 seconds, next 10 attempts retry after every 1 minute
   // after 20 attempts, retry every 10 minutes
    return (attemptNumber < 10) ? 3 : (attemptNumber < 20) ? 60 : 600;
}

void attemptConnect(MQTTSN::Client<MQTTSNUDP, Countdown> *client, MQTTSNUDP* ipstack)
{          
    while (connect(client, ipstack) != MQTTSN_CONNECTION_ACCEPTED) 
    {    
        int timeout = getConnTimeout(++retryAttempt);
        printf("Retry attempt number %d waiting %d\n", retryAttempt, timeout);
        
        // if ipstack and client were on the heap we could deconstruct and goto a label where they are constructed
        //  or maybe just add the proper members to do this disconnect and call attemptConnect(...)        
        // this works - reset the system when the retry count gets to a threshold
        if (retryAttempt == 5)
            NVIC_SystemReset();
        else
            wait(timeout);
    }
}

void messageArrived(MQTTSN::MessageData& md)
{
    MQTTSN::Message &message = md.message;
    printf("Message arrived: qos %d, retained %d, dup %d, packetid %d\n, Number: %d", message.qos, message.retained, message.dup, message.id, arrivedcount);
    memcpy(messageBuffer, (char*)message.payload, message.payloadlen);
    messageBuffer[message.payloadlen] = '\0';
    printf("Payload %.*s\n", message.payloadlen, (char*)message.payload);
    printf("Payload length %d\n",message.payloadlen);
    ++arrivedcount;
    process_msg(messageBuffer);
    isMessageArrived = true;
}

int subscribe(MQTTSN::Client<MQTTSNUDP, Countdown> *client, MQTTSNUDP* ipstack, MQTTSN_topicid& topicid)
{  
    //MQTTSN_topicid topicid;
    topicid.type = MQTTSN_TOPIC_TYPE_NORMAL;
    topicid.data.long_.name = MQTT_TOPIC;
    topicid.data.long_.len = strlen(MQTT_TOPIC);
    MQTTSN::QoS grantedQoS;
    return client->subscribe(topicid, MQTTSN::QOS1, grantedQoS, messageArrived);
}

int publish(MQTTSN::Client<MQTTSNUDP, Countdown> *client, MQTTSNUDP* ipstack, MQTTSN_topicid& topicid)
{
    /*int rc;
    if(PUB_REG == false)
    {
        if ((rc = subscribe(client, ipstack, topicid2)) != 0)
         printf("rc from MQTT subscribe is %d\n", rc);
    else{
         printf("Subscribed to Topic %s\n", MQTT_TOPIC);
         PUB_REG = true;
         }
    }*/

    MQTTSN::Message message;
    message.qos = MQTTSN::QOS1;
    message.retained = false;
    message.dup = false;
    message.payload = (void*)buf;
    message.payloadlen = strlen(buf)+1;
    return client->publish(topicid, message);
}

void create_Topic(char* tpc_ty, char* orignator, char* receiver)
{
    sprintf(MQTT_TOPIC,"/oneM2M/%s/%s/%s",tpc_ty,orignator,receiver);
    printf("Topic is %s", MQTT_TOPIC);
    return;
}
