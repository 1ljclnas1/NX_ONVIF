#include "SimpleIni.h"
#include <iostream>
#include <vector>
#include<string>
#include <cstdlib>
#include<thread>
using namespace std;
class Config_Parser
{
private:
    CSimpleIniA config;
    string configAddress;
    /*
    data */
public:
    Config_Parser(/* args */);
    Config_Parser(string address);
    void GetconfigAdd(string address);
    void Config_Load();
    void Config_Load(string address);
    bool Source_and_sink_Removeall();
    bool Source_add(vector<string>);
    bool Sink_add(vector<string> rtsp);
    void Config_Update(vector<string>);
    void rundeepstream();
    ~Config_Parser();
};

Config_Parser::~Config_Parser()
{
}
