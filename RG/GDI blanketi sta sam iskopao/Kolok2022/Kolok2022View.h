#include "DImage.h"
// Kolok2022View.h : interface of the CKolok2022View class
//

#pragma once

#define PI 3.14
#define toRad(x) (x * PI / 180)
#define toDeg(x) (x * 180 /PI)

class CKolok2022View : public CView
{
protected: // create from serialization only
	CKolok2022View() noexcept;
	DECLARE_DYNCREATE(CKolok2022View)

// Attributes
public:
	CKolok2022Doc* GetDocument() const;


	DImage* base;
	DImage* arm1; 
	DImage* arm2;
	DImage* head;
	DImage* pozadina;
	DImage* base_shadow;
	DImage* arm1_shadow;
	DImage* arm2_shadow;
	DImage* head_shadow;

	float rArm1;
	float rArm2;
	float rArm3;

// Operations
public:

	void Translate(CDC* pDC, int dX, int dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);

	void DrawBackground(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	
	
	void DrawLampBase(CDC* pDC, bool blsShadow);
	void DrawLampArm1(CDC* pDC, bool blsShadow);
	void DrawLampArm2(CDC* pDC, bool blsShadow);
	void DrawLampHead(CDC* pDC, bool blsShadow);

	void DrawLamp(CDC* pDC);

	void DrawLampShadow(CDC* pDC);







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
	virtual ~CKolok2022View();
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

#ifndef _DEBUG  // debug version in Kolok2022View.cpp
inline CKolok2022Doc* CKolok2022View::GetDocument() const
   { return reinterpret_cast<CKolok2022Doc*>(m_pDocument); }
#endif

