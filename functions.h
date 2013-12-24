#include "placement.h"
#ifndef FUNCTIONS_H
class Functions {
public:
	Functions(double precision);
	double smoothMin(const std::vector<double> *var);
	double smoothMin(double a, double b);
	double smoothMax(const std::vector<double> *var);
	double smoothMax(double a, double b);
	double smoothPos(double a);
	double smoothAbs(double a);

	double smoothMaxD(int i, const std::vector<double> *var);
	double smoothMinD(int i, const std::vector<double> *var);
	double smoothMinD(int i, double a, double b);
	double smoothMaxD(int i, double a, double b);
	double smoothAbsD(double a);
	double smoothPosD(double a);

	double min(const std::vector<double> *var);
	double min(double a, double b);
	double max(const std::vector<double> *var);
	double max(double a, double b);
	double pos(double a);
	double abs(double a);

	double getArea(const Placement *placement);
	double getOverlap(const Placement *placement);
	double getOverlap(const Shape *a, const Shape *b);
	double getOverlap(const Rect *a, const Rect *b);

	double getSmoothArea(const Placement *placement);
	double getSmoothOverlap(const Placement *placement);
	double getSmoothOverlap(const Shape *a, const Shape *b);
	double getSmoothOverlap(const Rect *a, const Rect *b);

	double getSmoothAreaD(int i, char c, bool max, const Placement *placement);
	double getSmoothOverlapD(char c, bool max, const Rect *a, const Rect *b);
	double getSmoothOverlapD(char c, bool max, const Rect *a, const Shape *b);
private:
	double precision;
};
#define FUNCTIONS_H
#endif