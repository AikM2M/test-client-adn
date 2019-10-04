#include "sensor.h"
#include "MbedJSONValue.h"

void process_rsc(MbedJSONValue &MSG, MbedJSONValue &MSG_1);

struct Resource RES;
struct Management MGO;
struct regularResource regRES;
struct announceableResource ancRES;
struct announcedResource ancdRES;
struct subordinateResource subRES;
struct announceableSubordinateResource ancsubRES;
struct announcedSubordinateResource ancdsubRES;

struct CreateAE CAE;
struct CreateCIN CIN;
struct CreateSub CSub;
struct respAE RAE;
struct respCnt RCnt;
struct respCin RCin;
struct respSub RSub;

//extern const char* CSE_ID;
extern char* AE_ID;
extern bool lcl;

//////////////////////////////////////////////
//      Func: Create_Req                    //
//      returns a json string               //
//      input parameters Class Request      //
//////////////////////////////////////////////
const char* Create_Req(Request Req)
{
    MbedJSONValue demo, demo2, demo3, demo4;
         
    std::string s;
    switch(Req.Resource_Type){
        case 0:
            break;
        case 1:
            break;
        case 2:
            demo3["api"] = CAE.App_ID;
            demo3["rn"] = CAE.resourceName;
            demo3["rr"] = CAE.requestReachability; 
            demo2["m2m:ae"] = demo3;
        break;
        case 3:
            demo3["rn"] = CAE.resourceName;
            demo2["m2m:cnt"] = demo3;
        break;
        case 4:
            demo3["rn"] = CIN.resourceName;
            demo3["cnf"] = CIN.contentInfo;
            demo3["con"] = CIN.content;
            demo2["m2m:cin"] = demo3;
        break;
        case 23:
            demo3["rn"] = CSub.resourceName;          
            demo4["net"] = CSub.notificationEventType;
            demo3["enc"] = demo4; 
            demo3["nu"] = CSub.notificationURI;             
            demo3["nct"] = CSub.notificationContentType;      
            demo2["m2m:sub"] = demo3;
        break;
    }
    demo["fr"] = Req.From;
    demo["op"] = Req.Operation;
    demo["pc"] = demo2;
    demo["rqi"] = Req.Request_Identifier;
    demo["to"] = Req.To;
    demo["ty"] = Req.Resource_Type;
    //demo["lcl"] = lcl;
    
    //serialize it into a JSON string
    s = demo.serialize();
    printf("\nMSG SIZE: %d\n", s.length());
    printf("Req JSON: %s\r\n", s.c_str());

    return s.c_str();
}
//////////////////////////////////////////////
//      Func: Delete_Req                    //
//      returns a json string               //
//      input parameters Class Request      //
//////////////////////////////////////////////
const char* Delete_Req(Request Req)
{
    MbedJSONValue demo, demo2, demo3, demo4;
         
    std::string s;
   
    demo["fr"] = Req.From;
    demo["op"] = Req.Operation;
    demo["rqi"] = Req.Request_Identifier;
    demo["to"] = Req.To;
    //demo["lcl"] = lcl;
    
    //serialize it into a JSON string
    s = demo.serialize();
    printf("\nMSG SIZE: %d\n", s.length());
    printf("Req JSON: %s\r\n", s.c_str());

    return s.c_str();
}
//////////////////////////////////////////////
//      Func: Retrive_Req                   //
//      returns a json string               //
//      input parameters Class Request      //
//////////////////////////////////////////////
const char* Retrive_Req(Request Req)
{
    MbedJSONValue demo;
         
    std::string s;
   
    demo["fr"] = Req.From;
    demo["op"] = Req.Operation;
    demo["rqi"] = Req.Request_Identifier;
    demo["to"] = Req.To;

    //serialize it into a JSON string
    s = demo.serialize();
    printf("\nMSG SIZE: %d\n", s.length());
    printf("Req JSON: %s\r\n", s.c_str());

    return s.c_str();
}
//////////////////////////////////////////////
//      Func: Update_Req                   //
//      returns a json string               //
//      input parameters Class Request      //
//////////////////////////////////////////////
const char* Update_Req(Request Req)
{
    MbedJSONValue demo, demo2, demo3;
         
    std::string s;
    
    demo["fr"] = Req.From;
    demo["op"] = Req.Operation;
    demo["rqi"] = Req.Request_Identifier;
    demo["to"] = Req.To;
    //demo["lcl"] = lcl;
    switch(RES.Resource_Type){
        case 0:
            demo2["m2m:ae"] = demo3;
        break;
        case 1:
        break;
        case 2:
            demo2["m2m:ae"] = demo3;
        break;
        case 3:
            demo2["m2m:cnt"] = demo3;
        break;
        case 4:
            demo2["m2m:cin"] = demo3;
        break;
        case 23:
            demo2["m2m:sub"] = demo3;
        break;   
    }
    demo["pc"] = demo2;
    //serialize it into a JSON string
    s = demo.serialize();
    printf("\nMSG SIZE: %d\n", s.length());
    printf("Req JSON: %s\r\n", s.c_str());

    return s.c_str();
}
//////////////////////////////////////////////
//      Func: Create_Resp                   //
//      returns a json string               //
//      input parameters Class Response     //
//////////////////////////////////////////////
const char* Create_Resp(Response Resp)
{
    MbedJSONValue demo, demo2, demo3, demo4;
    demo3["ty"] = RES.Resource_Type;
    demo3["ri"] = RES.resourceID;
    demo3["pi"] = RES.parentID;
    demo3["ct"] = RES.creationTime;         //ct     1
    demo3["lt"] = RES.lastModifiedTime;     //lt     1  
    demo3["rn"] = RES.resourceName;
    std::string s;
    printf("RES.Resource_Type = %d",RES.Resource_Type);
    switch(RES.Resource_Type){
        case 0:
            demo3["api"] = RAE.App_ID;
            demo3["rr"] = RAE.requestReachability;
            demo3["aei"] = RAE.AE_ID;
            demo3["poa"] = RAE.pointOfAccess; 
            demo2["m2m:ae"] = demo3;
        break;
        case 1:
        break;
        case 2:
            demo3["api"] = RAE.App_ID;
            demo3["rr"] = RAE.requestReachability;
            demo3["aei"] = RAE.AE_ID;
            demo3["poa"] = RAE.pointOfAccess; 
            demo2["m2m:ae"] = demo3;
        break;
        case 3:
            demo3["st"] = RCnt.stateTag;
            demo3["cni"] = RCnt.CurrentNrOfInstances;
            demo3["cbs"] = RCnt.CurrentByteSize;
            demo2["m2m:cnt"] = demo3;
        break;
        case 4:
            demo3["et"] = ancsubRES.expirationTime;
            demo3["st"] = RCin.stateTag;
            demo3["cnf"] = RCin.contentInfo;
            demo3["cs"] = RCin.contentSize;
            demo3["con"] = RCin.content;
            demo2["m2m:cin"] = demo3;
        break;
        case 13:
            demo["m2m:mgo"] = demo2;
            switch(mmg){
                case 11:
                    demo2["mma"] = MGO.memoryAvailabe;
                    demo2["mmt"] = MGO.memoryTotal;
                break;
                case 33:
                    demo2["FwV"] = MGO.FirmwareVersion; 
                    demo2["SwV"] = MGO.SoftwareVersion;
                break;
                case 44:
                    demo2["Man"] = MGO.Manufacturer;
                    demo2["Mod"] = MGO.Model;
                    demo2["DevType"] = MGO.DeviceType;
                    demo2["HwV"] = MGO.HardwareVersion;
                break;
            }    
        break;
        case 23:
            demo3["et"] = ancsubRES.expirationTime;
            demo4["net"] = RSub.notificationEventType;
            demo3["enc"] = demo4;
            demo3["nct"] = RSub.notificationContentType;
            demo3["nu"] = RSub.notificationURI;
            demo2["m2m:sub"] = demo3;
        break;        
    }
    demo["rsc"] = Resp.responseStatusCode;
    demo["rqi"] = Resp.Request_Identifier;
    demo["pc"] = demo2;
    demo["to"] = Resp.To;
    demo["fr"] = Resp.From;
    //demo["lcl"] = lcl;
    
    //serialize it into a JSON string
    s = demo.serialize();
    printf("\nMSG SIZE: %d\n", s.length());
    printf("RESP JSON: %s\r\n", s.c_str());
    return s.c_str();
}

