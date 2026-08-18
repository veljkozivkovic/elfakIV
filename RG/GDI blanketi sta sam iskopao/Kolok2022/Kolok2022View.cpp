
// Kolok2022View.cpp : implementation of the CKolok2022View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kolok2022.h"
#endif

#include "Kolok2022Doc.h"
#include "Kolok2022View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKolok2022View

IMPLEMENT_DYNCREATE(CKolok2022View, CView)

BEGIN_MESSAGE_MAP(CKolok2022View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKolok2022View construction/destruction

CKolok2022View::CKolok2022View() noexcept
{
	base = new DImage();
	base->Load(CString("base.png"));
	arm1 = new DImage();
	arm1->Load(CString("arm1.png"));
	arm2 = new DImage();
	arm2->Load(CString("arm2.png"));
	head = new DImage();
	head->Load(CString("head.png"));
	pozadina = new DImage();
	pozadina->Load(CString("pozadina.jpg"));
	base_shadow = new DImage();
	base_shadow->Load(CString("base_shadow.png"));
	arm1_shadow = new DImage();
	arm1_shadow->Load(CString("arm1_shadow.png"));
	arm2_shadow = new DImage();
	arm2_shadow->Load(CString("arm2_shadow.png"));
	head_shadow = new DImage();
	head_shadow->Load(CString("head_shadow.png"));


	rArm1 = 0.0f;
	rArm2 = 0.0f;
	rArm3 = 0.0f;
}

CKolok2022View::~CKolok2022View()
{
	delete base;
	delete arm1;
	delete	arm2;
	delete	head;
	delete	pozadina;
	delete	base_shadow;
	delete	arm1_shadow;
	delete	arm2_shadow;
	delete head_shadow;
}

BOOL CKolok2022View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKolok2022View drawing

void CKolok2022View::OnDraw(CDC* pDC)
{
	CKolok2022Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	CBitmap memDcBmp;

	CRect client;
	GetClientRect(client);
	memDcBmp.CreateCompatibleBitmap(pDC, client.Width(), client.Height());

	int gm = memDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldForm;

	memDC->GetWorldTransform(&oldForm);
	CBitmap* oldBMP = memDC->SelectObject(&memDcBmp);
	/*----------------------------------------------------------------------------*/

	DrawBackground(memDC);
	
	DrawLampShadow(memDC);
	DrawLamp(memDC);


	/*----------------------------------------------------------------------------*/

	memDC->SetWorldTransform(&oldForm);
	//DrawGrid(memDC);
	pDC->BitBlt(0, 0, client.Width(), client.Height(), memDC, 0, 0, SRCCOPY);
	memDC->SetGraphicsMode(gm);
	memDC->DeleteDC();
	delete memDC;
	


}


// CKolok2022View printing

BOOL CKolok2022View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKolok2022View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKolok2022View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKolok2022View diagnostics

#ifdef _DEBUG
void CKolok2022View::AssertValid() const
{
	CView::AssertValid();
}

void CKolok2022View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKolok2022Doc* CKolok2022View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKolok2022Doc)));
	return (CKolok2022Doc*)m_pDocument;
}
#endif //_DEBUG


// CKolok2022View message handlers

void CKolok2022View::Translate(CDC* pDC, int dX, int dY, bool rightMultiply)
{
	XFORM forma = {
		1, 0,
		0, 1,
		dX, dY
	};
	pDC->ModifyWorldTransform(&forma, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKolok2022View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM forma = {
		cos(toRad(angle)), sin(toRad(angle)),
		-sin(toRad(angle)), cos(toRad(angle)),
		0, 0
	};
	pDC->ModifyWorldTransform(&forma, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CKolok2022View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM forma = {
		sX, 0,
		0, sY,
		0, 0
	};

	pDC->ModifyWorldTransform(&forma, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CKolok2022View::DrawBackground(CDC* pDC)
{
	CRect clientR;
	GetClientRect(clientR);
	CRect image(0, 0, pozadina->Width(), pozadina->Height());
	pozadina->Draw(pDC, image, clientR);
}

void CKolok2022View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	CBitmap bmpMask;
	CBitmap bmpImage;

	BITMAP bmp;
	pImage->GetBitmap()->GetBitmap(&bmp);

	bmpMask.CreateBitmap(bmp.bmWidth, bmp.bmHeight, 1, 1, NULL);
	bmpImage.CreateBitmap(bmp.bmWidth, bmp.bmHeight, 1, bmp.bmBitsPixel, NULL);

	CDC* srcDC = new CDC();
	CDC* destDC = new CDC();

	srcDC->CreateCompatibleDC(pDC);
	destDC->CreateCompatibleDC(pDC);

	CBitmap* oldSrcBMP = srcDC->SelectObject(&bmpImage);
	CBitmap* oldDestBMP = destDC->SelectObject(&bmpMask);

	CRect pImgRect(0, 0, bmp.bmWidth, bmp.bmHeight);
	pImage->Draw(srcDC, pImgRect, pImgRect);

	COLORREF topLeftClr = srcDC->GetPixel(0, 0);
	COLORREF oldSrcBk = srcDC->SetBkColor(topLeftClr);

	destDC->BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, srcDC, 0, 0, SRCCOPY);

	COLORREF oldSrcFK = srcDC->SetTextColor(RGB(255, 255, 255));
	srcDC->SetBkColor(RGB(0, 0, 0));

	srcDC->BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, destDC, 0, 0, SRCAND);

	pDC->BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, destDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, srcDC, 0, 0, SRCPAINT);

	srcDC->SetBkColor(oldSrcBk);
	srcDC->SetTextColor(oldSrcFK);
	srcDC->SelectObject(&oldSrcBMP);
	destDC->SelectObject(&oldDestBMP);
	srcDC->SetBkColor(oldSrcBk);

	delete srcDC;
	delete destDC;


}

