
int contentInstance(MQTTSN::Client<MQTTSNUDP, Countdown> *client, MQTTSNUDP* ipstack,  MQTTSN_topicid& topicid)
{
    //Create Container
    //Mendatory resources: Operation, To, From, Request Identifier ,Resource Type, Content 
    //Resource Specific Attributes [M]: App-ID, requestReachability, supportedReleaseVersions
    //                              [0]: PointofAccess, ResourceName

    //struct Mendatory m1; (to, fr, rqi, ty, op)
    Request R1;
    sprintf(URI,"/%s/%s/%s",CSE_ID, c_aei, CAE.resourceName);
    R1.To = URI; 
    R1.From = aei;
    R1.Request_Identifier = "createCin01";
    R1.Resource_Type = 4;
    R1.Operation = 1;
    
    //struct CreateAE CAE;
    
    CIN.contentInfo = "text/plains:0"; 
    CIN.content = "25";
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
    //wait for response
    return 0;
}
