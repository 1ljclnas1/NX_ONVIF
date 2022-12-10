//
// Created by ljc on 22-12-7.
//

#include "OnvifUriFilter.h"

OnvifUriFilter::OnvifUriFilter(const std::string& conf):
                               m_conf(conf){}
void OnvifUriFilter::loadConf() {
    m_rules = YAML::LoadFile(m_conf);
}
std::vector<std::string> OnvifUriFilter::filter(const std::vector<std::string>& addrs) {
    m_addrs = addrs;
    std::vector<std::string> res;
    loadConf();
    std::vector<std::regex> regexes;
    for(auto i : m_rules["filters"]){
        regexes.emplace_back(std::regex(i.second.as<std::string>().c_str()));
    }
    for(auto i : m_addrs){
        bool flag = false;
        for(auto j : regexes){
            std::smatch m;
            bool found = regex_search(i, m, j);
            if(found) {
                flag = true;
                break;
            }
        }
        if(!flag) res.emplace_back(i);
    }
    return res;
}

