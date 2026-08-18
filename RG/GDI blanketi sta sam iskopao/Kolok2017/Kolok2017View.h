#include "DImage.h"	
// Kolok2017View.h : interface of the CKolok2017View class
//

#pragma once
#define PI 3.14
#define toRad(x) (x*PI/180)
#define toDeg(x) (x * 180/PI)

class CKolok2017View : public CView
{
protected: // create from serialization only
	CKolok2017View() noexcept;
	DECLARE_DYNCREATE(CKolok2017View)

// Attributes
public:
	CKolok2017Doc* GetDocument() const;


	DImage* body, * leg1, * leg2, * leg3, * back2;

	float rJedan;
	float rDva;
	float rTri;
	float rCetri;

	float right, scale;

	float pozadinaX, pozadinaY;

	float maxPozadinaX, maxPozadinaY;

// Operations
public:

	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawBackground(CDC* pDC);

	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawScene(CDC* pDC, CRect rect);
	void DrawLeg(CDC* pDC, double alpha, double dx, double dy);

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
	virtual ~CKolok2017View();
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

#ifndef _DEBUG  // debug version in Kolok2017View.cpp
inline CKolok2017Doc* CKolok2017View::GetDocument() const
   { return reinterpret_cast<CKolok2017Doc*>(m_pDocument); }
#endif

