#include "functions.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

Functions::Functions(double precision) {
	this->precision = precision;
}

/* precise functions */

double Functions::min(double a, double b) {
	return a < b ? a : b;
}
double Functions::min(const vector<double> *variables) {
	double min = variables->at(0);
	for (int i = 0; i < variables->size(); i++) {
		if (min > variables->at(i)) {
			min = variables->at(i);
		}
	}
	return min;
}
double Functions::max(double a, double b) {
	return a > b ? a : b;
}
double Functions::max(const vector<double> *variables) {
	double max = variables->at(0);
	for (int i = 0; i < variables->size(); i++) {
		if (max < variables->at(i)) {
			max = variables->at(i);
		}
	}
	return max;
}
double Functions::pos(double a) {
	return a > 0.0 ? a : 0.0;
}
double Functions::abs(double a) {
	return max(a, 0) - min(a, 0);
}

double Functions::getArea(const Placement *placement) {
	double top = 0;
	double bottom = 0;
	double left = 0;
	double right = 0;

	const vector<Shape*> *shapes = placement->getShapes();

	left = shapes->at(0)->getMinX();
	right = shapes->at(0)->getMaxX();
	bottom = shapes->at(0)->getMinY();
	top = shapes->at(0)->getMaxY();

	for (int i = 1; i < shapes->size(); i++) {
		Shape *current = shapes->at(i);
		if (current->getMinX() < left) {
			left = current->getMinY();
		}
		if (current->getMaxX() > right) {
			right = current->getMaxX();
		}
		if (current->getMinY() < bottom) {
			bottom = current->getMinY();
		}
		if (current->getMaxY() > top) {
			top = current->getMaxY();
		}
	}

	return (top - bottom) * (right - left);
}

double Functions::getOverlap(const Placement *placement) {
	const vector<Shape*> *shapes = placement->getShapes();
	double overlap = 0.0;
	for (int i = 0; i < shapes->size(); i++) {
		for (int j = i + 1; j < shapes->size(); j++) {
			overlap += this->getOverlap(shapes->at(i), shapes->at(j));
		}
	}
	return overlap;
}
double Functions::getOverlap(const Shape *a, const Shape *b) {
	const vector<Rect*> *aRects = a->getRectangles(), *bRects = b->getRectangles();
	double overlap = 0.0;
	for (int i = 0; i < aRects->size(); i++) {
		for (int j = 0; j < bRects->size(); j++) {
			overlap += this->getOverlap(aRects->at(i), bRects->at(j));
		}
	}
	return overlap;	
}
double Functions::getOverlap(const Rect *a, const Rect *b) {
	return pos(min(a->getMaxX() - b->getMinX(),  b->getMaxX() - a->getMinX())) * pos(min(a->getMaxY() - b->getMinY(), b->getMaxY() - a->getMinY()));	
}



/* Smooth functions */

double Functions::smoothMax(const vector<double> *variables) {
	double max = 0;
	for (int i = 0; i < variables->size(); i++) {
		max += exp(variables->at(i) / precision);
	}
	return precision * log(max);
}
double Functions::smoothMin(const vector<double> *variables) {
	double min = 0;
	for (int i = 0; i < variables->size(); i++) {
		min += exp(- variables->at(i) / precision);
	}
	return - precision * log(min);
}
double Functions::smoothMax(double a, double b) {
	return precision * log(exp(a / precision) + exp(b / precision));
}
double Functions::smoothMin(double a, double b) {
	return - precision * log(exp(- a / precision) + exp(- b / precision));
}
double Functions::smoothPos(double a) {
	return smoothMax(a, 0);
}
double Functions::smoothAbs(double a) {
	return smoothMax(a, 0) - smoothMin(a, 0);
}
double Functions::getSmoothArea(const Placement *placement) {
	const vector<Shape*> *shapes = placement->getShapes();
	vector<double> left(shapes->size()), right(shapes->size()), top(shapes->size()), bottom(shapes->size());

	for (int i = 0; i < shapes->size(); i++) {
		left[i] = shapes->at(i)->getMinX();
		right[i] = shapes->at(i)->getMaxX();
		top[i] = shapes->at(i)->getMaxY();
		bottom[i] = shapes->at(i)->getMinY();
	}

	return (smoothMax(&right) - smoothMin(&left)) * (smoothMax(&top) - smoothMin(&bottom));
}
double Functions::getSmoothOverlap(const Placement *placement) {
	const vector<Shape*> *shapes = placement->getShapes();

	double overlap = 0.0;
	for (int i = 0; i < shapes->size(); i++) {
		for (int j = i + 1; j < shapes->size(); j++) {
			overlap += this->getSmoothOverlap(shapes->at(i), shapes->at(j));
		}
	}
	return overlap;
}
double Functions::getSmoothOverlap(const Shape *a, const Shape *b) {
	const vector<Rect*> *aRects = a->getRectangles(), *bRects = b->getRectangles();
	double overlap = 0.0;
	for (int i = 0; i < aRects->size(); i++) {
		for (int j = 0; j < bRects->size(); j++) {
			overlap += this->getSmoothOverlap(aRects->at(i), bRects->at(j));
		}
	}
	return overlap;
}
double Functions::getSmoothOverlap(const Rect *a, const Rect *b) {
	double overlapX = smoothPos(smoothMin(a->getMaxX() - b->getMinX(),  b->getMaxX() - a->getMinX()));
	double overlapY = smoothPos(smoothMin(a->getMaxY() - b->getMinY(), b->getMaxY() - a->getMinY()));

	return overlapX * overlapY;
}

