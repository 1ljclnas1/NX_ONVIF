//
// Created by ljc on 22-12-7.
//

#ifndef MULTI_PIPELINE_ONVIFURIFILTER_H
#define MULTI_PIPELINE_ONVIFURIFILTER_H

#include <regex>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

class OnvifUriFilter {
public:
    OnvifUriFilter(const std::string& conf);
    std::vector<std::string> filter(const std::vector<std::string>& addrs);
private:
    void loadConf();
    std::string m_conf;
    YAML::Node m_rules;
    std::vector<std::string> m_addrs;
};


#endif //MULTI_PIPELINE_ONVIFURIFILTER_H
