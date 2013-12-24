#include "placement.h"
#include <vector>
#include "functions.h"

using namespace std;

Placement::Placement() {
	shapes = new vector<Shape*>(0);
	indices = new vector<int>(0);
	currentIndex = 0;
}

int Placement::getVariablesLength() const {
	return currentIndex;
}

void Placement::addShape(Shape* shape) {
	shapes->push_back(shape);

	currentIndex += shape->getLength();
	indices->push_back(currentIndex);
}

void Placement::update(const double *solution) {
	int i;
	for (i = shapes->size() - 1; i > -1; i--) {
		if (i > 0) {
			shapes->at(i)->update(solution, indices->at(i - 1));
		} else {
			shapes->at(0)->update(solution, 0);
		}
	}
}

double* Placement::getSmoothAreaG(double precision) const {
	double *result = new double[this->currentIndex];
	Functions *functions = new Functions(precision);

	int i;
	for (i = shapes->size() - 1; i > -1; i--) {
		int length;
		int index = indices->at(i);
		if (i > 0) {
			length = index - indices->at(i - 1);
		} else {
			length = index;
		}

		double maxXD = functions->getSmoothAreaD(i, 'x', true, this);
		double minXD = functions->getSmoothAreaD(i, 'x', false, this);
		double maxYD = functions->getSmoothAreaD(i, 'y', true, this);
		double minYD = functions->getSmoothAreaD(i, 'y', false, this);

		int start = index - length;

		for (int j = 0; j < length; j++) {
			result[start + j] = shapes->at(i)->getMaxXD(j) * maxXD + shapes->at(i)->getMinXD(j) * minXD;
			result[start + j] += shapes->at(i)->getMaxYD(j) * maxYD + shapes->at(i)->getMinYD(j) * minYD;
		}
	}

	delete functions;

	return result;
}

double* Placement::getSmoothOverlapG(double precision) const {
	double *result = new double[this->currentIndex];
	Functions *functions = new Functions(precision);

	int i;
	for (i = shapes->size() - 1; i > -1; i--) {
		int length;
		int index = indices->at(i);
		if (i > 0) {
			length = index - indices->at(i - 1);
		} else {
			length = index;
		}

		int start = index - length;		
		int j;
		
		for (j = 0; j < length; j++) {
			result[start + j] = 0;
			int k;
			for (k = shapes->size() - 1; k > -1; k--) {
				if (k != i) {
					result[start + j] = shapes->at(i)->getSmoothOverlapD(j, shapes->at(k), precision);
				}	
			}
			
		}
	}

	delete functions;

	return result;
}

double* Placement::getGradient(double alpha, double *vect, double precision) const {
	double *areaG = this->getSmoothAreaG(precision);
	double *overlapG = this->getSmoothOverlapG(precision);

	double *gradient;
		if (vect) {
			gradient = vect;
		} else {
			gradient = new double[this->currentIndex];
		}

	int i;
	for (i = 0; i < this->currentIndex; i++) {
		gradient[i] = areaG[i] * alpha + overlapG[i] * (1 - alpha);
	}

	delete []areaG;
	delete []overlapG;

	return gradient;
}

double* Placement::getVariables() const {
	double* result = new double[currentIndex];
	int i;
	for (i = shapes->size() - 1; i > -1; i--) {
		if (i > 0) {
			shapes->at(i)->setVariables(result, indices->at(i - 1));
		} else {
			shapes->at(i)->setVariables(result, 0);
		}
	}
	return result;
}