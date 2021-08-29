#include <math.h>
#include "stdafx.h"
#include "EllipseF.h"

IMPLEMENT_SERIAL(EllipseF, CObject, 1)

EllipseF::EllipseF(CPoint p1, CPoint p2)
	:Figure(p1, p2)
{
}
void EllipseF::Draw(CDC* dc) const
{
	CPoint p1 = getP1();
	CPoint p2 = getP2();
	CBrush cb(BGColor);
	cb.CreateSolidBrush(BGColor);
	CPen cp;
	cp.CreatePen(PS_SOLID, PenWidth, SColor);
	dc->SelectObject(cb);
	dc->SelectObject(cp);
	dc->Ellipse(p1.x, p1.y, p2.x, p2.y);
}

bool EllipseF::isInside(const CPoint& P) const {
	// save some time if the point is not even in the bounding rectangle
	if (!Figure::isInside(P)) return false;

	CPoint p1 = getP1();
	CPoint p2 = getP2();
	double centerX = p1.x + (p2.x - p1.x) / 2;
	double centerY = p1.y + (p2.y - p1.y) / 2;
	double r1 = p2.x - p1.x;
	double r2 = p2.y - p1.y;
	double res = ((P.x - centerX) * (P.x - centerX)) + ((P.y - centerY) * (P.y - centerY));
	return res <= (r1 * r1) + (r2 * r2);
}
