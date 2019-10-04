
int Container(MQTTSN::Client<MQTTSNUDP, Countdown> *client, MQTTSNUDP* ipstack,  MQTTSN_topicid& topicid)
{
    //Create Container
    //Mendatory resources: Operation, To, From, Request Identifier ,Resource Type, Content 
    //Resource Specific Attributes [M]: App-ID, requestReachability, supportedReleaseVersions
    //                              [0]: PointofAccess, ResourceName

    //struct Mendatory m1; (to, fr, rqi, ty, op)
    Request R1;
    //Create to parameter URI cse01/AE01
    sprintf(URI,"/%s/%s",CSE_ID, c_aei);
    R1.To = URI; 
    R1.From = aei;
    R1.Request_Identifier = "createCont01";
    R1.Resource_Type = 3;
    R1.Operation = 1;
    
    //struct CreateAE CAE;
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
    rsc = 0;
    return 0;
}

int delete_cnt (MQTTSN::Client<MQTTSNUDP, Countdown> *client, MQTTSNUDP* ipstack,  MQTTSN_topicid& topicid)
{
    Request R1;
    //Create to parameter URI cse01/AE01/cont_monitor01
    sprintf(URI,"/%s/%s/%s",CSE_ID, c_aei, CAE.resourceName);
    R1.To = URI; 
    R1.From = aei;
    R1.Request_Identifier = "deleteCont01";
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