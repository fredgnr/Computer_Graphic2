
// Computer_GraphicView.cpp : CComputer_GraphicView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "Computer_Graphic.h"
#endif

#include "Computer_GraphicDoc.h"
#include "Computer_GraphicView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CComputer_GraphicView
extern  CStatusBar m_wndStatusBar;
IMPLEMENT_DYNCREATE(CComputer_GraphicView, CView)

BEGIN_MESSAGE_MAP(CComputer_GraphicView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_DRAW_DDALINE, &CComputer_GraphicView::OnDrawDdaline)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_DRAW_MIDLINE, &CComputer_GraphicView::OnDrawMidline)
	ON_COMMAND(ID_DRAW_MIDCIRCLE, &CComputer_GraphicView::OnDrawMidcircle)
	ON_COMMAND(ID_DRAW_BCIRCLE, &CComputer_GraphicView::OnDrawBcircle)
	ON_COMMAND(ID_DRAW_PNCIRCLE, &CComputer_GraphicView::OnDrawPncircle)
	ON_COMMAND(ID_CURVE_BEZIER, &CComputer_GraphicView::OnCurveBezier)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CComputer_GraphicView ����/����

CComputer_GraphicView::CComputer_GraphicView()
{
	// TODO: �ڴ˴���ӹ������
	Press_num = 0;
	Menu_ID = 0;
	saved_number = -1;
	control = false;
}

CComputer_GraphicView::~CComputer_GraphicView()
{
}

BOOL CComputer_GraphicView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CComputer_GraphicView ����

void CComputer_GraphicView::OnDraw(CDC* /*pDC*/)
{
	CComputer_GraphicDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CComputer_GraphicView ��ӡ

BOOL CComputer_GraphicView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CComputer_GraphicView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CComputer_GraphicView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CComputer_GraphicView ���

#ifdef _DEBUG
void CComputer_GraphicView::AssertValid() const
{
	CView::AssertValid();
}

void CComputer_GraphicView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CComputer_GraphicDoc* CComputer_GraphicView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CComputer_GraphicDoc)));
	return (CComputer_GraphicDoc*)m_pDocument;
}
#endif //_DEBUG


// CComputer_GraphicView ��Ϣ�������


void CComputer_GraphicView::OnDrawDdaline()
{
	Menu_ID = 1; Press_num = 0;
}


void CComputer_GraphicView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CComputer_GraphicDoc *pDoc = GetDocument();
	CClientDC pDC(this);
	if (Menu_ID >= 1 && Menu_ID <= 5) {
		if (Press_num == 0) {
			pDoc->pointgroup[Press_num++] = point;
			SetCapture();
		}
		else {
			pDoc->pointgroup[Press_num] = point;
			Press_num = 0;
			if (Menu_ID == 1)
				pDoc->DDAline(&pDC, pDoc->pointgroup[0], pDoc->pointgroup[1]);
			else if (Menu_ID == 2)
				pDoc->MidLineCC(&pDC, pDoc->pointgroup[0], pDoc->pointgroup[1]);
			else if (Menu_ID == 3)
				pDoc->MidCircleCC(&pDC, pDoc->pointgroup[0], pDoc->pointgroup[1]);
			else if (Menu_ID == 4)
				pDoc->BCircle(&pDC, pDoc->pointgroup[0], pDoc->pointgroup[1]);
			else if (Menu_ID == 5)
				pDoc->PNCircle(&pDC, pDoc->pointgroup[0], pDoc->pointgroup[1]);
			ReleaseCapture();
		}
		mPointOrign = point; mPointOld = point;//��¼��һ��
	}
	else if(Menu_ID==6){
		OnPrepareDC(&pDC);
		pDC.DPtoLP(&point);
		pDC.SetROP2(R2_NOT);
		pDC.MoveTo(point.x + 5, point.y + 5);
		pDC.LineTo(point.x - 5, point.y - 5);
		pDC.MoveTo(point.x - 5, point.y + 5);
		pDC.LineTo(point.x + 5, point.y - 5);
		pDoc->Points[pDoc->pnum++] = point;
		if (Press_num == 0) {
			pDoc->pointgroup[0] = point;
			Press_num = 1;
			SetCapture();
		}
		else {
			pDC.MoveTo(pDoc->Points[pDoc->pnum-1]),pDC.LineTo(point);
			ReleaseCapture();
		}
		mPointOrign = point; mPointOld = point;//��¼��һ��
	}
	else if (Menu_ID == 7) {
		for (int i = 0; i < pDoc->pnum; i++) {
			if (point.x <= pDoc->Points[i].x + 5 && point.x >= pDoc->Points[i].x - 5 && point.y >= pDoc->Points[i].y - 5 && point.y <= pDoc->Points[i].y + 5) {
				saved_number = i;
			}
		}
		if (saved_number >0) {
			if (control) {
				OnPrepareDC(&pDC);
				pDC.DPtoLP(&point);
				pDC.SetROP2(R2_NOT);
				pDC.MoveTo(pDoc->Points[saved_number - 1]);pDC.LineTo(pDoc->Points[saved_number]);
				pDC.MoveTo(pDoc->Points[saved_number - 1]);pDC.LineTo(point);
				pDC.MoveTo(pDoc->Points[saved_number ]);pDC.LineTo(point);
				control = false;
				mPointOld = point;
			}
			for (int i = pDoc->pnum-1;i>=saved_number; i--) {
				pDoc->Points[i + 1] = pDoc->Points[i];
			}
			pDoc->pnum++;
			pDoc->Points[saved_number] = point;
		}
	}
	CView::OnLButtonDown(nFlags, point);
}


