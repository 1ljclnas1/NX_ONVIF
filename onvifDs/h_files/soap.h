/*
         |------>Http_Header*---->{char *method, *service_address, *http_version, *hostname, *content_type, *content_type;
         |                     	  {Http_Digest *http_digest;
Message--|
         |
         |
         |------>Http_Content*	 {int *content_length;
         |             	         {Xml_Info *---------->{char *version;
         |                    	 {                     {char *encoding;
         |                    	 {
         |                     	 {Xml_element *------->{char *ns;
         |                     	 {                     {char *name;
         |                    	 {                     {char *vlaue;
         |                    	 {                     {Xmlns_ns *;
         |                    	 {                     {Xmlns_value *;
         |                    	 {                     {Xml_element *brother;
         |                    	 {                     {Xml_element *child;
*/
#pragma once
#ifndef soap_h
#define soap_h
#define MAINSTREAM 0
#define SUBSTREAM 1
struct Xmlns_value{
  Xmlns_value *next = 0;
  char value[200]={};
};

struct Xmlns_ns
{
  Xmlns_ns *next = 0;
  char value[200]={};
};

struct Xml_element{
  int self = 0;
  char type[200] = {};
  char ns[20]={};
  char name[20]={};
  Xmlns_ns *xmlns_ns = 0;
  Xmlns_value *xmlns_value = 0;
  char value[200]={};
  Xml_element *brother = 0;
  Xml_element *child = 0;
};

struct Xml_Info{
char encoding[20]={};  
char version[20]={};
};

struct Http_Content
{
  char content_length[10]={};  
  Xml_Info *xml_info = 0;
  Xml_element *envelope = 0;
  
};

struct Http_Digest
{
;
};
struct Http_Header
{
  char method[10]={};
  char service_address[20]={};
  char http_version[20]={};
  char hostname[30]={};
  char content_type[100]={};
  char content_length[10]={};
  Http_Digest *http_digest = 0;
};
struct Message
{
  Http_Header *http_header = 0;
  Http_Content *http_content = 0;
};
#endif



