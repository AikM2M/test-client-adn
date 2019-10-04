#pragma once
#include <string>

using namespace std;

extern int ty, op, cst, rsc, mmg;
extern std::string resourceType, cseType, operation, response;
extern bool RequestReachability;  
extern bool rr, local;   
extern std::string mma, mmt, mgd, DevID, Man, Mod, DevType, FwV, SwV, HwV;
extern std::string ri, pi, ct, et, lt, dmma, dmmt;
extern std::string From, csi, api, poa, to, rqi, aei, rn;
extern char URI[100];
extern char* c_aei;
extern std::string con, cnf;
extern std::string nu;
extern std::string sur;
extern int net, nct;
extern const char* content;

extern void resource_type();
extern void Operation_Type();
extern void Response_Type();
extern void CSE_Type();


struct Resource 
{
    string resourceName;            //rn     1
    int Resource_Type;              //ty     1
    string resourceID;              //ri     1
    string parentID;                //pi     1
    string creationTime;            //ct     1
    string lastModifiedTime;        //lt     1
    string labels;                  //      0-1
};

struct Management
{
    string memoryAvailabe;             //mma    1
    string memoryTotal;                //mmt    1
    string DeviceID;                //DevID 
    string Manufacturer;            //Man
    string Model;                   //Mod
    string DeviceType;              //DevType 
    string FirmwareVersion;          //FwV
    string SoftwareVersion;         //SwV
    string HardwareVersion;         //HwV
};

struct regularResource 
{ 
    //struct RES;
    string accessControlPolicyIDs;                  //
    string expirationTime;                          //et    1
    string dynamicAuthorizationConsultationIDs;     //
};

struct announceableResource 
{
    //struct regRES;                          
    string announceTo;              //
    string announcedAttribute;      //
};

struct announcedResource 
{
    //struct RES;
    string accessControlPolicyIDs;                          //      1
    string expirationTime;                                  //et    1
    string link;                                            //      1
    string dynamicAuthorizationConsultationIDs;             //   0-1
};

struct subordinateResource 
{
    //struct RES;
    string expirationTime;          //et    1
};

struct announceableSubordinateResource 
{
    //struct RES;
    string expirationTime;          //et    1
    string announceTo;              //      0-1
    string announcedAttribute;      //      0-1
};

struct announcedSubordinateResource
{   
    //struct RES;
    string expirationTime;          //et        1
    string link;                    //          1
};

class Response
{
public:
    int responseStatusCode;
    string Request_Identifier;
    string To;
    string From;
    int releaseVersion;
};

class Request{
public:
    int Operation;  
    string To;
    string From;
    string Request_Identifier;
    int Resource_Type;
};

struct CreateAE
{
    string resourceName; 
    string App_ID; 
    string pointOfAccess; 
    bool requestReachability;
};

struct CreateCIN
{
    string resourceName;                //rn
    string contentInfo;                 //cnf 
    string content;                     //con
};

struct CreateSub
{
    string resourceName;                //rn
    string notificationURI;             //nu
    int notificationContentType;        //nct
    int notificationEventType;          //net
};

struct respAE
{
    string App_ID;
    bool requestReachability;
    string AE_ID;
    string pointOfAccess;
};

struct respCnt
{
    int stateTag;
    int CurrentNrOfInstances;
    int CurrentByteSize;
};
struct respCin
{
    string contentInfo;
    int contentSize;
    int stateTag;
    string content;
};

struct respSub
{
    string notificationURI;
    int notificationContentType;
    int notificationEventType;
};

const char* Create_Req(Request Req);

const char* Retrive_Req(Request Req);

const char* Update_Req(Request Req);

const char* Delete_Req(Request Req);
/////////////////////////////////////

const char* Create_Resp(Response Resp);

const char* Notify_Resp(Response Resp);

void process_msg(const char* Buffer);