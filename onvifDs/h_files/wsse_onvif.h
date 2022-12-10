#pragma once
#ifndef wsse_onvif_h
#define wsse_onvif_h
#include <openssl/evp.h>
#include "cbase64.h"
#include <cstring>
#include <string>

#define MD5 0
#define SHA1 1
#define SHA256 2
class Wsse_onvif_ljc
{
public:
  void Template(char* nonce, char* created, const char* password, char* password_digest, char* noncebase64, const int type);
  void s2base64(char* src, char* des);
  void Init(const int ctype);
  void GenNonoce(char *nonce);
  void GetCreated(char* created);
  void Update(char* nonce, char* created, const char* password);
  void Final();
private:
  char *nonce, *noncebase64, *created, *password, *HA, *password_digest; 
  void* ctx; 
};
#endif
