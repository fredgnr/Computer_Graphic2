
// Computer_GraphicDoc.cpp : CComputer_GraphicDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Computer_Graphic.h"
#endif

#include "Computer_GraphicDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CComputer_GraphicDoc

IMPLEMENT_DYNCREATE(CComputer_GraphicDoc, CDocument)

BEGIN_MESSAGE_MAP(CComputer_GraphicDoc, CDocument)
END_MESSAGE_MAP()


// CComputer_GraphicDoc 构造/析构

CComputer_GraphicDoc::CComputer_GraphicDoc()
{
	pnum = 0;
}

CComputer_GraphicDoc::~CComputer_GraphicDoc()
{
}

BOOL CComputer_GraphicDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CComputer_GraphicDoc 序列化

void CComputer_GraphicDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CComputer_GraphicDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CComputer_GraphicDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CComputer_GraphicDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CComputer_GraphicDoc 诊断

#ifdef _DEBUG
void CComputer_GraphicDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CComputer_GraphicDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

void change_point(CPoint &point1, CPoint &point2)
{
	CPoint temp;
	temp.x = point1.x, temp.y = point1.y;
	point1.x = point2.x, point1.y = point2.y;
	point2.x = temp.x, point2.y = temp.y;
}

void CComputer_GraphicDoc::DDAline(CClientDC * pDc, CPoint startp, CPoint endp)
{

	if (startp.x != endp.x&&startp.y != endp.y) {
		float m = float(endp.y - startp.y) / float(endp.x - startp.x);
		//斜率绝对值小于1，正常画线
		if (m >= -1 && m <= 1) {
			if (startp.x > endp.x) {
				change_point(startp, endp);
			}
			float y0 = startp.y;
			for (int x = startp.x; x < endp.x; x++) {
				y0 += m;
				pDc->SetPixel(x, round(y0), RGB(255, 0, 0));
			}
		}
		//斜率绝对值大于1，将xy交换
		else {
			if (startp.y > endp.y) {
				change_point(startp, endp);
			}
			m = float(endp.x - startp.x) / float(endp.y - startp.y);
			float x0 = startp.x;
			for (int y = startp.y; y < endp.y; y++) {
				x0 += m;
				pDc->SetPixel(round(x0), y, RGB(255, 0, 0));
			}
		}
	}
	else if (startp.x == endp.x) {
		if (startp.y<endp.y)
			for (int y0 = startp.y; y0 <= endp.y; y0++)
				pDc->SetPixel(startp.x, y0, RGB(255, 0, 0));
		else
			for (int y0 = startp.y; y0 >= endp.y; y0--)
				pDc->SetPixel(startp.x, y0, RGB(255, 0, 0));
	}
	else {
		if (startp.x<endp.x)
			for (int x0 = startp.x; x0 <= endp.x; x0++)
				pDc->SetPixel(x0, startp.y, RGB(255, 0, 0));
		else
			for (int x0 = startp.x; x0 >= endp.x; x0--)
				pDc->SetPixel(x0, startp.y, RGB(255, 0, 0));
	}
}
float distance(CPoint &p1, CPoint& p2)
{
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}
void swap(int &a, int &b)
{
	int c;
	c = a;
	a = b;
	b = c;
}
void CComputer_GraphicDoc::MidLineCC(CClientDC *pDc, CPoint startp, CPoint endp)
{
	int x1 = startp.x, y1 = startp.y, x2 = endp.x, y2 = endp.y;
	if (x1 == x2) {
		if (y1 < y2)
			for (int y0 = y1; y0 <= y2; y0++)
				pDc->SetPixel(x1, y0, RGB(255, 0, 0));
		else
			for (int y0 = y1; y0 >= y2; y0--)
				pDc->SetPixel(x1, y0, RGB(255, 0, 0));
	}
	else if (y1 == y2) {
		if (x1<x2)
			for (int x0 = x1; x0<x2; x0++)
				pDc->SetPixel(x0, y1, RGB(255, 0, 0));
		else
			for (int x0 = x1; x0 <= y2; x0--)
				pDc->SetPixel(x0, y1, RGB(255, 0, 0));
	}
	int number;
	//将坐标系按照每45读划分成一个区域，使用对称的方式，将所有区域的点规划到第一象限的两个区域上
	if ((x2 - x1) > 0 && (y2 - y1) > 0) {
		if ((x2 - x1) > (y2 - y1))
			number = 1;
		else number = 2;
	}
	else if ((x2 - x1) < 0 && (y2 - y1) > 0) {
		if (abs(x2 - x1) > abs(y2 - y1))
			number = 4;
		else number = 3;
	}
	else if ((x2 - x1) < 0 && (y2 - y1) < 0)
	{
		if (abs(x2 - x1) > abs(y2 - y1))
			number = 5;
		else number = 6;
	}
	else {
		if (abs(x2 - x1) > abs(y2 - y1))
			number = 8;
		else number = 7;
	}
	if (number == 1) {
		int a = y1 - y2, b = x2 - x1, c = x1*y2 - x2*y1;
		int d = 2 * a + b;
		a *= 2, b *= 2;//减少后续步骤开销
		int x = x1, y = y1;
		for (; x <= x2;) {
			pDc->SetPixel(x, y, RGB(255, 0, 0));
			if (d > 0) {
				x++;
				d += a;
			}
			else {
				x++, y++;
				d += a + b;
			}
		}
	}
	else if (number == 2) {
		swap(x1, y1);
		swap(x2, y2);
		int a = y1 - y2, b = x2 - x1, c = x1*y2 - x2*y1;
		int d = 2 * a + b;
		a *= 2, b *= 2;//减少后续步骤开销
		int x = x1, y = y1;
		for (; x < x2;) {
			pDc->SetPixel(y, x, RGB(255, 0, 0));
			if (d > 0) {
				x++;
				d += a;
			}
			else {
				x++, y++;
				d += a + b;
			}
		}
	}
	else if (number == 3) {
		x2 = 2 * x1 - x2;
		swap(x1, y1);
		swap(x2, y2);
		int a = y1 - y2, b = x2 - x1, c = x1*y2 - x2*y1;
		int d = 2 * a + b;
		a *= 2, b *= 2;//减少后续步骤开销
		int x = x1, y = y1;
		for (; x < x2;) {
			pDc->SetPixel(2 * y1 - y, x, RGB(255, 0, 0));
			if (d > 0) {
				x++;
				d += a;
			}
			else {
				x++, y++;
				d += a + b;
			}
		}
	}
	else if (number == 4) {
		x2 = 2 * x1 - x2;
		int a = y1 - y2, b = x2 - x1, c = x1*y2 - x2*y1;
		int d = 2 * a + b;
		a *= 2, b *= 2;//减少后续步骤开销
		int x = x1, y = y1;
		for (; x < x2;) {
			pDc->SetPixel(2 * x1 - x, y, RGB(255, 0, 0));
			if (d > 0) {
				x++;
				d += a;
			}
			else {
				x++, y++;
				d += a + b;
			}
		}
	}
	else if (number == 5) {
		x2 = 2 * x1 - x2, y2 = 2 * y1 - y2;
		int a = y1 - y2, b = x2 - x1, c = x1*y2 - x2*y1;
		int d = 2 * a + b;
		a *= 2, b *= 2;//减少后续步骤开销
		int x = x1, y = y1;
		for (; x < x2;) {
			pDc->SetPixel(2 * x1 - x, 2 * y1 - y, RGB(255, 0, 0));
			if (d > 0) {
				x++;
				d += a;
			}
			else {
				x++, y++;
				d += a + b;
			}
		}
	}
	else if (number == 6) {
		x2 = 2 * x1 - x2, y2 = 2 * y1 - y2;
		swap(x1, y1);
		swap(x2, y2);
		int a = y1 - y2, b = x2 - x1, c = x1*y2 - x2*y1;
		int d = 2 * a + b;
		a *= 2, b *= 2;//减少后续步骤开销
		int x = x1, y = y1;
		for (; x < x2;) {
			pDc->SetPixel(2 * y1 - y, 2 * x1 - x, RGB(255, 0, 0));
			if (d > 0) {
				x++;
				d += a;
			}
			else {
				x++, y++;
				d += a + b;
			}
		}
	}
	else if (number == 7) {
		y2 = 2 * y1 - y2;
		swap(x1, y1);
		swap(x2, y2);
		int a = y1 - y2, b = x2 - x1, c = x1*y2 - x2*y1;
		int d = 2 * a + b;
		a *= 2, b *= 2;//减少后续步骤开销
		int x = x1, y = y1;
		for (; x < x2;) {
			pDc->SetPixel(y, 2 * x1 - x, RGB(255, 0, 0));
			if (d > 0) {
				x++;
				d += a;
			}
			else {
				x++, y++;
				d += a + b;
			}
		}
	}
	else {
		y2 = 2 * y1 - y2;
		int a = y1 - y2, b = x2 - x1, c = x1*y2 - x2*y1;
		int d = 2 * a + b;
		a *= 2, b *= 2;//减少后续步骤开销
		int x = x1, y = y1;
		for (; x < x2;) {
			pDc->SetPixel(x, 2 * y1 - y, RGB(255, 0, 0));
			if (d > 0) {
				x++;
				d += a;
			}
			else {
				x++, y++;
				d += a + b;
			}
		}
	}
};
//以对称的方式画出坐标系中对称的8个点
void circle_pixel(CClientDC *pDc, int centerx, int centery, int x, int y)
{
	pDc->SetPixel(centerx + x, centery + y, RGB(255, 0, 0));
	pDc->SetPixel(centerx + y, centery + x, RGB(255, 0, 0));
	pDc->SetPixel(centerx - x, centery + y, RGB(255, 0, 0));
	pDc->SetPixel(centerx - y, centery + x, RGB(255, 0, 0));
	pDc->SetPixel(centerx + x, centery - y, RGB(255, 0, 0));
	pDc->SetPixel(centerx + y, centery - x, RGB(255, 0, 0));
	pDc->SetPixel(centerx - x, centery - y, RGB(255, 0, 0));
	pDc->SetPixel(centerx - y, centery - x, RGB(255, 0, 0));
}
void  CComputer_GraphicDoc::MidCircleCC(CClientDC *pDc, CPoint center, CPoint point_r)
{
	int r = sqrt(pow(center.x - point_r.x, 2) + pow(center.y - point_r.y, 2));
	int xp = 0, yp = round(r);
	int d = 2 * xp - yp + 1;
	while (xp <= yp) {
		circle_pixel(pDc, center.x, center.y, xp, yp);
		if (d > 0) {
			d += 2 * xp - 2 * yp + 5;
			xp++, yp--;
		}
		else {
			d += 2 * xp + 3;
			xp++;
		}
	}
}
void circle_pixel2(CClientDC *pDc, int centerx, int centery, int x, int y)
{
	pDc->SetPixel(centerx + x, centery + y, RGB(255, 0, 0));
	pDc->SetPixel(centerx - x, centery + y, RGB(255, 0, 0));
	pDc->SetPixel(centerx + x, centery - y, RGB(255, 0, 0));
	pDc->SetPixel(centerx - x, centery - y, RGB(255, 0, 0));
}
void  CComputer_GraphicDoc::BCircle(CClientDC *pDc, CPoint center, CPoint point_r)
{
	int centerx = center.x, centery = center.y, r = distance(center, point_r);
	int x0 = 0, y0 = r;
	int d = 1 + pow(y0 - 1, 2) - r*r;
	while (y0 >= 0) {
		circle_pixel2(pDc, centerx, centery, x0, y0);
		if (d == 0) {
			d += 2 * (x0 - y0) + 2;
			x0++, y0--;
		}
		else if (d < 0) {
			if (2 * d + 2 * y0 - 1 < 0) {
				d += 2 * x0 + 1;
				x0++;
			}
			else {
				d += 2 * (x0 - y0) + 2;
				x0++, y0--;
			}
		}
		else {
			if (2 * (d - x0) - 1 > 0) {
				d += -2 * y0 + 1;
				y0--;
			}
			else {
				d += 2 * (x0 - y0) + 2;
				x0++, y0--;
			}
		}
	}
}
void CComputer_GraphicDoc::PNCircle(CClientDC *pDc, CPoint center, CPoint point_r)
{
	int centerx = center.x, centery = center.y, r = distance(center, point_r);
	int x0 = 0, y0 = r;
	int d = 0;
	while (x0 <= y0) {
		circle_pixel(pDc, centerx, centery, x0, y0);
		if (d <= 0) {
			d += 2 * x0 + 1;
			x0++;
		}
		else {
			d += -2 * y0 + 1;
			y0--;
		}
	}
}
// CComputer_GraphicDoc 命令
