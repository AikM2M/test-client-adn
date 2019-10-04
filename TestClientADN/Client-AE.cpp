
/*  Initial Flag Settings   
 *  USE_NO_PSK flag to activate certificate mode
 *  USE_NO_CERT flag to activate psk mode
 *  USE_NO_TLS flag to activate DTLS mode
 *  USE_NO_DTLS flag to activate TLS mode   */ 
#define USE_PSK
//#define USE_CERT
#define USE_TLS
//#define USE_NO_DTLS

#define USE_DTLS
//#ifdef USE_DTLS
//#endif
 
#define MQTTSNCLIENT_QOS2 0

#define MBEDTLS_TIMING_ALT
#define MBEDTLS_HAVE_ASM

#define MAF_MAX_PACKET_SIZE         1500
#define MAXIDLength                 40

#include "mbed.h"
#include "mbed_mem_trace.h"
#include "mafClient.h"
#include "MQTTmbed.h"
#include "MQTTSNClient.h"

#include "easy-connect.h"
#include "mbed_mem_trace.h"
//#include "MQTTNetwork.h"

#include "Enumeration.h"
#include "sensor.h"
#include "MQTTSN_func.h"
#include "registration.h"
#include "container.h"
#include "contentInstance.h"
#include "Subscription.h"

#if !defined(MBED_SYS_STATS_ENABLED)
#error[NOT_SUPPORTED] test not supported
#endif

static uint32_t stack_available, stack_total;
static uint32_t heap_available, heap_total;

char h_available[6];
char h_total[6];
char s_available[6];
char s_total[6];

//////////------------- Memory Info Function Begins ----------//////////////

void print_memory_info()
{
    // allocate enough room for every thread's stack statistics
    int cnt = osThreadGetCount();
    mbed_stats_stack_t* stats = (mbed_stats_stack_t*)malloc(cnt * sizeof(mbed_stats_stack_t));

    cnt = mbed_stats_stack_get_each(stats, cnt);
    for (int i = 0; i < cnt; i++) {
        stack_available = stats[i].reserved_size - stats[i].max_size;
        stack_total = stats[i].reserved_size;
        printf("Thread: 0x%lX, Stack size: %lu / %lu\r\n", stats[i].thread_id, stack_available, stack_total);
    }

    free(stats);
    mbed_stats_heap_t heap_stats;
    mbed_stats_heap_get(&heap_stats);
    heap_available = heap_stats.reserved_size - heap_stats.current_size;
    heap_total = heap_stats.reserved_size;
    printf("Heap size: %lu / %lu bytes\r\n", heap_available, heap_total);

    sprintf(h_available, "%lu", heap_available);
    sprintf(h_total, "%lu", heap_total);
    sprintf(s_available, "%lu", stack_available);
    sprintf(s_total, "%lu", stack_total);

    printf("string is %s: ", h_available);
    printf("string is %s: ", h_total);
    printf("string is %s: ", s_available);
    printf("string is %s: ", s_total);
}

NetworkInterface* network = NULL;

