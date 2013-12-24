#include <vector>
#include "rect.h"

#ifndef SHAPE_H
class Shape {
public:
	std::vector<Rect*>* getRectangles() const;
	virtual double getMaxX() const = 0;
	virtual double getMinX() const = 0;
	virtual double getMaxY() const = 0;
	virtual double getMinY() const = 0;
	virtual double getMaxXD(int i) const = 0;
	virtual double getMinXD(int i) const = 0;
	virtual double getMaxYD(int i) const = 0;
	virtual double getMinYD(int i) const = 0;

	virtual void setVariables(double *vect, int i) const = 0;

	virtual double getSmoothOverlapD(int i, Shape *a, double precision = 0.1) const = 0;

	virtual void update(const double *solution, int index) = 0;
	virtual int getLength() const = 0;
protected:
	std::vector<Rect*>* rectangles;
};
#define SHAPE_H
#endif