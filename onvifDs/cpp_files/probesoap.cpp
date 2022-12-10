#include <set>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <ctime>
#include "../h_files/probesoap.h"
#include "../h_files/utils.h"
#include <vector>

using namespace std;

void GetSoap::GenSoap(Xml_element* element)
{
  soap += "<";
  if (element->ns != 0)
  {
    soap += element->ns;
    soap += ":";
  }
  soap += element->name;
  if((element->xmlns_ns!=0) && (element->xmlns_value != 0))
  {
    GenXmlns(element->xmlns_ns, element->xmlns_value);
  }
  if(strlen(element->type) != 0)
  {
    soap += " Type=\"";
    soap += element->type;
    soap += "\"";
  }
  if(element->self == 0)
  {
    soap += ">";
    if(element->value != 0)
    {
      soap += element->value;
    }
    if(element->child != 0)
    {
      GenSoap(element->child);
    }
    soap += "</";
    if(element->ns != 0)
    {
      soap += element->ns;
      soap +=":";
    }
    soap += element->name;
    soap += ">";    
  }
  else
  {
    soap += " />";
  }
  if(element->brother != 0)
  {
    GenSoap(element->brother);
  }
}
void GetSoap::GenXmlns(Xmlns_ns* ns, Xmlns_value* value)
{
  if(ns != 0)
  {
    soap += " xmlns";
    if(ns->value != 0)
    {
      soap += ":";
      soap += ns->value;
    }
    if(value != 0)
    {
      soap += "=\"";
      soap += value->value;
      soap += "\"";
    }
  }
  if((ns->next != 0) && value->next!=0)
  {
    return GenXmlns(ns->next, value->next);
  }
}


