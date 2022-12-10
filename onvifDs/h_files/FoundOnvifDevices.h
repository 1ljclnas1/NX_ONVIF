#pragma once
#ifndef foundonvifdevices_h
#define foundonvifdevices_h
#include "probesoap.h"
#include <vector>
#include <iostream>
#include "utils.h"
#include "filters.h"
#include <set>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

class FoundOnvifDevices
{
public:
    FoundOnvifDevices(string config_path);
    void probe(vector<string>& rtsp);
    void watch();
    void getserviceaddr();
    void getprofile(string username, string passwd);
    void getstreamuri(string username, string passwd);
    void setAccPwd(string account, string password);
private:
    vector<string> list;
    HttpRequest http;
    string media_res, device_adr, hostname, profile_token, video_token, streamuri;
    string account_password = "admin:asd123456@";
    set<string> device_list;
    YAML::Node m_cnf;
};
#endif
