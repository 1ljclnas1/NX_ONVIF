#ifndef FILTERS_H
#define FILTERS_H
#define MAINSTREAM 0
#define SUBSTREAM 1

#include <iostream>
#include <vector>

using namespace std;
class device_adr_parse{

	public:
		vector<string> parse(string src);
};

class device_num_parse{
	public: 
		string parse(string src);
};
class media_adr_parse
{
	public:
		string parse(string src);
};
class ProfileTokenParse_Media
{
	public:
		vector<string> parse(string src, int main_sub);
};


class ProfileTokenParse_Media2
{
	public:
		vector<string> parse(string src, int main_sub);

};
class rtsp_parse
{
	public:
		string parse(string src);
};
class StreamUriParse
{
	public:
		string parse(string src);
};

#endif