void ProbeDecorator::Init()
{
/*
创建HTTP头和HTTPcontent
并将地址交付给message
其中Header为0
*/

    http_content.xml_info = &xml_info;
    http_content.envelope = &envelope;
    message.http_content = &http_content;
    memcpy(xml_info.version,"1.0",3);
    memcpy(xml_info.encoding,"utf-8",5);
  /*
<?xml version="1.0" encoding="utf-8"?><Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns="http://www.w3.org/2003/05/soap-envelope">
这里定义的是如上所示的标签，都先创建需要的结构体其次，构建标签的结构，最后赋值
*/

  xmlns_ns_xsi.next = &xmlns_ns_xsd;
  xmlns_ns_xsd.next = &xmlns_ns_non;
  
  xmlns_value_xsi.next = &xmlns_value_xsd;
  xmlns_value_xsd.next = &xmlns_value_non;
  //envelope.name = new char[29];
  //memset(envelope.name,0,29);
  memcpy(envelope.name,"Envelope",9);
  envelope.xmlns_ns = &xmlns_ns_xsi;
  envelope.xmlns_value = &xmlns_value_xsi;
  memcpy(xmlns_ns_xsi.value, "xsi", 3);
  memcpy(xmlns_ns_xsd.value,"xsd",3);
  memcpy(xmlns_value_xsi.value, "http://www.w3.org/2001/XMLSchema-instance", 41);
  memcpy(xmlns_value_xsd.value, "http://www.w3.org/2001/XMLSchema", 32);
  memcpy(xmlns_value_non.value, "http://www.w3.org/2003/05/soap-envelope", 39);
/*
xml结构定义
ns,name,xmlns_ns,xmlns_value,value
*/
  envelope.child = &xml_header;
  xml_header.child = &xml_header_messageid;
  xml_header.brother = &xml_body;
  xml_header_messageid.brother = &xml_header_to;
  xml_header_to.brother = &xml_header_action;
  xml_body.child = &xml_body_probe;
  xml_body_probe.child = &xml_body_probe_types;
  xml_body_probe_types.brother = &xml_body_probe_scopes;

  /*
  
  创建所有需要用到的Xmlns_ns和Xmlns_value
  xmlns_ns_to_wsa表示to标签的名为wsa的xmlns命名空间
  */ 
  
  //将xml header的xmlns_ns和xmlns_value的首地址赋值
  xml_header_messageid.xmlns_ns = &xmlns_ns_messageid_wsa;
  xml_header_messageid.xmlns_value = &xmlns_value_messageid_wsa;
  xml_header_to.xmlns_ns = &xmlns_ns_to_wsa;
  xml_header_to.xmlns_value = &xmlns_value_to_wsa;
  xml_header_action.xmlns_ns = &xmlns_ns_action_wsa;
  xml_header_action.xmlns_value = &xmlns_value_action_wsa;
  //xml header
  //xml body
  xml_body_probe.xmlns_ns = &xmlns_ns_probe_xsi;
  xml_body_probe.xmlns_value = &xmlns_value_probe_xsi;
  xmlns_ns_probe_xsi.next = &xmlns_ns_probe_xsd;
  xmlns_ns_probe_xsd.next = &xmlns_ns_probe_non;
  xmlns_value_probe_xsi.next = &xmlns_value_probe_xsd;
  xmlns_value_probe_xsd.next = &xmlns_value_probe_non;

  //-----------结构定义结束-----------
  // 
  // 
  //-----------开始赋值--------------- 
  memcpy(xml_header.name,"Header", 6);
  memcpy(xml_header_messageid.ns,"wsa",3);
  memcpy(xml_header_messageid.name,"MessageID", 9);
  char* s = (char*)"uuid:a3ffa768-7650-41c5-8464-479239d8c80c";
  memcpy(xml_header_messageid.value,s, strlen(s));
  memcpy(xmlns_ns_messageid_wsa.value, "wsa", 3);
  s = (char*)"http://schemas.xmlsoap.org/ws/2004/08/addressing";
  memcpy(xmlns_value_messageid_wsa.value,s,strlen(s));
  memcpy(xml_header_to.ns,"wsa",3);
  memcpy(xml_header_to.name,"To",2);
  memcpy(xmlns_ns_to_wsa.value,"wsa",3);
  s = (char*)"http://schemas.xmlsoap.org/ws/2004/08/addressing";
  memcpy(xmlns_value_to_wsa.value,s,strlen(s));
  s = (char*)"urn:schemas-xmlsoap-org:ws:2005:04:discovery";
  memcpy(xml_header_to.value,s,strlen(s));
  memcpy(xml_header_action.ns,"wsa",3);
  memcpy(xml_header_action.name,"Action",6);
  memcpy(xmlns_ns_action_wsa.value,"wsa",3);
  s = (char*)"http://schemas.xmlsoap.org/ws/2004/08/addressing";
  memcpy(xmlns_value_action_wsa.value,s,strlen(s));
  s = (char*)"http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe";
  memcpy(xml_header_action.value,s,strlen(s));
  memcpy(xml_body.name,"Body", 4);
  memcpy(xml_body_probe.name,"Probe",5);
  memcpy(xmlns_ns_probe_xsi.value,"xsi",3);
  s = (char*)"http://www.w3.org/2001/XMLSchema-instance";
  memcpy(xmlns_value_probe_xsi.value,s,strlen(s));
  memcpy(xmlns_ns_probe_xsd.value,"xsd",3);
  s = (char*)"http://www.w3.org/2001/XMLSchema";
  memcpy(xmlns_value_probe_xsd.value,s,strlen(s));
  s = (char*)"http://schemas.xmlsoap.org/ws/2005/04/discovery";
  memcpy(xmlns_value_probe_non.value,s,strlen(s));
  memcpy(xml_body_probe_types.name,"Types",5);
  memcpy(xml_body_probe_scopes.name,"Scopes",6);
  s = (char*)"onvif://www.onvif.org/hardware onvif://www.onvif.org/name";
  memcpy(xml_body_probe_scopes.value,s,strlen(s));
  soap += "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
  GenSoap(this->message.http_content->envelope);
}

/*
根据message结构和内容生成soap消息
字符串形式。
*/


