#include "../h_files/wsse_onvif.h"

void Wsse_onvif_ljc::Init(const int ctype){
  ctx = (void*)EVP_MD_CTX_new();
  HA = new char[40];
  memset(HA, 0, 40);
  switch(ctype){
    case MD5:
      EVP_DigestInit((EVP_MD_CTX*)ctx, EVP_md5());
      break;
    case SHA1:
      EVP_DigestInit((EVP_MD_CTX*)ctx, EVP_sha1());
      break;
    case SHA256:
      EVP_DigestInit((EVP_MD_CTX*)ctx, EVP_sha256());
  }
}
void Wsse_onvif_ljc::Template(char* nonce, char* created, const char* password, char* password_digest, char* noncebase64, const int type){
  Init(type);
  GenNonoce(nonce);
  s2base64(nonce, noncebase64);
  GetCreated(created);
  Update(nonce, created, password);
  Final();
  s2base64(HA, password_digest);
}

void Wsse_onvif_ljc::GenNonoce(char *nonce){
  int32_t r = (int32_t)time(NULL);
  memcpy((void*)(nonce),(const void*)&r, 4);
  for(size_t i = 4; i<20; i+=4){
      r = rand();
      memcpy((void*)(nonce + i), (const void*)&r, 4);
    }
}
void Wsse_onvif_ljc::GetCreated(char* created){
  memcpy(created, "2022-08-14T08:56:47Z", 20);
}
void Wsse_onvif_ljc::s2base64(char* src, char* des){
  cbase64_encode(src, strlen(src), des);
}
void Wsse_onvif_ljc::Update(char* nonce, char* created, const char* password){
  EVP_DigestUpdate((EVP_MD_CTX*)ctx, (const void*)nonce, (unsigned int)strlen(nonce));
  EVP_DigestUpdate((EVP_MD_CTX*)ctx, (const void*)created, (unsigned int)strlen(created));
  EVP_DigestUpdate((EVP_MD_CTX*)ctx, (const void*)password, (unsigned int)strlen(password));
}
void Wsse_onvif_ljc::Final(){
  unsigned int n = 0;  
  EVP_DigestFinal_ex((EVP_MD_CTX *)ctx, (unsigned char*)HA, &n);
}


