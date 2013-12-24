#include "shape.h"

#ifndef RECTSHAPE_H
class RectShape: public Shape {
public:
	RectShape(double x, double y, double offsetX, double offsetY);
	double getMaxX() const;
	double getMinX() const;
	double getMaxY() const;
	double getMinY() const;
	double getMaxXD(int i) const;
	double getMinXD(int i) const;
	double getMaxYD(int i) const;
	double getMinYD(int i) const;

	void setVariables(double *vect, int i) const;

	double getSmoothOverlapD(int i, Shape *a, double precision = 0.1) const;

	void update(const double *solution, int index);
	virtual int getLength() const { return 2; }
private:
	double x, y, offsetX, offsetY;
};
#define RECTSHAPE_H
#endif