/* Derivatives */
double Functions::smoothMinD(int i, const vector<double> *variables) {
	/* min'_{x_i}(x_1, ..., x_n) = (exp(- x_i / phi) / sum(exp(-x_k / phi))) */
	double numerator = exp(- variables->at(i) / precision);
	double denominator = 0;
	for (int k = 0; k < variables->size(); k++) {
		denominator += exp(- variables->at(k) / precision);
	}

	return numerator / denominator;
}

double Functions::smoothMinD(int i, double a, double b) {
	if (i == 0) {
		return exp(- a / precision) / (exp(- a / precision) + exp(- b / precision));
	} else {
		return exp(- b / precision) / (exp(- a / precision) + exp(- b / precision));
	}
}

double Functions::smoothMaxD(int i, const vector<double> *variables) {
	/* min'_{x_i}(x_1, ..., x_n) = (exp(x_i / phi) / sum(exp(x_k / phi))) */
	double numerator = exp(variables->at(i) / precision);
	double denominator = 0;
	for (int k = 0; k < variables->size(); k++) {
		denominator += exp(variables->at(k) / precision);
	}
	return numerator / denominator;	
}

double Functions::smoothMaxD(int i, double a, double b) {
	if (i == 0) {
		return exp(a / precision) / (exp(a / precision) + exp(b / precision));
	} else {
		return exp(b / precision) / (exp(a / precision) + exp(b / precision));
	}
}

double Functions::smoothPosD(double a) {
	return exp(a / precision) / (1 + exp(a / precision));
}
double Functions::smoothAbsD(double a) {
	return exp(a / precision) / (1 + exp(a / precision)) - exp(- a / precision) / (1 + exp(- a / precision));
}
double Functions::getSmoothAreaD(int i, char c, bool max, const Placement *placement) {
	const vector<Shape*> *shapes = placement->getShapes();
	vector<double> left(shapes->size()), right(shapes->size()), top(shapes->size()), bottom(shapes->size());

	for (int j = 0; j < shapes->size(); j++) {
		left[j] = shapes->at(j)->getMinX();
		right[j] = shapes->at(j)->getMaxX();
		top[j] = shapes->at(j)->getMaxY();
		bottom[j] = shapes->at(j)->getMinY();
	}

	if (c == 'x') {
		if (max) {
			return smoothMaxD(i, &right) * (smoothMax(&top) - smoothMin(&bottom));
		} else {
			return - smoothMinD(i, &left) * (smoothMax(&top) - smoothMin(&bottom));
		}
	} else {
		if (max) {
			return (smoothMax(&right) - smoothMin(&left)) * smoothMaxD(i, &top);
		} else {
			return - (smoothMax(&right) - smoothMin(&left)) * smoothMinD(i, &bottom);
		}
	}
}

/* second rectangle is considered as constant */
double Functions::getSmoothOverlapD(char c, bool max, const Rect *a, const Rect *b) {
	double overlapX;
	double overlapY;

	if (c == 'x') {
		if (max) {
			overlapX = smoothPosD(smoothMin(a->getMaxX() - b->getMinX(),  b->getMaxX() - a->getMinX())) * smoothMinD(0, a->getMaxX() - b->getMinX(),  b->getMaxX() - a->getMinX());
		} else {
			overlapX = - smoothPosD(smoothMin(a->getMaxX() - b->getMinX(),  b->getMaxX() - a->getMinX())) * smoothMinD(1, a->getMaxX() - b->getMinX(),  b->getMaxX() - a->getMinX());
		}
		overlapY = smoothPos(smoothMin(a->getMaxY() - b->getMinY(), b->getMaxY() - a->getMinY()));
	} else {
		if (max) {
			overlapY = smoothPosD(smoothMin(a->getMaxY() - b->getMinY(), b->getMaxY() - a->getMinY())) * smoothMinD(1, a->getMaxY() - b->getMinY(), b->getMaxY() - a->getMinY());
		} else {
			overlapY = - smoothPosD(smoothMin(a->getMaxY() - b->getMinY(), b->getMaxY() - a->getMinY())) * smoothMinD(0, a->getMaxY() - b->getMinY(), b->getMaxY() - a->getMinY());
		}
		overlapX = smoothPos(smoothMin(a->getMaxX() - b->getMinX(),  b->getMaxX() - a->getMinX()));
	}

	return overlapX * overlapY;	
}

double Functions::getSmoothOverlapD(char c, bool max, const Rect *a, const Shape *b) {
	vector<Rect*> *rectangles = b->getRectangles();
	double result = 0;
	int i;

	for (i = rectangles->size() - 1; i > -1; i--) {
		result += getSmoothOverlapD(c, max, a, rectangles->at(i));
	}

	return result;
}