#include "../h_files/Config_Parser.h"

Config_Parser::Config_Parser(/* args */)
{
}

Config_Parser::Config_Parser(string address)
{
    configAddress = address;
}

void Config_Parser::GetconfigAdd(string address)
{
    configAddress = address;
    return;
}

void Config_Parser::Config_Load()
{
    if (configAddress == "")
    {
        cout << "Please enter the profile address " << endl;
        return;
    }
    int res = config.LoadFile(configAddress.c_str());
    if (res < 0)
    {
        cout << "load config is failed" << endl;
        return;
    }
    cout << "load config is succeed" << endl;
    return;
}

void Config_Parser::Config_Load(string address)
{
    GetconfigAdd(address);
    return Config_Load();
}

bool Config_Parser::Source_and_sink_Removeall()
{
    CSimpleIniA::TNamesDepend sections;
    config.GetAllSections(sections);
    for (auto it = sections.begin(); it != sections.end(); it++)
    {
        if (strstr(it->pItem, "source") || strstr(it->pItem, "sink"))
        {
            // bool done = config.Delete("Data", nullptr);
            bool done = config.Delete(it->pItem, nullptr);
            if (!done)
            {
                cout<<it->pItem<<"is remove failed"<<endl;
            }
        }
    }
    cout<<"all source and sink is remove "<<endl;
    return true;
}

bool Config_Parser::Source_add(vector<string> rtsp)
{
    cout << "source is begin adding" << endl;
    for (int i = 0; i < rtsp.size(); i++)
    {
        const char *session = ("source" + to_string(i)).c_str();
        config.SetLongValue(session, "enable", 1);
        config.SetLongValue(session, "type", 2);
        config.SetValue(session, "uri", rtsp[i].c_str());
        config.SetLongValue(session, "num-sources", 1);
    }
    int res = config.SaveFile(configAddress.c_str());
    ;
    if (!res)
    {
        cout << "the source is add succeed" << endl;
        return true;
    }
    else
    {
        cout << "the source is add failed" << endl;
        return false;
    }
}

bool Config_Parser::Sink_add(vector<string> rtsp)
{
    cout << "sink is begin adding" << endl;
    long rtsp_port = 8554;
    long udp_port = 5400;
    for (int i = 0; i < rtsp.size(); i++)
    {
        const char *session = ("sink" + to_string(i)).c_str();
        config.SetLongValue(session, "enable", 1);
        config.SetLongValue(session, "type", 4);
        config.SetLongValue(session, "codec", 1);
        config.SetLongValue(session, "enc-type", 0);
        config.SetLongValue(session, "sync", 1);
        config.SetLongValue(session, "profile", 0);
        config.SetLongValue(session, "rtsp-port", rtsp_port);
        config.SetLongValue(session, "udp-port", udp_port);
        rtsp_port += 4;
        udp_port += 4;
    }
    int res = config.SaveFile(configAddress.c_str());
    if (!res)
    {
        cout << "the sink is add succeed" << endl;
        return true;
    }
    else
    {
        cout << "the sink is add failed" << endl;
        return false;
    }
}

void Config_Parser::Config_Update(vector<string> rtsp)
{
    if (!Source_and_sink_Removeall())
    {
        cout << "reomove source is failed" << endl;
        return;
    }
    if (!Source_add(rtsp) || !Sink_add(rtsp))
    {
        cout << "update rtsp uri failed" << endl;
        return;
    }
    cout << "update rtsp uri succeed" << endl;
    return;
}

void Config_Parser::rundeepstream(){
    // th();
}