void CKolok2022View::DrawLampBase(CDC* pDC, bool blsShadow)
{
	CRect client;
	GetClientRect(client);
	int w = base->Width();
	int h = base->Height();
	Translate(pDC, 3.8 * w, 5 * h, false);
	if(blsShadow)
		DrawImgTransparent(pDC, base_shadow);
	else
		DrawImgTransparent(pDC, base);


}

void CKolok2022View::DrawLampArm1(CDC* pDC, bool blsShadow)
{
	
	Translate(pDC, 100, -50, false);

	Translate(pDC, 58 , 61 , false);
	Rotate(pDC, -45 + rArm1 , false);
	Translate(pDC, -58 , -61 , false);
	blsShadow ? DrawImgTransparent(pDC, arm1_shadow) : DrawImgTransparent(pDC, arm1);


	
}

void CKolok2022View::DrawLampArm2(CDC* pDC, bool blsShadow)
{
		Translate(pDC, 306, 61, false);
		Translate(pDC, -36, -40, false);
		Translate(pDC, 36, 40, false);
		Rotate(pDC, -90 - rArm2, false);
		Translate(pDC, -36, -40, false);
		blsShadow ? DrawImgTransparent(pDC, arm2_shadow) : DrawImgTransparent(pDC, arm2);
	
	
}

void CKolok2022View::DrawLampHead(CDC* pDC, bool blsShadow)
{
	

		Translate(pDC, 272, 40, false);

		Translate(pDC, -178, -100, false);
		
		Translate(pDC, 178, 100, false);
		Rotate(pDC, 180 + rArm3, false);
		Translate(pDC, -178, -100, false);
		blsShadow ? DrawImgTransparent(pDC, head_shadow) : DrawImgTransparent(pDC, head);

	
}

void CKolok2022View::DrawLamp(CDC* pDC)
{
	XFORM form;
	pDC->GetWorldTransform(&form);
	DrawLampBase(pDC, false);
	DrawLampArm1(pDC, false);
	DrawLampArm2(pDC, false);
	DrawLampHead(pDC, false);
	pDC->SetWorldTransform(&form);

}

void CKolok2022View::DrawLampShadow(CDC* pDC)
{
	XFORM form;
	pDC->GetWorldTransform(&form);

	CRect rekt;
	GetClientRect(rekt);
	Translate(pDC, rekt.Width() / 2 + base->Width() / 6, 3 * rekt.Height() / 4 + 20, false);
	Scale(pDC, 1, 0.25, false);
	Rotate(pDC, -90, false);
	Translate(pDC, -(rekt.Width() / 2 + base->Width() / 2), -(3 * rekt.Height() / 4 + 80), false);

	DrawLampBase(pDC, true);
	DrawLampArm1(pDC, true);
	DrawLampArm2(pDC, true);
	DrawLampHead(pDC, true);
	pDC->SetWorldTransform(&form);

}


void CKolok2022View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	 
	if (nChar == '1')
	{
		rArm1 -= 10;
	}
	if (nChar == '2')
	{
		rArm1 += 10;
	}

	if (nChar == '3')
	{
		rArm2 -= 10;
	}
	if (nChar == '4')
	{
		rArm2 += 10;
	}

	if (nChar == '5')
	{
		rArm3 -= 10;
	}
	if (nChar == '6')
	{
		rArm3 += 10;
	}

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CKolok2022View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
