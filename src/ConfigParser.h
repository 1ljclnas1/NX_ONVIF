//
// Created by ljc on 22-12-8.
//

#ifndef MULTI_PIPELINE_CONFIGPARSER_H
#define MULTI_PIPELINE_CONFIGPARSER_H
#include <yaml-cpp/yaml.h>
#include <string>
#include <map>
#include <memory>

using std::string;
using std::vector;


class ConfigParser{
public:
    explicit ConfigParser(const string& configPath);
private:
    YAML::Node m_cnf;
    string m_path;
    class source{
    public:
        string m_hostname;
        string m_onvif_username;
        string m_onvif_passwd;
        string m_result_username;
        string m_result_passwd;
        string m_model;
    };
public:
    vector<source> m_source_list;
};
#endif //MULTI_PIPELINE_CONFIGPARSER_H
