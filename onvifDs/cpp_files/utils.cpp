#include "../h_files/utils.h"
#include <string.h>

string insert_char(string src, string dst, int location)
{
  string res;
  
  res = src.substr(0, location) + dst + src.substr(location, src.length());
  return res;
}

void getLocalInfo(std::vector<std::string>& res)
{
      int fd;
      int interfaceNum = 0;
      struct ifreq buf[16];
      struct ifconf ifc;
      char ip[32] = {0};
      char* s = (char*)"eth";  
      if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket");
   
           close(fd);
       }
   
       ifc.ifc_len = sizeof(buf);
       ifc.ifc_buf = (caddr_t)buf;
       if (!ioctl(fd, SIOCGIFCONF, (char *)&ifc)){
           interfaceNum = ifc.ifc_len / sizeof(struct ifreq);
           while (interfaceNum-- > 0){    
                if(!strncmp(s,buf[interfaceNum].ifr_name,3)){   
                    if (!ioctl(fd, SIOCGIFADDR, (char *)&buf[interfaceNum]))
                    {
                        snprintf(ip, sizeof(ip), "%s",
                            (char *)inet_ntoa(((struct sockaddr_in *)&(buf[interfaceNum].ifr_addr))->sin_addr));
			res.emplace_back(ip);
                    }
                    else
                    {
                        close(fd);
                    }
                }
          }
      }
      else{
          printf("ioctl: %s [%s:%d]\n", strerror(errno), __FILE__, __LINE__);
          close(fd);
      }
      close(fd);
  }


