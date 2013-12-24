#ifndef RECT_H
class Rect {
public:
	double getMinX() const;
	double getMinY() const;
	double getMaxX() const;
	double getMaxY() const;
	double getWidth() const;
	double getHeight() const;
	void update(const double x, const double y);
	//void flip(const angle);
	Rect(double width, double height, double x, double y);
private:
	double x;
	double y;
	double width;
	double height;
};
#define RECT_H
#endif