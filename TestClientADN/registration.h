
int Registration(MQTTSN::Client<MQTTSNUDP, Countdown> *client, MQTTSNUDP* ipstack,  MQTTSN_topicid& topicid)
{
    //Create AE
    //Mendatory resources: Operation, To, From, Request Identifier ,Resource Type, Content 
    //Resource Specific Attributes [M]: App-ID, requestReachability, supportedReleaseVersions
    //                              [0]: PointofAccess, ResourceName
    Request R1;
    AE_ID = "S";                    //Initial registration
    //struct Mendatory m1;
    sprintf(URI,"/%s",CSE_ID);
    R1.To = URI; 
    R1.From = AE_ID;
    R1.Request_Identifier = "createAE11";
    R1.Resource_Type = 2;
    R1.Operation = 1;
    
    //struct CreateAE CAE;
    CAE.resourceName = MQTT_CLIENT_ID;
    CAE.requestReachability = true;
    CAE.App_ID = "A01.com.sensor011";

    buf = Create_Req(R1);
    printf("string in buf is %s\n",buf);
    //buf[0] = '\0';
    
    
    //PUBLISH

    if ((rc = publish(client, ipstack, topicid)) != 0)
         printf("rc from MQTT Publish is %d\n", rc);
    else
         printf("Published Buffer: %s to Topic %s\n",buf, MQTT_TOPIC);
    
    while(1)
    {
           
        client->yield(4000);
        if(rsc != 0){ 
            //reg_resp = true;
            printf("rsc = %d\n",rsc);
            break;
        }
        else rsc = 0;
    }
    if(rsc == 2001 ){ 
        rsc = 0;
        return 0;
    }
    else if(rsc == 4000){
        rsc = 0;
        return -1;
    }
    //wait for response
    rsc = 0;
    return 0;
}


int retrive_reg(MQTTSN::Client<MQTTSNUDP, Countdown> *client, MQTTSNUDP* ipstack,  MQTTSN_topicid& topicid)
{
    //Create Retrive request
    //op, to fr, rqi
    Request R1;
    //struct Mendatory m1;
    sprintf(URI,"/%s",CSE_ID);
    R1.To = URI; 
    R1.From = aei;
    R1.Request_Identifier = "retAE01";
    R1.Operation = 2;
    buf = Retrive_Req(R1);
    printf("string in buf is %s\n",buf);
    
    //PUBLISH

    if ((rc = publish(client, ipstack, topicid)) != 0)
         printf("rc from MQTT Publish is %d\n", rc);
    else
         printf("Published Buffer: %s to Topic %s\n",buf, MQTT_TOPIC);
    //wait for response
    while(1)
    {           
        client->yield(4000);

        if(rsc != 0){ 
            //reg_resp = true;
            printf("rsc = %d\n",rsc);
            break;
        }
        else rsc = 0;
    }
    if(rsc == 2000 ){ 
        rsc = 0;
        return 0;
    }
    else if(rsc == 4000){
        rsc = 0;
        return -1;
    }
    
    rsc = 0;
    return 0;
}

int update_reg(MQTTSN::Client<MQTTSNUDP, Countdown> *client, MQTTSNUDP* ipstack,  MQTTSN_topicid& topicid)
{
    //Create Retrive request
    //op, to fr, rqi, content
    Request R1;
    //struct Mendatory m1;
    sprintf(URI,"/%s",CSE_ID);
    R1.To = URI;  
    R1.From = AE_ID;
    R1.Request_Identifier = "updateAE11";
    R1.Operation = 3;
    buf = Update_Req(R1);
    printf("string in buf is %s\n",buf);
    
    while(1)
    {           
        client->yield(4000);

        if(rsc != 0){ 
            //reg_resp = true;
            printf("rsc = %d\n",rsc);
            break;
        }
        else rsc = 0;
    }
    if(rsc == 2001 ){ 
        rsc = 0;
        return 0;
    }
    else if(rsc == 2003){
        rsc = 0;
        return -1;
    }
    //wait for response
    rsc = 0;
    return 0;
}