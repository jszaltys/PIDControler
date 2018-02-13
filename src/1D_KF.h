#ifndef __1D_KF__
#define __1D_KF__


class KF_1D
{
public:
	KF_1D();
	~KF_1D();
	
	double process(const double& measurments);
	void init(const double& _Q, const double&_R);

private:

	void predict();
	void update(const double& measurments);

	double Pc;
	double G;
	double P;
	double Q;
	double R;
	double Xe;
};

#endif