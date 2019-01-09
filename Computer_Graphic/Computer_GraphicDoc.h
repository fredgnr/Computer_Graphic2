
// Computer_GraphicDoc.h : CComputer_GraphicDoc ��Ľӿ�
//


#pragma once


class CComputer_GraphicDoc : public CDocument
{
protected: // �������л�����
	CComputer_GraphicDoc();
	DECLARE_DYNCREATE(CComputer_GraphicDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CComputer_GraphicDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	CPoint pointgroup[1000];
	void DDAline(CClientDC * pDc, CPoint startp, CPoint endp);
	void MidCircleCC(CClientDC *pDc, CPoint center, CPoint point_r);
	void MidLineCC(CClientDC *pDc, CPoint startp, CPoint endp);
	void BCircle(CClientDC *pDc, CPoint startp, CPoint endp);
	void PNCircle(CClientDC *pDc, CPoint startp, CPoint endp);
	CPoint Points[1000];//���bezier�Լ�B�������ߵĿ��Ƶ�
	int pnum;//beizer�Լ�B�������ߵĿ��Ƶ�����
};