void CComputer_GraphicView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CComputer_GraphicDoc *pDoc = GetDocument();
	CClientDC pDC(this);
	OnPrepareDC(&pDC);
	pDC.DPtoLP(&point);
	pDC.SetROP2(R2_NOT);
	int xx, yy;
	char p1[20];
	xx = point.x;	yy = point.y;					//ȡ��������Ϣ
	sprintf(p1, "%4d", xx);						//ת��Ϊ�ַ���
	m_wndStatusBar.SetPaneText(2, LPCTSTR(p1), TRUE);	//�ڵ�2��������ʾx����
	sprintf(p1, "%4d", yy);						//ת��Ϊ�ַ���
	m_wndStatusBar.SetPaneText(3, LPCTSTR(p1), TRUE);	//�ڵ�3��������ʾy����
	CView::OnMouseMove(nFlags, point);
	if (Menu_ID >= 1 && Menu_ID <= 5) {
		if (mPointOld != point&&Press_num == 1) {
			if (Menu_ID == 1) {
				pDoc->DDAline(&pDC, mPointOrign, mPointOld);//������
				pDoc->DDAline(&pDC, mPointOrign, point);//������
			}
			else if (Menu_ID == 2) {
				pDoc->MidLineCC(&pDC, mPointOrign, mPointOld);//������
				pDoc->MidLineCC(&pDC, mPointOrign, point);//������
			}
			else if (Menu_ID == 3) {
				pDoc->MidCircleCC(&pDC, mPointOrign, mPointOld);
				pDoc->MidCircleCC(&pDC, mPointOrign, point);
			}
			else if (Menu_ID == 4) {
				pDoc->BCircle(&pDC, mPointOrign, mPointOld);
				pDoc->BCircle(&pDC, mPointOrign, point);
			}
			else if (Menu_ID == 5) {
				pDoc->PNCircle(&pDC, mPointOrign, mPointOld);
				pDoc->PNCircle(&pDC, mPointOrign, point);
			}
			mPointOld = point;
		}
	}
	else if (Menu_ID == 6) {
		if (mPointOld != point&&(Press_num == 1||pDoc->pnum>=1)) {
			pDC.MoveTo(mPointOrign),pDC.LineTo(mPointOld);//������
			pDC.MoveTo(mPointOrign), pDC.LineTo(point);
			mPointOld = point;
		}
	}
	else if (Menu_ID == 7&&saved_number>=0) {
		//������
		pDC.MoveTo(pDoc->Points[saved_number - 1]),pDC.LineTo(mPointOld);
		pDC.MoveTo(pDoc->Points[saved_number + 1]), pDC.LineTo(mPointOld);
		pDC.MoveTo(pDoc->Points[saved_number - 1]), pDC.LineTo(point);
		pDC.MoveTo(pDoc->Points[saved_number + 1]), pDC.LineTo(point);
		mPointOld = point;
	}
}


void CComputer_GraphicView::OnDrawMidline()
{
	Menu_ID = 2, Press_num = 0;
}

void CComputer_GraphicView::OnDrawMidcircle()
{
	Menu_ID = 3, Press_num = 0;
}

void CComputer_GraphicView::OnDrawBcircle()
{
	Menu_ID = 4, Press_num = 0;
}

void CComputer_GraphicView::OnDrawPncircle()
{
	Menu_ID = 5, Press_num = 0;

}

void CComputer_GraphicView::OnCurveBezier()
{
	Menu_ID = 6, Press_num = 0;
	GetDocument()->pnum = 0;
}


void CComputer_GraphicView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CComputer_GraphicDoc *pDoc = GetDocument();
	if (Menu_ID == 6) {
		CClientDC pDC(this);
		OnPrepareDC(&pDC);
		pDC.DPtoLP(&point);
		pDC.SetROP2(R2_NOT);
		pDC.MoveTo(pDoc->Points[pDoc->pnum - 1]),pDC.LineTo(point);
		Menu_ID = 7;
		control = true;
	}
	else if (Menu_ID == 7) {
		pDoc->Points[saved_number] = point;
		CClientDC pDC(this);
		OnPrepareDC(&pDC);
		pDC.DPtoLP(&point);
		pDC.SetROP2(R2_NOT);
		pDC.MoveTo(point.x + 5, point.y + 5);
		pDC.LineTo(point.x - 5, point.y - 5);
		pDC.MoveTo(point.x - 5, point.y + 5);
		pDC.LineTo(point.x + 5, point.y - 5);
		control = true;
		saved_number = -1;
	}
	CView::OnRButtonDown(nFlags, point);
}
