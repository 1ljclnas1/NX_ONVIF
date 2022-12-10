//
// Created by ljc on 22-12-8.
//
#include "ConfigParser.h"

ConfigParser::ConfigParser(const string& configPath): m_path(configPath){
    m_cnf = YAML::LoadFile(m_path);
    for(const auto s_item : m_cnf["camera_auth"]){
        source cur_src;
        cur_src.m_hostname = s_item.second["hostname"].as<string>();
        cur_src.m_onvif_passwd = s_item.second["onvif_username"].as<string>();
        cur_src.m_result_username = s_item.second["result_username"].as<string>();
        cur_src.m_result_passwd = s_item.second["result_passwd"].as<string>();
        cur_src.m_model = s_item.second["model"].as<string>();
        m_source_list.emplace_back(cur_src);
    }
}