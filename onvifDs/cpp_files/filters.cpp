#include <regex>
#include <iostream>
#include "../h_files/filters.h"

using namespace std;

vector<string> device_adr_parse::parse(string src){
	vector<string> res;
	regex device_service_reg("<d:XAddrs>(.*)</d:XAddrs>");
	regex hostname_reg("<d:XAddrs>(?:.*)//(.*)/onvif(?:.*)</d:XAddrs>");
	smatch device_service_m;
	smatch hostname_m;
	bool found = regex_search(src, device_service_m, device_service_reg);
	bool f = regex_search(src, hostname_m, hostname_reg);
	if(found && f) 
	{ 
	  res.push_back(device_service_m.str(1));
	  res.push_back(hostname_m.str(1));
	}
	return res;
}

string device_num_parse::parse(string src){
	string res;
	regex num_r("(?:\\d{3})[.](?:\\d{0,3})[.](\\d{0,3})[.](\\d{0,3})");
	smatch num_s;
	regex_search(src, num_s, num_r);
	string num_a = num_s.str(1);
	string num_b = num_s.str(2);
	res = num_a + "_" + num_b;
	return res;
}

string media_adr_parse::parse(string src){
	string res;
	//Media2
	regex media2_adr_reg("<tds:XAddr>(.*)Media2</tds:XAddr>");
	smatch media2_adr_m;
	string s = src;
	bool media2_found = regex_search(s, media2_adr_m, media2_adr_reg);
	if(media2_found) {
		res = media2_adr_m.str(1);
		res += "Media2";
	}
	//Media
	else {
		regex media_adr_reg("<tds:XAddr>(.*)Media</tds:XAddr>");
		smatch media_adr_m;
		bool media_found = regex_search(s, media_adr_m, media_adr_reg);
		if(media_found){
			res = media_adr_m.str(1);
			res += "Media";
		}
	}
	return res;
}


vector<string> ProfileTokenParse_Media::parse(string src, int main_sub){
	vector<string> res;
	regex main_reg("Profiles token=\"(.*)\"(?:(.)*)\"true(?:(.)*)mainStream");
	regex sub_reg("</trt:Profiles>(?:.*)Profiles token=\"(.*)\"(?:.*)\"true(?:.*)subStream");
	regex main_encoder_reg("<tt:VideoEncoderConfiguration token=\"(.*)\"(?:.*)VideoEncoder_1");
	regex sub_encoder_reg("Profile_2(?:.*)<tt:VideoEncoderConfiguration token=\"(.*)\"\\s(?:.*)VideoEncoder_2");
	smatch main_m;
	smatch sub_m;
	smatch main_encoder_m;
	smatch sub_encoder_m;
	bool mainstream_found = true;
	bool substream_found = true;
	switch (main_sub){
		case MAINSTREAM:        
			mainstream_found = regex_search(src, main_m, main_reg);
			regex_search(src, main_encoder_m, main_encoder_reg);
			if(mainstream_found){
				res.push_back(main_m.str(1));
				res.push_back(main_encoder_m.str(1));
			}
			break;
		case SUBSTREAM:
			substream_found = regex_search(src, sub_m, sub_reg);
			regex_search(src, sub_encoder_m, sub_encoder_reg);
			if(substream_found){

				res.push_back(sub_m.str(1));
                                res.push_back(sub_encoder_m.str(1));
			}  
			break;      
	}
	return res;
}

vector<string> ProfileTokenParse_Media2::parse(string src, int main_sub){
	vector<string> res;
	regex main_reg("Profiles token=\"(.*)\"(?:(.)*)\"true(?:(.)*)mainStream");
	regex sub_reg("</trt:Profiles>(?:.*)Profiles token=\"(.*)\"(?:.*)\"true(?:.*)subStream");
	regex main_encoder_reg("<tr2:VideoEncoder token=\"(.*)\"\\sG");
	regex sub_encoder_reg("Profile_2(?:.*)<tt:VideoEncoderConfiguration token=\"(.*)\"\\s(?:.*)VideoEncoder_2");
	smatch main_m;
	smatch sub_m;
	smatch main_encoder_m;
	smatch sub_encoder_m;
	bool mainstream_found;
	bool substream_found;
	bool efound;
	switch (main_sub){
		case MAINSTREAM:        
			mainstream_found = regex_search(src, main_m, main_reg);
			efound = regex_search(src, main_encoder_m, main_encoder_reg);
			if(mainstream_found && efound){
				res.push_back(main_m.str(1));
                                res.push_back(main_encoder_m.str(1));
			}
			else{
				cout << src << endl;
				cout << "not found" << endl;
			}
			if(!efound) cout << "not efound" << endl;
			break;
		case SUBSTREAM:
			substream_found = regex_search(src, sub_m, sub_reg);
			regex_search(src, sub_encoder_m, sub_encoder_reg);

			if(substream_found){
				res.push_back(sub_m.str(1));
                                res.push_back(sub_encoder_m.str(1));
			}  
			break;      
	}
	return res;
}

string rtsp_parse::parse(string src){
 
	string res;
	regex rtsp_reg("(.*)[?]");
	smatch rtsp_m;
	regex_search(src, rtsp_m, rtsp_reg);
	res = rtsp_m.str(1);
	return res;
}

string StreamUriParse::parse(string src){
	string res;
	regex streamuri_reg("<tr2:Uri>(.*)</tr2:Uri>");
	smatch streamuri_m;
	bool streamuri_found = regex_search(src, streamuri_m, streamuri_reg);
	if(streamuri_found)
	{
		res = streamuri_m.str(1);
	}
	return res;
}