const char* Notify_Resp(Response Resp)
{
    MbedJSONValue demo;
    std::string s;
    demo["rsc"] = Resp.responseStatusCode;
    demo["rqi"] = Resp.Request_Identifier;
    demo["fr"] = Resp.From;
    demo["to"] = Resp.To;
    //demo["lcl"] = lcl;
    //serialize it into a JSON string
    s = demo.serialize();
    printf("\nMSG SIZE: %d\n", s.length());
    printf("RESP JSON: %s\r\n", s.c_str());
    return s.c_str();
}

void process_msg(const char* Buffer)
{
    MbedJSONValue MSG, MSG_1;

    parse(MSG, Buffer); 

    ////////////////////////Local Parameter (Additional parameter)//////////////////////
    /*if(MSG.hasMember("lcl"))
    {
        local = MSG["lcl"].get<bool>();
        printf("Local: %s\r\n", local? "true" : "false");
    }*/
     
    ////////////////////////resource Type Parameter (Mendatory parameter)//////////////////////
    if(MSG.hasMember("rsc"))
    {
        rsc = MSG["rsc"].get<int>();
        //printf("Response Status Code: %d\r\n", rsc);
        Response_Type();
        printf("Response Status: %s\r\n", response.c_str());
        MSG_1 = MSG["pc"][content];
        process_rsc(MSG, MSG_1);
        return;
    }
    
    ////////////////////////From parameter (Mendatory parameter[optional for Create AE])//////////
    if(MSG.hasMember("fr"))
    {
       From = MSG["fr"].get<std::string>();
       printf("From: %s\r\n", From.c_str());
    }
    else     {
    //add Response Status Code for no mendatory parameter BAD_REQUEST 
    return;  }
    
    ////////////////////////Operation Parameter (Mendatory parameter)//////////////////////
    if(MSG.hasMember("op"))
    {
       op = MSG["op"].get<int>();
       printf("Operation: %d\r\n", op);
       Operation_Type();
       printf("Operation: %s\r\n", operation.c_str());
       /* Process Notification*/
       if(op == 5)
       {
            to = MSG["to"].get<std::string>();
            printf("To: %s\r\n", to.c_str());     
            rqi = MSG["rqi"].get<std::string>();
            printf("Request Identifier: %s\r\n", rqi.c_str());  
            MSG_1 = MSG["pc"]["m2m:sgn"];
            sur = MSG_1["sur"].get<std::string>();
            printf("Subscription Reference: %s\r\n", sur.c_str());
            //MbedJSONValue MSG_2 = MSG["pc"]["m2m:sgn"]["nev"];
            //net = MSG_2["net"].get<int>();
            //printf("NotificationEventType: %d\r\n", net); 
            //MbedJSONValue MSG_3 = MSG["pc"]["m2m:sgn"]["nev"]["rep"]["cin"];
            //con = MSG_3["con"].get<std::string>();
            //printf("Content: %s\r\n", con.c_str());
            //cnf = MSG_3["cnf"].get<std::string>(); 
            //printf("Content Info: %s\r\n", cnf.c_str());
            MSG.~MbedJSONValue();
            MSG_1.~MbedJSONValue();
            //MSG_2.~MbedJSONValue();
            //MSG_3.~MbedJSONValue();
            return;
       }
    }
    else     {
    //add response code for no mendatory parameter
    return;  }
    
    ////////////////////////To Parameter (Mendatory parameter)//////////////////////
    if(MSG.hasMember("to"))
    {
       to = MSG["to"].get<std::string>();
       printf("To: %s\r\n", to.c_str());
    }
    else     {
    //add response code for no mendatory parameter
    return;  } 
         
    ////////////////////////Request Identifier (Mendatory parameter)//////////////////////
    if(MSG.hasMember("rqi"))
    {
       rqi = MSG["rqi"].get<std::string>();
       printf("Request Identifier: %s\r\n", rqi.c_str());
    }
    else     {
    //add response code for no mendatory parameter
    return;   }        
    
    ////////////////////////resource Type Parameter (Mendatory parameter)//////////////////////
    if(MSG.hasMember("ty"))
    {
       ty = MSG["ty"].get<int>();
       printf("ResourceType: %d\r\n", ty);
       resource_type();
       printf("ResourceType: %s\r\n", resourceType.c_str());
    }
    else     {
    //add response code for no mendatory parameter
    return;   }
    
    MSG_1 = MSG["pc"][content]; //content value depends on the resource type e.g. m2m:ae for resource type 2
    
    //////////////////////// Resource specific attributes CSE-ID
    if(MSG_1.hasMember("csi"))
    {
       csi = MSG_1["csi"].get<std::string>();
       printf("CSI: %s\r\n", csi.c_str());
    }
       
    //////////////////////// Resource specific attributes Point of Access
    if(MSG_1.hasMember("poa"))
    {
       poa = MSG_1["poa"].get<std::string>();
       printf("POA: %s\r\n", poa.c_str());
    }  
     
    /////////////////////// Resource specific attributes CSE Type
    if(MSG_1.hasMember("cst"))
    {
       cst = MSG_1["cst"].get<int>();
       printf("CSE Type: %d\r\n", cst);
       CSE_Type();
       printf("CSE Type: %s\r\n", cseType.c_str());
    }
       
    /////////////////////// Resource specific attributes RequestReachability
    if(MSG_1.hasMember("rr"))
    {
       RequestReachability = MSG_1["rr"].get<bool>();
       printf("RR: %s\r\n", RequestReachability? "true" : "false");
    }
       
    /////////////////////// Resource specific attributes App-ID
    if(MSG_1.hasMember("api"))
    {
       api = MSG_1["api"].get<std::string>();
       printf("App-ID: %s\r\n", api.c_str());
    } 
    /////////////////////// Resource specific attributes resourceName
    if(MSG_1.hasMember("rn"))
    {
        rn = MSG_1["rn"].get<std::string>();
        printf("resourceName : %s\r\n", rn.c_str());
    } 
    /////////////////////// Resource specific attributes Content
    if(MSG_1.hasMember("con"))
    {
        con = MSG_1["con"].get<std::string>();
        printf("Content: %s\r\n", con.c_str());
    } 
    /////////////////////// Resource specific attributes ContentInfo
    if(MSG_1.hasMember("cnf"))
    {
        cnf = MSG_1["cnf"].get<std::string>();
        printf("Content Info: %s\r\n", cnf.c_str());
    }  
    /////////////////////// Resource specific attributes NotificationURI nu
    if(MSG_1.hasMember("nu"))
    {
        nu = MSG_1["nu"].get<std::string>();
        printf("Notification URI: %s\r\n", nu.c_str());
    } 
    /////////////////////// Resource specific attributes NotificationContentType
    if(MSG_1.hasMember("nct"))
    {
        nct = MSG_1["nct"].get<int>();
        printf("NotificationContentType: %d\r\n", nct);
        MbedJSONValue &MSG_2 = MSG_1["m2m:sub"]["enc"];
        if(MSG_2.hasMember("net"))
        {
            net = MSG_2["net"].get<int>();  
            printf("NotificationEventType: %d\r\n", net);  
        }
        MSG_2.~MbedJSONValue();
    }   
    
    MSG.~MbedJSONValue();
    MSG_1.~MbedJSONValue();
    return;
}

