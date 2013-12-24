#include "functions.h"
#include "placement.h"
#include "rectshape.h"
#include "output.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
	Placement *placement = new Placement();
	RectShape *rect = new RectShape(2, 2, 2, 2);
	RectShape *rect2 = new RectShape(1, 3, 1, 1);
	placement->addShape(rect);
	placement->addShape(rect2);

	Functions *functions = new Functions(.1);
	
	cout << functions->getSmoothArea(placement) << endl;
	cout << functions->getSmoothOverlap(placement) << endl;
	
	
	delete placement;
	delete rect;
	delete rect2;

	return 0;
}