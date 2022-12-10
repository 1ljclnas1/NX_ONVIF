#pragma once 
#ifndef PROBESOAP_H
#define PROBESOAP_H
#include <iostream>
#include <cstring>
#include "wsse_onvif.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <ctime>
#include <vector>
#include "soap.h"
#include "HttpReq.h"
#include "utils.h"
#include <set>
#include <string>

#define SERVER_PORT 3702
#define CLIENT_PORT 3702
#define MAXLINE 41000
#ifndef VIDEO_ENCODE_H264
#define VIDEO_ENCODE_H264 0
#endif
#ifndef VIDEO_ENCODE_H265
#define VIDEO_ENCODE_H265 1
#endif

#define GROUP "239.255.255.250"

using namespace std;

class GetSoap
{
public:
  GetSoap(){};
  ~GetSoap(){};
public:
  void GenSoap(Xml_element* element);
  void GenXmlns(Xmlns_ns* ns, Xmlns_value* value);
  string soap;
  char nonce[30]={};
  char noncebase64[60]={};
  char created[30]={};
  char password[60]="asd123456";
  char password_digest[100]={};
  
};

class ProbeDecorator: public GetSoap
{
public:
  void Init();
  Message message;
private:
  Http_Content http_content;
  Xml_Info xml_info;
  Xmlns_ns xmlns_ns_xsi, xmlns_ns_xsd, xmlns_ns_non, xmlns_ns_messageid_wsa, xmlns_ns_to_wsa, xmlns_ns_action_wsa, xmlns_ns_probe_xsi, xmlns_ns_probe_xsd, xmlns_ns_probe_non;
  Xmlns_value xmlns_value_xsi, xmlns_value_xsd, xmlns_value_non, xmlns_value_messageid_wsa, xmlns_value_to_wsa, xmlns_value_action_wsa, xmlns_value_probe_xsi, xmlns_value_probe_xsd, xmlns_value_probe_non;
  Xml_element envelope, xml_header, xml_header_messageid, xml_header_to, xml_header_action, xml_body, xml_body_probe, xml_body_probe_types, xml_body_probe_scopes;
};

class GetServicesSoap: public GetSoap
{
public:
  Message message;
  void Init(char* hostname);
private:
  Http_Header http_header;
  Http_Content http_content;
  Xml_Info xml_info;
  Xmlns_ns xmlns_env_ns, xmlns_body_ns_xsi, xmlns_body_ns_xsd, xmlns_body_getservices_ns;
  Xmlns_value xmlns_env_value, xmlns_body_value_xsi, xmlns_body_value_xsd, xmlns_body_getservices_value;
  Xml_element envelope, xml_body, xml_body_getservices, xml_body_getservices_includecap;
};

class GetProfileSoap: public GetSoap
{
public:
    GetProfileSoap(string username, string passwd): m_username(username), m_passwd(passwd){
    }
  void Init(char* seradr,char* hostname);
  Message message;
private:
  Http_Content http_content;
  Xml_Info xml_info;
  Xml_element envelope, xml_header, xml_header_security, xml_header_sec_usernametoken, xml_header_sec_unt_username, xml_header_sec_unt_password, xml_header_sec_unt_nonce, xml_header_sec_unt_created, xml_body, xml_body_getprofiles, xml_body_gpf_type_1, xml_body_gpf_type_2;
  Xmlns_ns xmlns_env_ns, getprofiles_ns_non;
  Xmlns_value xmlns_env_value, getprofiles_value_non;
  string m_username;
  string m_passwd;
};

class GetStreamUri: public GetSoap
{
public:
    GetStreamUri(string username, string passwd): m_username(username), m_passwd(passwd){
    }
  void Init(char* profiletoken);
  Message message;
private:
  Http_Content http_content;
  Xml_Info xml_info;
  Xml_element envelope, xml_header, xml_header_security, xml_header_sec_usernametoken, xml_header_sec_unt_username, xml_header_sec_unt_password, xml_header_sec_unt_nonce, xml_header_sec_unt_created, xml_body, gstmuri_profiletoken, getstreamuri, gstmuri_protocol;
  Xmlns_ns xmlns_env_ns, gstmuri_ns_non;
  Xmlns_value xmlns_env_value, gstmuri_value_non;
    string m_username;
    string m_passwd;
};

class Udpprobe
{
public:
  set<string> probe();
};

#endif
