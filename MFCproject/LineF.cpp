#include "stdafx.h"
#include "LineF.h"

IMPLEMENT_SERIAL(LineF, CObject, 1)

LineF::LineF(CPoint p1, CPoint p2) : Figure(p1, p2)
{

}

bool LineF::isInside(const CPoint& P) const {
	// skip calculating if not even in the bounding rectangle
	if (!Figure::isInside(P)) return false;

	CPoint p1 = getP1();
	CPoint p2 = getP2();
	double d = abs((p2.x - p1.x) * (p1.y - P.y) - (p1.x - P.x) * (p2.y - p1.y)) /
		sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
	return d < 5; // allow some deviation
}

void LineF::Draw(CDC* dc) const
{
	CPoint p1 = getP1();
	CPoint p2 = getP2();
	CPen cp;
	cp.CreatePen(PS_SOLID, PenWidth, SColor);
	dc->SelectObject(cp);
	dc->MoveTo(p1.x, p1.y);
	dc->LineTo(p2.x, p2.y);
}
