#ifndef __MQTT_SERVER_SETTING_H__
#define __MQTT_SERVER_SETTING_H__

#define MQTT_MAX_PAYLOAD_SIZE 700 
#define MQTT_CLIENT_ID "SensorNode"
 
//char *buf;
const char * ssid = "KICS-IOT-DHCP";
const char * seckey = "AAAFFFBBBC";  
bool PUB_REG = false;
/* Buffer size for a receiving message. */
const int MESSAGE_BUFFER_SIZE = 512;
/* Buffer for a receiving message. */
char messageBuffer[MESSAGE_BUFFER_SIZE];

const char* buf;

char *MQTT_SERVER_HOST_NAME = "192.168.0.61";
char *MAF_SERVER_HOST_NAME = "192.168.0.61";
int MAF_PORT = 6666; 
int PORT = 1000;
int GwSecPort = 6667;

char MQTT_TOPIC[40];
//char* MQTT_TOPIC = "TEST";

short MQTT_Atmpt = 0;
//const unsigned char* kpsa = "123";
//const unsigned char* kpsaID = "bridge";

// MQTT-SN Gateway DTLS Secure Credentials
/* Eclipse Root-CA */
char Gateway_rootCA[] =  
"-----BEGIN CERTIFICATE-----\n"
"MIIBiDCCAS+gAwIBAgIUV65SNZmLEFReJCpxC+I4JqUpWVgwCgYIKoZIzj0EAwIw\n"
"GjEYMBYGA1UEAwwPbXRydXN0ZWRfY2EuY29tMB4XDTE5MDIxMDA3MzY1NFoXDTI5\n"
"MDIwNzA3MzY1NFowGjEYMBYGA1UEAwwPbXRydXN0ZWRfY2EuY29tMFkwEwYHKoZI\n"
"zj0CAQYIKoZIzj0DAQcDQgAEY/bBYut1Gs+UwNmv4OL9w/0Gv3sMoos/fGoRt2jn\n"
"8ubq/QhAg/NFMm/pXMRzYyJjRZDvThiRPaZHox6U40bFlqNTMFEwHQYDVR0OBBYE\n"
"FLutiux/Qxsax5O1ST3DOssFcfbQMB8GA1UdIwQYMBaAFLutiux/Qxsax5O1ST3D\n"
"OssFcfbQMA8GA1UdEwEB/wQFMAMBAf8wCgYIKoZIzj0EAwIDRwAwRAIgGXZI7HXi\n"
"V21jK42IFBaJfqKdRdGSPx2JxrPK8KDNe3ECIBGhyY1ks+lYiCTP0f5F7Ogmg6Ep\n"
"tcXc/XBbpyz7wAr/\n"
"-----END CERTIFICATE-----\n";

// ASN_AE.crt ECDSA Certificate [my.m2mprovider.com/asn-ae]
char mbed_certKey[] = 
"-----BEGIN CERTIFICATE-----\n"
"MIIBOTCB3wIUEr9b48sXFwyYx6Yn/E/kdMmf5vwwCgYIKoZIzj0EAwIwGjEYMBYG\n"
"A1UEAwwPbXRydXN0ZWRfY2EuY29tMB4XDTE5MDIxMDA3NTYxN1oXDTI5MDIwNzA3\n"
"NTYxN1owJDEiMCAGA1UEAwwZbXkubTJtcHJvdmlkZXIub3JnL2Fzbi1hZTBZMBMG\n"
"ByqGSM49AgEGCCqGSM49AwEHA0IABDN9s2LsnKizcBkD/Gjnk6vQBVVJxGB72Quj\n"
"aZCfrruTbyDx05D3sZscRxdEpIFDrJgzZJHIIODtYWg94b+kVAEwCgYIKoZIzj0E\n"
"AwIDSQAwRgIhALQUFeMRzwJb+xARhsRlcblsYInE+pI8/GvwkzesYOSSAiEA3UKj\n"
"JVxikaiKvnXD9Cu9uHzXlVOF4wK87RXKB1yYB2A=\n"
"-----END CERTIFICATE-----\n";

// ASN_AE.key ECDSA Private Key [ASN_AE]
char mbed_privateKey[] =
"-----BEGIN EC PARAMETERS-----\n"
"BggqhkjOPQMBBw==\n"
"-----END EC PARAMETERS-----\n"
"-----BEGIN EC PRIVATE KEY-----\n"
"MHcCAQEEIM0pVl+8oEI9OOP5A8hfSU0MiaKNQmwqkRW0ZScGshU+oAoGCCqGSM49\n"
"AwEHoUQDQgAEM32zYuycqLNwGQP8aOeTq9AFVUnEYHvZC6NpkJ+uu5NvIPHTkPex\n"
"mxxHF0SkgUOsmDNkkcgg4O1haD3hv6RUAQ==\n"
"-----END EC PRIVATE KEY-----\n";

#endif /* __MQTT_SERVER_SETTING_H__ */