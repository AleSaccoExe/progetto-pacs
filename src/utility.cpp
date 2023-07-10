/*!	\file	utility.cpp
	\brief	Implementations of static members of class utility. */
	
#include <istream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

#include "utility.hpp"
#include "intersection.hpp"

namespace geometry
{
	string utility::getFileExtension(string filename)
	{
		// Reverse name of the file
		reverse(filename.begin(), filename.end());
				
		// Extract the format
		string format;
		istringstream iss;
		iss.str(filename);
		if (getline(iss, format, '.'))
		{
			reverse(format.begin(), format.end());
			return format;
		}
		else
		{
			reverse(filename.begin(), filename.end());
			throw runtime_error("The file " + filename + " does not have any extension.");
		}
	}
	
	
	void utility::printPoint2Seg(const Point2Seg & p2s, ostream & out)
	{
		if (p2s == Point2Seg::EXTERN)
			out << "The point is outside the segment." << endl;
		else if (p2s == Point2Seg::INTERN)
			out << "The point is (strictly) inside the segment." << endl;
		else
			out << "The point coincides with a vertex of the segment." << endl;
	}
	
	
	void utility::printPoint2Tri(const Point2Tri & p2t, ostream & out)
	{
		if (p2t == Point2Tri::EXTERN)
			out << "The point is outside the triangle." << endl;
		else if (p2t == Point2Tri::INTERN)
			out << "The point is inside the triangle." << endl;
		else if (p2t == Point2Tri::ONEDGE)
			out << "The point is on an edge of the triangle." << endl;
		else
			out << "The point coincides with a vertex of the triangle." << endl;
	}
	
	
	void utility::printLine2Plane(const Line2Plane & l2p, ostream & out)
	{
		if (l2p == Line2Plane::PARALLEL)
			out << "The line and the plane are parallel." << endl;
		else if (l2p == Line2Plane::COMPLANAR)
			out << "The line and the plane are complanar." << endl;
		else 
			out << "The line and the plane are incident." << endl;
	}
	
	
	void utility::printIntersectionType(const IntersectionType & it, const string & elements,
		ostream & out)
	{
		out << elements << " ";
		if (it == IntersectionType::NONE)
			out << "do not intersect." << endl;
		else if (it == IntersectionType::VALID)
			out << "intersect in a conformal way." << endl;
		else
			out << "intersect in a non-conformal way." << endl;
	}

	Real utility::newton(const std::function<Real(Real)> &f, Real x0, unsigned max_it, Real toll, Real h)
	{
		Real x = x0;
		for(unsigned i=0; i<max_it && std::abs(f(x))>toll; ++i)
		{
			Real fx = f(x);
			Real dfx = ( f(x+h)-f(x-h) )/(2.*h);
			Real dx = -fx/dfx;
			x = x+dx;
		}
		return x;
	}

	point utility::stayOnSurface(point P,point Q, const std::function<Real(Real, Real, Real)> & F,
			 unsigned max_it, unsigned max_newton_it, Real h)
	{
		auto M = 0.5*(P+Q);
		unsigned it = 0; // number of current iterations
		while(std::abs( F(M[0], M[1], M[2]) )>TOLL && it<max_it )
		{
		
			point gradP, gradQ; // gradients of F in P e Q
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
			M = 0.5*(P+Q);
			
			it++;
		}
		return M;

	}

}
