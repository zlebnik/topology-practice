#include <sstream>
#include <string>
#include "placement.h"
#include "shape.h"
#include "rect.h"
#include <vector>
#include "output.h"
#include <cstdlib>

using namespace std;

string Output::toSVG(const Placement *placement) {
	stringstream ss;
	ss << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" << endl;
	ss << "<svg version=\"1.1\" baseProfile=\"full\" xmlns=\"http://www.w3.org/2000/svg\">" << endl;
	const vector<Shape*>* shapes = placement->getShapes();
	vector<Rect*>* rects;
	int r, g, b;
	for (int i = 0; i < shapes->size(); i++) {
		r = rand() % 256;
		g = rand() % 256;
		b = rand() % 256;

		rects = shapes->at(i)->getRectangles();
		for (int j = 0; j < rects->size(); j++) {
			ss << "<rect x=\"" << rects->at(j)->getMinX() * 10 << "\" y=\"" << rects->at(j)->getMinY() * 10 << '"';
			ss << " width=\"" << rects->at(j)->getWidth() * 10 << "\" height=\"" << rects->at(j)->getHeight() * 10 << '"';
			ss << " style=\"fill:rgb(" << r << ',' << g << ',' << b << ");stroke-width:1;stroke:rgb(0,0,0)\" />";
			ss << endl;
		}
	}
	ss << "</svg>" << endl;

	return ss.str();
}