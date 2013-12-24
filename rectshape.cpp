#include "rectshape.h"
#include <iostream>
#include "functions.h"

using namespace std;

RectShape::RectShape(double x, double y, double offsetX, double offsetY) {
	this->x = x;
	this->y = y;
	this->offsetX = offsetX > 0 ? offsetX : - offsetX;
	this->offsetY = offsetY > 0 ? offsetY : - offsetY;

	this->rectangles = new std::vector<Rect*>(1, new Rect(2 * offsetX, 2 * offsetY, x - offsetX, y - offsetY));
}

double RectShape::getMaxX() const {
	return x + offsetX;
}
double RectShape::getMinX() const {
	return x - offsetX;
}
double RectShape::getMaxY() const {
	return y + offsetY;
}
double RectShape::getMinY() const {
	return y - offsetY;
}

/* assume offsets are constants */
double RectShape::getMaxXD(int i) const {
	if (i == 0) {
		return 1.0;
	} else {
		return 0.0;
	}
}
double RectShape::getMinXD(int i) const {
	if (i == 0) {
		return 1.0;
	} else {
		return 0.0;
	}
}
double RectShape::getMaxYD(int i) const {
	if (i == 1) {
		return 1.0;
	} else {
		return 0.0;
	}
}
double RectShape::getMinYD(int i) const {
	if (i == 1) {
		return 1.0;
	} else {
		return 0.0;
	}
}

void RectShape::update(const double *solution, int i) {
	this->x = solution[i];
	this->y = solution[i + 1];
	this->rectangles->at(0)->update(x - offsetX, y - offsetY);
}

double RectShape::getSmoothOverlapD(int i, Shape *b, double precision) const {
	Functions *functions = new Functions(precision);
	double result = 0;

	if (i == 0) {
		result = functions->getSmoothOverlapD('x', true, rectangles->at(0), b) + functions->getSmoothOverlapD('x', false, rectangles->at(0), b);
	} else {
		result = functions->getSmoothOverlapD('y', true, rectangles->at(0), b) + functions->getSmoothOverlapD('y', false, rectangles->at(0), b);
	}

	delete functions;
	
	return result;
}

void RectShape::setVariables(double *vect, int i) const {
	vect[i] = x;
	vect[i + 1] = y;
}