int main() {
////////////
    const char* ret;
    const char* rett;
    mbed_stats_sys_t stats;
    mbed_stats_sys_get(&stats);
//////////////////
    // Memory Info. at the start
    int rc;
    float version = 0.47;
    // Memory Info. at the start   
    pc.printf("Opening network interface...\r\n"); 
    network = easy_connect(true);    // If true, prints out connection details.
    if (!network) {
        pc.printf("Unable to open network interface.\r\n");
        return -1; 
    }
    pc.printf("Network interface opened successfully.\r\n");
    pc.printf("\r\n");
    
    const char* Local_IP = network->get_ip_address();
    pc.printf("IP Address of Device is: %s\n", Local_IP);
    const char* Local_MAC = network->get_mac_address();
    pc.printf("MAC Address of Device is: %s\n", Local_MAC); 

    MQTTSNUDP *ipstack = NULL;
    ipstack = new MQTTSNUDP(network);
    
    pc.printf("ADN-Client Initiated \n");
    pc.printf("Connecting to MN-Server via DTLS/UDP %s:%d\n", MQTT_SERVER_HOST_NAME, GwSecPort);
    
    if ((rc = ipstack->connect2(MQTT_SERVER_HOST_NAME, GwSecPort)) != 0) 
         pc.printf("rc from DTLS over UDP connect is %d\n", rc);
    else {
        pc.printf("Connected to MN-Server \n");
        ISADNConnected = true;
    }
    
    MQTTSN::Client<MQTTSNUDP, Countdown> *client = NULL;
    client = new MQTTSN::Client<MQTTSNUDP, Countdown>(*ipstack);
    printf("Connecting to %s:%d\n", MQTT_SERVER_HOST_NAME, GwSecPort);
    //////////////////////////////////MQTT Connect///////////////////////////////////////////
    attemptConnect(client, ipstack);
    
    //////////////////////////////////MQTT Subscribe//////////////////////////////////////////
    
     //Set Topic to /oneM2M/reg_resp/Sensor01/CSE_01
    create_Topic("reg_resp", MQTT_CLIENT_ID, "CSE_01");
    
    MQTTSN_topicid topicid;
    if ((rc = subscribe(client, ipstack, topicid)) != 0)
        printf("rc from MQTT subscribe is %d\n", rc);
    else
        printf("Subscribed to Topic %s\n", MQTT_TOPIC);

    //Set Topic to /oneM2M/reg_req/Sensor01/CSE_01
    create_Topic("reg_req", MQTT_CLIENT_ID, "CSE_01");
    
    MQTTSN_topicid topicid2;    
    if ((rc = subscribe(client, ipstack, topicid2)) != 0)
        printf("rc from MQTT subscribe is %d\n", rc);
    else
        printf("Subscribed to Topic %s\n", MQTT_TOPIC);
    
    //////////////////////////////////Registration///////////////////////////////////////////
    if(Registration(client, ipstack, topicid2) != 0 )
        printf("Registration failed with response: %s\n", response.c_str());
    else printf("Registration Done with response: %s\n", response.c_str());

    //////////////////////////////Req-Resp Topics//////////////////////////////////////////
    c_aei =  new char [aei.length()+1];;
    printf("\nAEI in main: %s\r\n\n\n", aei.c_str());
    strcpy(c_aei, aei.c_str());
    printf("C_AEI: %s\r\n", c_aei);
    
    //Set Topic to /oneM2M/resp/SAE01/CSE_01
    create_Topic("resp", c_aei, "CSE_01");
    
    MQTTSN_topicid topicid3;
    if ((rc = subscribe(client, ipstack, topicid3)) != 0)
         printf("rc from MQTT subscribe is %d\n", rc);
    else
         printf("Subscribed to Topic %s\n", MQTT_TOPIC);
        
    //Set Topic to /oneM2M/req/SAE01/CSE_01
    create_Topic("req", c_aei, "CSE_01");
    
    MQTTSN_topicid topicid4;
    if ((rc = subscribe(client, ipstack, topicid4)) != 0)
         printf("rc from MQTT subscribe is %d\n", rc);
    else
         printf("Subscribed to Topic %s\n", MQTT_TOPIC);
    wait(3);    
    
    //////////////////////////////// Retrive Registration ////////////////////////////////////
    //if(retrive_reg(client, ipstack, topicid4) != 0 )
    //   printf("retrive_reg failed with response: %s\n", response.c_str());
    //else printf("retrive_reg return with response: %s\n", response.c_str());
    //wait(3);
    
    ////////////////////////////////// Container ////////////////////////////////////////////
    CAE.resourceName = "cont_monitor01";
    if(Container(client, ipstack, topicid4) != 0 )
        printf("Container Creation failed with response: %s\n", response.c_str());
    else printf("Container Created with response: %s\n", response.c_str());
    wait(3);
    
    ////////////////////////////////// Subscription ////////////////////////////////////////////
    CSub.resourceName = "sub_monitor01";
    if(Subscription(client, ipstack, topicid4) != 0 )
        printf("Subscription Creation failed with response: %s\n", response.c_str());
    else printf("Subscription Created wth response: %s\n", response.c_str());
    wait(3);
    
    ////////////////////////////////// Subscription ////////////////////////////////////////////
    //CSub.resourceName = "sub_monitor02";
    //if(Subscription(client, ipstack, topicid4) != 0 )
    //    printf("Subscription Creation failed with response: %s\n", response.c_str());
    //else printf("Subscription Created wth response: %s\n", response.c_str());
    //wait(3);
    
    ////////////////////////////////// ContentInstance //////////////////////////////////////
    CIN.resourceName = "cin_monitor01";
    if(contentInstance(client, ipstack, topicid4) != 0 )
        printf("ContentInstance Creation failed with response: %s\n", response.c_str());
    else printf("ContentInstance Created with response: %s\n", response.c_str());
    wait(3);
    
    //CSub.resourceName = "sub_monitor02";
    //rc = delete_sub(client, ipstack, topicid4);
    //wait(3);
    
    //CSub.resourceName = "sub_monitor01";
    //rc = delete_sub(client, ipstack, topicid4);
    //wait(3);
    
    //return 0;
    
    
 
    while(1) 
    {
        // Check connection //
        if(!client->isConnected()){
            break;
        }
        // Received a control message. //
        if(isMessageArrived) {
            isMessageArrived = false;
            // Just print it out here.
            printf("\r\nMessage arrived:\r\n%s\r\n", messageBuffer);
            //generate notification response
            if(op == 5)
            {
                Response Resp;
                RES.resourceName = rn;
                Resp.Request_Identifier = rqi;
                Resp.To = From;
                lcl = false;
                Resp.From = c_aei;
                Resp.responseStatusCode = 2000;
                buf = Notify_Resp(Resp);
                //Set Topic to /oneM2M/req/SAE01/CSE_01
                create_Topic("req", c_aei, "CSE_01");
                if ((rc = publish(client, ipstack, topicid4)) != 0)
                    printf("rc from MQTT Publish is %d\n", rc);
                else
                    printf("Published Buffer: %s to Topic %s\n",buf, MQTT_TOPIC);
                break;
            }
            else{
                ///////////////////////////////////
                ret = strstr((const char*)to.c_str(), "Memory01");
                if (ret != NULL) {
                    printf("res:%s\n", ret);
                    mmg = 11;
                    ty = 13;
                }
                /////------------------- Restart
                ret = strstr((const char*)to.c_str(), "Reboot01");
                if (ret != NULL) {
                    printf("res:%s\n", ret);
                    mmg = 22;
                }
                /////------------------- Software
                ret = strstr((const char*)to.c_str(), "Software01");
                if (ret != NULL) {
                    printf("res:%s\n", ret);
                    mmg = 33;
                    ty = 13;
                }
                ////------------------- Hardware
                ret = strstr((const char*)to.c_str(), "Hardware01");
                if (ret != NULL) {
                    printf("res:%s\n", ret);
                    mmg = 44;
                    ty = 13;
                }
                printf("To: %s\r\n", to.c_str());
                
                Response Resp;
                
                RES.Resource_Type = ty;
                //RES.Operation = op;
                create_Topic("resp", c_aei, "CSE_01");
                switch (mmg) {
                case 11:
                    MGO.memoryAvailabe = s_available;
                    MGO.memoryTotal = s_total;
                    ///MGO.Request_Identifier = "1235";
                    break;
                case 22:
                    NVIC_SystemReset();
                    break;
                case 33:    
                    MGO.FirmwareVersion = "5.12";
                    MGO.SoftwareVersion = "MbedOS";
                    RES.resourceName = "ABC789";
                    ///MGO.Request_Identifier = "1235";
                    break;
                case 44:
                    if (stats.compiler_id == 1) {
                        MGO.HardwareVersion = "ARM";
                    }
                    MGO.Manufacturer = "STM32";
                    MGO.Model = "L476RG";
                    MGO.DeviceType = "Nucleo";
                    RES.resourceName = "ABC789";
                    ////RES.Request_Identifier = "1235";
                    break;
                    default:
                        printf("Something's Wrong\n");
                    break;
                }
                Resp.responseStatusCode = 2000;
                buf = Create_Resp(Resp);
                if ((rc = publish(client, ipstack, topicid)) != 0)
                    printf("rc from MQTT Publish is %d\n", rc);
                else
                    printf("Published Buffer: %s to Topic %s\n", buf, MQTT_TOPIC);
                //////////////////////////////////
            }
        }          
            client->yield(4000);
            //wait(4);
    } 
    
    //if ((rc = client->unsubscribe(topicid2)) != 0)
    //    printf("rc from unsubscribe was %d\n", rc);
    
    //if ((rc = client->disconnect()) != 0)
    rc = client->disconnect(); 
    printf("rc from disconnect was %d\n", rc);
 
    ipstack->disconnect();
    
    delete ipstack;
    delete client;
    
    printf("Version %.2f: finish %d msgs\n", version, arrivedcount);
    printf("Finishing with %d messages received\n", arrivedcount);
    
    return 0;

}
