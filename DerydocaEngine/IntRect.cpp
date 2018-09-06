#include "IntRect.h"

bool IntRect::IsRectOverlapping(IntRect & r1, IntRect & r2)
{
	return r1.getX() < r2.getDx() && r1.getDx() > r2.getX() && r1.getDy() > r2.getY() && r1.getY() < r2.getDy();
}