void GetServicesSoap::Init(char* hostname)
{
  this->message.http_header = &http_header;
  memcpy(http_header.method, (char*)"POST", 4);
  memcpy(http_header.service_address, (char*)"/onvif/device_service",21);
  memcpy(http_header.http_version, (char*)"HTTP/1.1", 8);
  memcpy(http_header.hostname, hostname, strlen(hostname));
  memcpy(http_header.content_type,(char*)"application/soap+xml; charset=utf-8", 35);
  http_content.xml_info = &xml_info;
  http_content.envelope = &envelope;
  this->message.http_content = &http_content;
  memcpy(xml_info.version,"1.0",3);
  memcpy(xml_info.encoding,"utf-8",5);
  envelope.xmlns_ns = &xmlns_env_ns;
  memcpy(xmlns_env_ns.value,"s", 1);
  envelope.xmlns_value = &xmlns_env_value;
  char* s = (char*)"http://www.w3.org/2003/05/soap-envelope";
  memcpy(xmlns_env_value.value,"http://www.w3.org/2003/05/soap-envelope", strlen(s));
  memcpy(envelope.ns, "s", 1);
  memcpy(envelope.name, "Envelope", 8);
  envelope.child = &xml_body;
  xml_body.child = &xml_body_getservices;
  xml_body_getservices.child = &xml_body_getservices_includecap;
  xml_body.xmlns_ns = &xmlns_body_ns_xsi;
  xml_body.xmlns_value = &xmlns_body_value_xsi;
  xmlns_body_ns_xsi.next = &xmlns_body_ns_xsd;
  xmlns_body_value_xsi.next = &xmlns_body_value_xsd;
  memcpy(xml_body.ns,"s",1);
  memcpy(xml_body.name,"Body", 4);
  memset(xmlns_body_ns_xsi.value, 0,10);
  memcpy(xmlns_body_ns_xsi.value, "xsi", 3);
  memset(xmlns_body_ns_xsd.value, 0, 10);
  memcpy(xmlns_body_ns_xsd.value, "xsd", 3);
  memset(xmlns_body_value_xsi.value,0,50);
  s = (char*)"http://www.w3.org/2001/XMLSchema-instance";
  memcpy(xmlns_body_value_xsi.value,s,strlen(s));
  memset(xmlns_body_value_xsd.value, 0, 50);
  s = (char*)"http://www.w3.org/2001/XMLSchema";
  memcpy(xmlns_body_value_xsd.value, s, strlen(s));
  xml_body_getservices.xmlns_ns = &xmlns_body_getservices_ns;
  xml_body_getservices.xmlns_value = &xmlns_body_getservices_value;
  memcpy(xml_body_getservices.name,"GetServices",11);
  s = (char*)"http://www.onvif.org/ver10/device/wsdl";
  memcpy(xmlns_body_getservices_value.value,s,strlen(s));
  memcpy(xml_body_getservices_includecap.name, "IncludeCapability", 17);
  memcpy(xml_body_getservices_includecap.value, "false", 5);
  soap += "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
  GenSoap(message.http_content->envelope);
}

//getprofilesoap


