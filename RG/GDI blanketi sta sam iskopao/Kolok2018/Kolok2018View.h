#include "DImage.h"
// Kolok2018View.h : interface of the CKolok2018View class
//

#pragma once

#define PI 3.14
#define toRad(x) (x*PI/180)
#define toDeg(x) (x*180/PI)

class CKolok2018View : public CView
{
protected: // create from serialization only
	CKolok2018View() noexcept;
	DECLARE_DYNCREATE(CKolok2018View)

// Attributes
public:
	CKolok2018Doc* GetDocument() const;

	DImage* bager, * arm1, * arm2, * pozadina;
	HENHMETAFILE viljuska;

	bool drawGrid;
	int gridSize;
	float xPomeraj;
	float rArm1;
	float rArm2;
	float rViljuska;
// Operations
public:
	void Translate(CDC* pDC, int dX, int dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawBackGround(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawGrid(CDC* pDC);
	// s leva na desno
	void DrawBody(CDC* pDC);
	void DrawArm1(CDC* pDC);
	void DrawArm2(CDC* pDC);
	void DrawFork(CDC* pDC);

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
	virtual ~CKolok2018View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in Kolok2018View.cpp
inline CKolok2018Doc* CKolok2018View::GetDocument() const
   { return reinterpret_cast<CKolok2018Doc*>(m_pDocument); }
#endif

