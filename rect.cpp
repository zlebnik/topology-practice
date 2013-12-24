#include "rect.h"

Rect::Rect(double width, double height, double x, double y) {
	this->width = width;
	this->height = height;
	this->x = x;
	this->y = y;
}
void Rect::update(double x, double y) {
	this->x = x;
	this->y = y;
}
double Rect::getMaxX() const {
	return x + width;
}
double Rect::getMinX() const {
	return x;
}
double Rect::getMaxY() const {
	return y + height;
}
double Rect::getMinY() const {
	return y;
}
double Rect::getWidth() const {
	return width;
}
double Rect::getHeight() const {
	return height;
}