void GetProfileSoap::Init(char* seradr,char* hostname)
{
  message.http_header = 0;

  http_content.xml_info = &xml_info;
  http_content.envelope = &envelope;
  message.http_content = &http_content;
  memcpy(xml_info.version, "1.0", 3);
  memcpy(xml_info.encoding,"utf-8",5);
  envelope.xmlns_ns = &xmlns_env_ns;
  memcpy(xmlns_env_ns.value,"s", 1);
  xmlns_env_ns.next = 0;
  envelope.xmlns_value = &xmlns_env_value;
  char* s = (char*)"http://www.w3.org/2003/05/soap-envelope";
  memcpy(xmlns_env_value.value,"http://www.w3.org/2003/05/soap-envelope", strlen(s));
  memcpy(envelope.ns, "s", 1);
  memcpy(envelope.name, "Envelope", 8);
  envelope.child = &xml_header;
  memcpy(xml_header.ns, "s", 1);
  memcpy(xml_header.name, "Header", 6);
  xml_header.child = &xml_header_security;
  memcpy(xml_header_security.ns, "wsse", 4);
  memcpy(xml_header_security.name, "Security", 8);
  xml_header_security.child = &xml_header_sec_usernametoken;
  memcpy(xml_header_sec_usernametoken.ns, "wsse", 4);
  memcpy(xml_header_sec_usernametoken.name, "UsernameToken", 13);
  xml_header_sec_usernametoken.child = &xml_header_sec_unt_username;
  memcpy(xml_header_sec_unt_username.ns, "wsse", 4);
  memcpy(xml_header_sec_unt_username.name, "Username", 8);
  memcpy(xml_header_sec_unt_username.value, m_username.c_str(), m_username.length());
  xml_header_sec_unt_username.brother = &xml_header_sec_unt_password;
  memcpy(xml_header_sec_unt_password.ns, "wsse", 4);
  memcpy(xml_header_sec_unt_password.name, "Password", 8);
  s = (char*)"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest";
  memcpy(xml_header_sec_unt_password.type, s, strlen(s));
  Wsse_onvif_ljc wsse;
  wsse.Template(nonce, created, m_passwd.c_str(), password_digest, noncebase64, 1);
  memcpy(xml_header_sec_unt_password.value, password_digest, strlen(password_digest));
  xml_header_sec_unt_password.brother = &xml_header_sec_unt_nonce;
  memcpy(xml_header_sec_unt_nonce.ns, "wsse", 4);
  memcpy(xml_header_sec_unt_nonce.name, "Nonce", 5);
  memcpy(xml_header_sec_unt_nonce.value, noncebase64, strlen(noncebase64));
  xml_header_sec_unt_nonce.brother = &xml_header_sec_unt_created;
  memcpy(xml_header_sec_unt_created.ns, "wsu", 3);
  memcpy(xml_header_sec_unt_created.name, "Created", 7);
  memcpy(xml_header_sec_unt_created.value, created, strlen(created));
  xml_header.brother = &xml_body;
  memcpy(xml_body.ns, "s", 1);
  memcpy(xml_body.name, "Body", 4);
  xml_body.child = &xml_body_getprofiles;
  memcpy(xml_body_getprofiles.name, "GetProfiles", 11);
  xml_body_getprofiles.xmlns_ns = &getprofiles_ns_non;
  xml_body_getprofiles.xmlns_value = &getprofiles_value_non;
  s = (char*)"http://www.onvif.org/ver20/media/wsdl";
  memcpy(getprofiles_value_non.value, s, strlen(s));
  xml_body_getprofiles.child = &xml_body_gpf_type_1;
  memcpy(xml_body_gpf_type_1.name, "Type", 4);
  memcpy(xml_body_gpf_type_1.value, "VideoSource", 11);
  xml_body_gpf_type_1.brother = &xml_body_gpf_type_2;
  memcpy(xml_body_gpf_type_2.name, "Type", 4);
  memcpy(xml_body_gpf_type_2.value, "VideoEncoder", 12);

  soap += "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
  GenSoap(this->message.http_content->envelope);
}


