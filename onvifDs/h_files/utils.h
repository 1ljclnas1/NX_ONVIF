#ifndef GETIP_H
#define GETIP_H
#include <stdio.h>
#include <string.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <vector>
#include <iostream>
using namespace std;

void getLocalInfo(vector<string>& res);
#if __cplusplus
extern "C" {

#endif
    string insert_char(string src, string dst, int location = 7);
#if __cplusplus
}
#endif

#endif
