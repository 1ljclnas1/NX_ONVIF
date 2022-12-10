#include "../h_files/FoundOnvifDevices.h"

FoundOnvifDevices::FoundOnvifDevices(string config_path){
    m_cnf = YAML::LoadFile(config_path);
}

//func probe is umcomplete (only probe one device)
void FoundOnvifDevices::probe(vector<string>& rtsp){
  vector<string> res_list;
  Udpprobe send;
  device_list = send.probe();
  cout << "device counts: " <<device_list.size() << endl;
  device_adr_parse parse_device_adr;

  if(device_list.size() == 0){
      cout << "none device" << endl;
  }

  //获取每一个设备的主机名称和设备固定入口点
  for(const auto it : device_list){
      vector<string> res = parse_device_adr.parse(it);
      device_adr = res[0];
      hostname = res[1];
      string username, passwd;
      username = m_cnf["camera_auth"]["default"]["onvif_username"].as<string>();
      passwd = m_cnf["camera_auth"]["default"]["onvif_passwd"].as<string>();
      for(auto i : m_cnf["camera_auth"]){
          for(auto j : i.second){
              if(j.first.as<string>() == "hostname"){
                  username = i.second["onvif_username"].as<string>();
                  passwd = i.second["onvif_passwd"].as<string>();
                  break;
              }
          }
      }
      //获取服务地址
      getserviceaddr();
      //获取token
      getprofile(username, passwd);
      //获取rtsp流地址
      getstreamuri(username, passwd);
      if(streamuri.length() < 7){
	    continue;
      }
      //将用户名密码插入rtsp地址中
      string a_rtsp;      
      a_rtsp = insert_char(streamuri, account_password);
      string last_rtsp;
      rtsp_parse rtspp;
      //去除掉rtsp地址中无用的参数，得到最终地址last_rtsp
      last_rtsp = rtspp.parse(a_rtsp);
      res_list.push_back(last_rtsp);
    }
    for(auto it = res_list.begin();it != res_list.end();it++){
      rtsp.push_back(*it);
    }
}

void FoundOnvifDevices::getserviceaddr(){
	GetServicesSoap getservice;
	char* getservice_res = new char[20000];
	memset(getservice_res, 0, 20000);
	getservice.Init(const_cast<char*>(hostname.c_str()));
	http.HttpPost(const_cast<char*>(device_adr.c_str()), getservice.soap.c_str(), (char*)getservice_res);
	//解析media2或media
	media_adr_parse media_parse;
	string src = getservice_res;
	media_res = media_parse.parse(getservice_res);
	delete [] getservice_res;
	getservice_res = nullptr;
}


void FoundOnvifDevices::getprofile(string username, string passwd){
	GetProfileSoap profile_soap(username, passwd);
	profile_soap.Init(const_cast<char*>(media_res.c_str()), const_cast<char*>(hostname.c_str()));
	char* http_response = new char[41000];
	memset(http_response, 0, 41000);
	http.HttpPost(const_cast<char*>(media_res.c_str()), profile_soap.soap.c_str(), http_response);
	//parseprofile
	ProfileTokenParse_Media2 parse_media2;
	string src = http_response;
	vector<string> res;
	res = parse_media2.parse(src, MAINSTREAM);
	if(res.size() >1){
		profile_token = res[0];
        video_token = res[1];
	}
	delete http_response;
	http_response = 0;
}

void FoundOnvifDevices::getstreamuri(string username, string passwd){
	GetStreamUri streamuri_soap(username, passwd);
	streamuri_soap.Init(const_cast<char*>(profile_token.c_str()));
	char* streamuri_res = new char[41000];
	memset(streamuri_res, 0, 41000);
	http.HttpPost(media_res.c_str(), streamuri_soap.soap.c_str(), streamuri_res);

	StreamUriParse* streamuri_parse = new StreamUriParse();
	string src = streamuri_res;
	streamuri = streamuri_parse->parse(src);
	delete streamuri_res, streamuri_parse;
}
void FoundOnvifDevices::setAccPwd(string account, string password){
	account_password = account + ":" + password+"@";
}
