#ifndef __MASTER_H__
#define __MASTER_H__

#include <uWS/uWS.h>
#include "json.hpp"
#include "config.h"
#include "PID.h"
#include "1D_KF.h"

class Master
{
public:
	Master();	

	void run					 ();
private:

	std::string hasData			 (const std::string &s);
	void read_cfg				 (const std::string &cfg_path);
	
	
	uWS::Hub					 h;

	Config 						 cfg;
	PID							 steering_pid;
	KF_1D						 kf;
	
	std::fstream				 in;
	std::istringstream			 iss;
	std::string					 buff;

	unsigned int			 	 port;
};


#endif // __MASTER_H__ 