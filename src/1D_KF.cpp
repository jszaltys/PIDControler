#include "1D_KF.h"

KF_1D::~KF_1D(){}

KF_1D::KF_1D():Q(0.0),R(0.0),Pc(0.0),G(0.0),P(1.0),Xe(0.0){}
void KF_1D::init(const double& _Q, const double&_R)
{
	Q = _Q;
	R = _R;
}

double KF_1D::process(const double& measurments)
{
	predict();
	update(measurments);

	return Xe;
}

void KF_1D::predict()
{
	Pc = P + Q;
	G = Pc / (Pc + R);
}
void KF_1D::update(const double& measurments)
{
	P = (1.0 - G)*Pc;
	Xe = G*(measurments - Xe) + Xe;
}
