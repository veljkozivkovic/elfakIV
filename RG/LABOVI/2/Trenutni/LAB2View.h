
// LAB2View.h : interface of the CLAB2View class
//

#pragma once


class CLAB2View : public CView
{
protected: // create from serialization only
	CLAB2View() noexcept;
	DECLARE_DYNCREATE(CLAB2View)

// Attributes
public:
	CLAB2Doc* GetDocument() const;
	bool keyPressed;
	int gridSize;
	HENHMETAFILE emf_cactus;
	HENHMETAFILE emf_cactus_light;
	int partAngle;
	int wholeAngle;

// Operations
public:
	void SetBackgroundColor500x500(CDC* pDC);
	void DrawGrid(CDC* pDC);
	void Translate(CDC* pDC, float dX, float dY);
	void Scale(CDC* pDC, float sX, float sY);
	void Rotate(CDC* pDC, float angle);
	void DrawPot(CDC* pDC);
	void DrawCactus(CDC* pDC, float scalex, float scaley, POINT translate, int selfRotationAngle, HENHMETAFILE mf);
	void DrawCircle(CDC* pDC, POINT point, int r, COLORREF color, COLORREF fill);
	void RotateAroundPoint(CDC* pDC, POINT point, float angle);
	void DrawFigures(CDC* pDC);
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CLAB2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in LAB2View.cpp
inline CLAB2Doc* CLAB2View::GetDocument() const
   { return reinterpret_cast<CLAB2Doc*>(m_pDocument); }
#endif

