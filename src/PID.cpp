#include "PID.h"


PID::PID() : Kp(0.0), Ki(0.0), Kd(0.0), p_error(0.0), i_error(0.0), d_error(0.0), prv_cte(0.0) {}
PID::~PID() {}

void PID::Init(const double &_Kp, const  double &_Ki, const double &_Kd)
{
	Kp = _Kp;
	Ki = _Ki;
	Kd = _Kd;
}

double PID::TotalError(const double &cte) 
{
	p_error =  cte*Kp;
	i_error += cte*Ki;
	d_error =  (cte - prv_cte)*Kd;
	prv_cte =  cte;

	return - p_error - i_error - d_error;
}

