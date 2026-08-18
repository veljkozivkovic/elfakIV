#include "Dimage.h"
// Kolok2023View.h : interface of the CKolok2023View class
//

#pragma once

#define PI 3.14
#define toRad(x) (x * PI / 180)
#define toDeg(x) (x * 180/PI)

class CKolok2023View : public CView
{
protected: // create from serialization only
	CKolok2023View() noexcept;
	DECLARE_DYNCREATE(CKolok2023View)

// Attributes
public:
	CKolok2023Doc* GetDocument() const;


	DImage* glava;
	DImage* nadkolenica;
	DImage* nadlaktica;
	DImage* podkolenica;
	DImage* podlaktica;
	DImage* saka;
	DImage* stopalo;
	DImage* telo;
	DImage* pozadina;


	float rNadlaktica, rPodlaktica, rSaka, rRobot, scaleRobot;


// Operations
public:

	void DrawBackground(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);

	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);

	void DrawHalf(CDC* pDC);

	void DrawHead(CDC* pDC);


	void DrawRobot(CDC* pDC);

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
	virtual ~CKolok2023View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in Kolok2023View.cpp
inline CKolok2023Doc* CKolok2023View::GetDocument() const
   { return reinterpret_cast<CKolok2023Doc*>(m_pDocument); }
#endif

