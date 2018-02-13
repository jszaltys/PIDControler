#ifndef __PID_H__
#define __PID_H__

class PID 
{
public:
  /*
  * Errors
  */
  double p_error;
  double i_error;
  double d_error;
  double prv_cte;

  /*
  * Coefficients
  */ 
  double Kp;
  double Ki;
  double Kd;

  /*
  * Constructor
  */
  PID();

  /*
  * Destructor.
  */
   ~PID();


  /*
  * Initialize PID.
  */
   void Init(const double & Kp, const double & Ki, const double & Kd);

  /*
  * Update the PID error variables given cross track error.
  */
  double TotalError(const double &cte);

};

#endif /* __PID_H__ */
