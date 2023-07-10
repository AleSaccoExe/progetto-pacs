#include "point.hpp"
#include "utility.hpp"
#include <iostream>
#include <functional>
#include <vector>
#include <cmath>
#include <iomanip>
using namespace geometry;


int main()
{
	
	std::function<Real(Real, Real, Real)> F = [](Real x, Real y, Real z)->Real{return z-x*x*x-y*y;};
	point P(1., 0., 1.), Q(0., 1., 1.); // due punti qualsiasi sulla superficie;
	unsigned max_it = 15; // iterazioni massime del metodo
	unsigned max_newton_it = 10; // iterazioni massime per il metodo di newton
	Real h = 1e-4; // da usare per il calcolo della derivata
	std::vector<point> Ms; // vettore dei punti medi
	std::vector<point> Ps; 
	std::vector<point> Qs; 
	auto M = 0.5*(P+Q); // punto medio
	Ms.push_back(M);
	Ps.push_back(P);
	Qs.push_back(Q);
	unsigned it = 0; // numerp di iterazioni effettuate
	Real res = 1.0;
	while(std::abs( F(M[0], M[1], M[2]) )>TOLL && it<max_it )
	{
		

		point gradP, gradQ; // gradienti di F in P e Q
		gradP[0] = ( F(P[0]+h, P[1], P[2]) - F(P[0]-h, P[1], P[2]) )/(2.*h);
		gradP[1] = ( F(P[0], P[1]+h, P[2]) - F(P[0], P[1]-h, P[2]) )/(2.*h);
		gradP[2] = ( F(P[0], P[1], P[2]+h) - F(P[0], P[1], P[2]-h) )/(2.*h);


		gradQ[0] = ( F(Q[0]+h, Q[1], Q[2]) - F(Q[0]-h, Q[1], Q[2]) )/(2.*h);
		gradQ[1] = ( F(Q[0], Q[1]+h, Q[2]) - F(Q[0], Q[1]-h, Q[2]) )/(2.*h);
		gradQ[2] = ( F(Q[0], Q[1], Q[2]+h) - F(Q[0], Q[1], Q[2]-h) )/(2.*h);

		// vengono definite le rette:

		std::function<point(Real)> rP = [&gradP, &M](Real t)->point{return M+t*gradP;};
		std::function<point(Real)> rQ = [&gradQ, &M](Real t)->point{return M+t*gradQ;};

		// vengono definite le equazioni

		std::function<Real(Real)> fP = [&rP, &F](Real t)->Real{return F(rP(t)[0],rP(t)[1],rP(t)[2]);};
		std::function<Real(Real)> fQ = [&rQ, &F](Real t)->Real{return F(rQ(t)[0],rQ(t)[1],rQ(t)[2]);};

		// vengono risolte le equazioni

		Real tP = utility::newton(fP, 0., max_newton_it, TOLL, h);
		Real tQ = utility::newton(fQ, 0., max_newton_it, TOLL, h);

		P = rP(tP);
		
		Q = rQ(tQ);
		auto next_M = 0.5*(P+Q);
		res = (M-next_M).norm2squared();
		M = next_M;
		Ps.push_back(P);
		Qs.push_back(Q);
		Ms.push_back(M);
		it++;
		std::cout<<"|F(M)| = "<<std::abs( F(M[0], M[1], M[2]) )<<std::endl;
	}
	std::cout<<std::setprecision(10);
	std::cout<<"\nRisultati:\nNumero di iterazioni: "<<it<<std::endl;
	for(unsigned i =0; i<Ms.size(); ++i)
	{
		std::cout<<"Estremi:\n";
		std::cout<<"("<<Ps[i][0]<<", "<<Ps[i][1]<<", "<<Ps[i][2]<<"),  "<<"("<<Qs[i][0]<<", "<<Qs[i][1]<<", "<<Qs[i][2]<<")";
		std::cout<<std::endl;
		std::cout<<"Punto medio:\n";
		std::cout<<"("<<Ms[i][0]<<", "<<Ms[i][1]<<", "<<Ms[i][2]<<")"<<std::endl;
	}
	return 0;

}