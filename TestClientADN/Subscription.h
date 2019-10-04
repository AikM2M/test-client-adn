
int Subscription(MQTTSN::Client<MQTTSNUDP, Countdown> *client, MQTTSNUDP* ipstack,  MQTTSN_topicid& topicid)
{
    //Create Subscription
    //Mendatory resources: Operation, To, From, Request Identifier ,Resource Type, Content 
    //Resource Specific Attributes [M]: 
    //                              [0]: 

    //struct Mendatory m1; (to, fr, rqi, ty, op)
    Request R1;
    sprintf(URI,"/%s/%s/%s",CSE_ID, c_aei, CAE.resourceName);
    R1.To = URI; ///TODO: Container resource to subscribe to. 
    char *str_tok;
    str_tok = strtok (URI,"/");
    while (str_tok!= NULL)
    {
        printf ("%s\n",str_tok);
        str_tok = strtok (NULL, "/");
    }
    R1.From = aei;
    R1.Request_Identifier = "createSub01";
    R1.Resource_Type = 23;
    R1.Operation = 1;
    
    //struct CreateSub CSub;
    CSub.notificationEventType = 3;
    //CSub.resourceName = "sub_monitor01";;
    CSub.notificationURI = aei; 
    CSub.notificationContentType = 1;
    buf = Create_Req(R1);
        
    //PUBLISH
    if ((rc = publish(client, ipstack, topicid)) != 0)
         printf("rc from MQTT Publish is %d\n", rc);
    else
         printf("Published Buffer: %s to Topic %s\n",buf, MQTT_TOPIC);

    while(1)
    {  
        client->yield(4000);
        if(rsc != 0){ 
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
    rsc = 0;
    return 0;
}

int delete_sub (MQTTSN::Client<MQTTSNUDP, Countdown> *client, MQTTSNUDP* ipstack,  MQTTSN_topicid& topicid)
{
    Request R1;
    //Create to parameter URI cse01/AE01/cont_monitor01/sub_monitor01
    sprintf(URI,"/%s/%s/%s/%s",CSE_ID, c_aei, CAE.resourceName,CSub.resourceName);
    R1.To = URI; 
    R1.From = aei;
    R1.Request_Identifier = "deleteSub01";
    R1.Operation = 4;
    buf = Delete_Req(R1);
    //PUBLISH
    if ((rc = publish(client, ipstack, topicid)) != 0)
         printf("rc from MQTT Publish is %d\n", rc);
    else
         printf("Published Buffer: %s to Topic %s\n",buf, MQTT_TOPIC);
    while(1)
    {
        client->yield(4000);
        if(rsc != 0){ 
            printf("rsc = %d\n",rsc);
            break;
        }
        else rsc = 0;
    }
    if(rsc == 2003 ){ 
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