void GetStreamUri::Init(char* profiletoken)
{
  http_content.xml_info = &xml_info;
  http_content.envelope = &envelope;
  message.http_content = &http_content;
  memcpy(xml_info.version, "1.0", 3);
  memcpy(xml_info.encoding,"utf-8",5);
  envelope.xmlns_ns = &xmlns_env_ns;
  memcpy(xmlns_env_ns.value,"s", 1);
  envelope.xmlns_value = &xmlns_env_value;
  char* s = (char*)"http://www.w3.org/2003/05/soap-envelope";
  memcpy(xmlns_env_value.value,"http://www.w3.org/2003/05/soap-envelope", strlen(s));
  memcpy(envelope.ns, "s", 1);
  memcpy(envelope.name, "Envelope", 8);
  envelope.child = &xml_header;
  memcpy(xml_header.ns, "s", 1);
  memcpy(xml_header.name, "Header", 6);
  xml_header.child = &xml_header_security;
  memcpy(xml_header_security.ns, "wsse", 4);
  memcpy(xml_header_security.name, "Security", 8);
  xml_header_security.child = &xml_header_sec_usernametoken;
  memcpy(xml_header_sec_usernametoken.ns, "wsse", 4);
  memcpy(xml_header_sec_usernametoken.name, "UsernameToken", 13);
  xml_header_sec_usernametoken.child = &xml_header_sec_unt_username;
  memcpy(xml_header_sec_unt_username.ns, "wsse", 4);
  memcpy(xml_header_sec_unt_username.name, "Username", 8);
  memcpy(xml_header_sec_unt_username.value, m_username.c_str(), m_username.length());
  xml_header_sec_unt_username.brother = &xml_header_sec_unt_password;
  memcpy(xml_header_sec_unt_password.ns, "wsse", 4);
  memcpy(xml_header_sec_unt_password.name, "Password", 8);
  s = (char*)"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest";
  memcpy(xml_header_sec_unt_password.type, s, strlen(s));
  Wsse_onvif_ljc wsse;
  wsse.Template(nonce, created, m_passwd.c_str(), password_digest, noncebase64, SHA1);
  memcpy(xml_header_sec_unt_password.value, password_digest, strlen(password_digest));
  xml_header_sec_unt_password.brother = &xml_header_sec_unt_nonce;
  memcpy(xml_header_sec_unt_nonce.ns, "wsse", 4);
  memcpy(xml_header_sec_unt_nonce.name, "Nonce", 5);
  memcpy(xml_header_sec_unt_nonce.value, noncebase64, strlen(noncebase64));
  xml_header_sec_unt_nonce.brother = &xml_header_sec_unt_created;
  memcpy(xml_header_sec_unt_created.ns, "wsu", 3);
  memcpy(xml_header_sec_unt_created.name, "Created", 7);
  memcpy(xml_header_sec_unt_created.value, created, strlen(created));
  xml_header.brother = &xml_body;
  memcpy(xml_body.ns, "s", 1);
  memcpy(xml_body.name, "Body", 4);
  xml_body.child = &getstreamuri;
  memcpy(getstreamuri.name, "GetStreamUri", 12);
  getstreamuri.xmlns_ns = &gstmuri_ns_non;
  getstreamuri.xmlns_value = &gstmuri_value_non;
  s = (char*)"http://www.onvif.org/ver20/media/wsdl";
  memcpy(gstmuri_value_non.value, s, strlen(s));
  getstreamuri.child = &gstmuri_protocol;
  memcpy(gstmuri_protocol.name, "Protocol", 8);
  memcpy(gstmuri_protocol.value, "RTSP", 4); 
  gstmuri_protocol.brother = &gstmuri_profiletoken;
  memcpy(gstmuri_profiletoken.name, "ProfileToken", 13);
  memcpy(gstmuri_profiletoken.value, profiletoken, strlen(profiletoken));
  soap += "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
  GenSoap(message.http_content->envelope);
}


set<string> Udpprobe::probe()
{
  set<string> res;
  ProbeDecorator c;
  c.Init();
  char* buf = new char[MAXLINE];
  memset(buf, 0, MAXLINE);
  memcpy(buf, (char*)(c.soap).c_str(), (c.soap).length());
  int sockfd;
  struct sockaddr_in serveraddr, clientaddr;
  struct ip_mreqn group;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  
  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = inet_addr("192.168.1.102");
  serveraddr.sin_port = htons(SERVER_PORT);

  bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

  inet_pton(AF_INET, GROUP, &group.imr_multiaddr);
  inet_pton(AF_INET, "192.168.1.102", &group.imr_address);
  group.imr_ifindex = if_nametoindex("eth0");
  setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF, &group, sizeof(group));
  struct timeval tv_out;
  tv_out.tv_sec = 5;
  tv_out.tv_usec = 0;
  setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&tv_out, sizeof(tv_out));
  bzero(&clientaddr, sizeof(clientaddr));
  clientaddr.sin_family = AF_INET;
  inet_pton(AF_INET, GROUP, &clientaddr.sin_addr.s_addr);
  clientaddr.sin_port = htons(CLIENT_PORT);
  sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&clientaddr, sizeof(clientaddr));
  sleep(1);
  int sock_len = sizeof(serveraddr);
  int len;
  struct timeval timeout;
  timeout.tv_sec = 1;//秒
  timeout.tv_usec = 0;//微秒
  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) {
      cout << "setsockopt failed:" << endl;;
  }
  char buff[MAXLINE]={};
  for(size_t i=0;i<5;i++){ 
    len = recvfrom(sockfd, buff, sizeof(buff)-1, 0, (struct sockaddr *)&serveraddr,   (socklen_t*)&sock_len);
    if(strlen(buff) != 0){
        res.emplace(buff);
    }
  }
  close(sockfd);
  return res;
}
