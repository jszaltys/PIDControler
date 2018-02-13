#include "master.h"

Master::Master(){}


std::string Master::hasData(const std::string& s)
{
	auto found_null = s.find("null");
	auto b1			= s.find_first_of("[");
	auto b2			= s.find_first_of("]");

	if (found_null != std::string::npos)
		return "";

	else if (b1 != std::string::npos && b2 != std::string::npos)
		return s.substr(b1, b2 - b1 + 1);

	return "";
}
void Master::read_cfg(const std::string &cfg_path)
{
	double _kp = 0.0, _ki = 0.0, _kd = 0.0;

	cfg.read_cfg(cfg_path);
	
	for (auto &r : cfg.mstringmap)
	{	
		if (r.first == "PROPORTIONAL_GAIN")
			_kp = std::stod(r.second);

		else if (r.first == "INTEGRAL_GAIN")
			_ki = std::stod(r.second);

		else if (r.first == "DERIVATIVE_GAIN")
			_kd = std::stod(r.second);

		else if (r.first == "PORT")
			port = std::stod(r.second);
	}
	steering_pid.Init(_kp, _ki, _kd);
}
void Master::run()
{
	read_cfg("../data/cfg.txt");
	
	std::cout << "PROPORTIONAL GAIN = " << steering_pid.Kp << std::endl;
	std::cout << "INTEGRAL     GAIN = " << steering_pid.Ki << std::endl;
	std::cout << "DERIVATIVE   GAIN = " << steering_pid.Kd << std::endl;
	std::cout << "PORT 		  = " << port << std::endl;

	kf.init(1e-6f,1.12184278324081E-05);
	

	h.onMessage			([this](uWS::WebSocket<uWS::SERVER> ws, char *message, size_t length, uWS::OpCode opCode)
	{
		if (length && length > 2 && message[0] == '4' && message[1] == '2')
		{	
			auto s = hasData(std::string(message));	
			if (s != "")
			{
				auto j = nlohmann::json::parse(s);
				if (j[0].get<std::string>() == "telemetry")
				{
					const double cte   = std::stod(j[1]["cte"].get<std::string>());
					const double steer =kf.process(steering_pid.TotalError(cte));

					std::cout << "CTE: " << cte << " Steering Value: " << steer << std::endl;
					
					nlohmann::json msgJson;
					msgJson["steering_angle"] = steer;
					msgJson["throttle"]		  = (1.0 - std::abs(steer)) * 0.3 + 0.1;
					auto msg = "42[\"steer\"," + msgJson.dump() + "]";
					ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);			
				}
			}
			else
			{
				const std::string msg = "42[\"manual\",{}]";
				ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
			}
		}
	});
	h.onHttpRequest		([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t)
	{
		const std::string s = "<h1>Hello world!</h1>";

		if (req.getUrl().valueLength == 1)
			res->end(s.data(), s.length());

		else
			res->end(nullptr, 0);
	});
	h.onConnection		([this](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req)
	{
		std::cout << "Connected!!!" << std::endl;
	});
	h.onDisconnection	([this](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) 
	{
		ws.close();
		std::cout << "Disconnected" << std::endl;
	});

	if (h.listen(port))
	{
		std::cout << "Listening to port " << port << std::endl;
	}
	else
	{
		std::cerr << "Failed to listen to port" << std::endl;
	}
	h.run();
}

