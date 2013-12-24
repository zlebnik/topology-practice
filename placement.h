#include <vector>
#include "stddef.h"
#include "shape.h"

#ifndef PLACEMENT_H
class Placement {
public:
	Placement();
	void addShape(Shape* a);
	const std::vector<Shape*>* getShapes() const { return this->shapes; };
	int getVariablesLength() const;
	void update(const double *solution);
	double* getVariables() const;
	double* getSmoothAreaG(double precision = 0.1) const;
	double* getSmoothOverlapG(double precision = 0.1) const;
	double* getGradient(double alpha, double *vect = NULL, double precision = 0.1) const;
private:
	std::vector<int>* indices;
	std::vector<Shape*>* shapes;
	int currentIndex;
};
#define PLACEMENT_H
#endif
