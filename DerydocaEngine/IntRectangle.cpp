#include "IntRectangle.h"

bool IntRectangle::IsRectOverlapping(IntRectangle r1, IntRectangle r2)
{
	int r1l = r1.getX();
	int r1r = r1.getX() + r1.getWidth();
	int r1t = r1.getY() + r1.getHeight();
	int r1b = r1.getY();

	int r2l = r2.getX();
	int r2r = r2.getX() + r2.getWidth();
	int r2t = r2.getY() + r2.getHeight();
	int r2b = r2.getY();

	return r1l < r2r && r1r > r2l && r1t > r2b && r1b < r2t;
}

bool IntRectangle::IsPointInRect(IntRectangle rect, int x, int y)
{
	if (x < rect.getX() || y < rect.getY() || x > rect.getX() + rect.getWidth() || y > rect.getHeight() + rect.getY())
	{
		return false;
	}
	return true;
}
