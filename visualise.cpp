#include "placement.h"
#include "rectshape.h"
#include "output.h"
#include <iostream>

using namespace std;

int main() {
	Placement *placement = new Placement();

	int shapesNumber;
	cin >> shapesNumber;
	int i;

	for (i = 0; i < shapesNumber; i++) {
		double x, y, offsetX, offsetY;
		cin >> x >> y >> offsetX >> offsetY;
		placement->addShape(new RectShape(x, y, offsetX, offsetY));
	}

	cout << Output::toSVG(placement);
	return 0;
}