void process_rsc(MbedJSONValue &MSG, MbedJSONValue &MSG_1)
{
    if(MSG.hasMember("rqi"))
    {
       rqi = MSG["rqi"].get<std::string>();
       printf("Request Identifier in response: %s\r\n", rqi.c_str());
    }
    else     {
    //add response code for no mendatory parameter
    return;   }  
    if(MSG.hasMember("to"))
    {
       to = MSG["to"].get<std::string>();
       printf("To in response: %s\r\n", to.c_str());
    }
    else     {
    //add response code for no mendatory parameter
    return;   } 
    if(MSG.hasMember("fr"))
    {
       From = MSG["fr"].get<std::string>();
       printf("From in response: %s\r\n", From.c_str());
    }
    else     {
    //add response code for no mendatory parameter
    return;   }
    printf("Content is %s \n",content);
    
    if(MSG_1.hasMember("ty"))
    {
        ty = MSG_1["ty"].get<int>();
        printf("ResourceType in response: %d\r\n", ty);
        //resource_type();
        //printf("ResourceType in response: %s\r\n", resourceType.c_str());
    }
    else     {
    //add response code for no mendatory parameter
    return;   }
    
    if(MSG_1.hasMember("rr"))
    {
       RequestReachability = MSG_1["rr"].get<bool>();
       printf("RR: %s\r\n", RequestReachability? "true" : "false");
    } 
    if(MSG_1.hasMember("aei"))
    {
       aei = MSG_1["aei"].get<std::string>();
       printf("AE_ID: %s\r\n", aei.c_str());
    }         
    if(MSG_1.hasMember("poa"))
    {
       poa = MSG_1["poa"].get<std::string>();
       printf("POA: %s\r\n", poa.c_str());
    } 
    if(MSG_1.hasMember("rn"))
    {
       rn = MSG_1["rn"].get<std::string>();
       printf("Resource Name: %s\r\n", rn.c_str());
    }
    if(MSG_1.hasMember("api"))
    {
       api = MSG_1["api"].get<std::string>();
       printf("App-ID: %s\r\n", api.c_str());
    } 
    /////////////////////// Resource specific attributes Content
    if(MSG_1.hasMember("con"))
    {
        con = MSG_1["con"].get<std::string>();
        printf("Content: %s\r\n", con.c_str());
    } 
    /////////////////////// Resource specific attributes ContentInfo
    if(MSG_1.hasMember("cnf"))
    {
        cnf = MSG_1["cnf"].get<std::string>();
        printf("Content Info: %s\r\n", cnf.c_str());
    }  
    
    return;
}
