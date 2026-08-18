
// Kolok2017View.cpp : implementation of the CKolok2017View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kolok2017.h"
#endif

#include "Kolok2017Doc.h"
#include "Kolok2017View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKolok2017View

IMPLEMENT_DYNCREATE(CKolok2017View, CView)

BEGIN_MESSAGE_MAP(CKolok2017View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKolok2017View construction/destruction

CKolok2017View::CKolok2017View() noexcept
{

	body = new DImage();
	body->Load(CString("Body.png"));
	leg1 = new DImage();
	leg1->Load(CString("Leg1.png"));
	leg2 = new DImage();
	leg2->Load(CString("Leg2.png"));

	leg3 = new DImage();
	leg3->Load(CString("Leg3.png"));
	back2 = new DImage();
	back2->Load(CString("Back2.jpg"));


	this->right = 0.0f;
	this->scale = 0.0f;
	pozadinaX = 0.0f; 
	pozadinaY = 0.0f;

	rJedan = 0.0f;
	rDva = 0.0f;
	rTri = -20.0f;
	rCetri = 20.0f;

	maxPozadinaX = 600.0f;
	maxPozadinaY = 600.0f;

}

CKolok2017View::~CKolok2017View()
{
	delete body;
	delete leg1;
	delete leg2;
	delete leg3;
	delete back2;

	


}

BOOL CKolok2017View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKolok2017View drawing

void CKolok2017View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	CBitmap bmpImage, bmpMask;

	BITMAP bmp;

	pImage->GetBitmap()->GetBitmap(&bmp);


	bmpImage.CreateBitmap(bmp.bmWidth, bmp.bmHeight, 1, bmp.bmBitsPixel, NULL);
	bmpMask.CreateBitmap(bmp.bmWidth, bmp.bmHeight, 1, 1, NULL);

	CDC* srcDC = new CDC();
	CDC* dstDC = new CDC();

	srcDC->CreateCompatibleDC(pDC);
	dstDC->CreateCompatibleDC(pDC);

	CBitmap* oldSrcBMP = srcDC->SelectObject(&bmpImage);
	CBitmap* oldDstBMP = dstDC->SelectObject(&bmpMask);

	CRect rekt(0, 0, bmp.bmWidth, bmp.bmHeight);
	pImage->Draw(srcDC, rekt, rekt);

	COLORREF topLeftClr = srcDC->GetPixel(2, 2);
	COLORREF OldsrcBkClr = srcDC->SetBkColor(topLeftClr);

	dstDC->BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, srcDC, 0, 0, SRCCOPY);


	COLORREF oldFkClr = srcDC->SetTextColor(RGB(255, 255, 255));
	srcDC->SetBkColor(RGB(0, 0, 0));

	srcDC->BitBlt(0, 0, bmp.bmWidth, bmp.bmWidth, dstDC, 0, 0, SRCAND);


	pDC->BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, dstDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, srcDC, 0, 0, SRCPAINT);


	srcDC->SetBkColor(OldsrcBkClr);
	srcDC->SetTextColor(oldFkClr);


	dstDC->SelectObject(oldDstBMP);
	srcDC->SelectObject(oldSrcBMP);



	srcDC->DeleteDC();
	dstDC->DeleteDC();
	delete srcDC;
	delete dstDC;





}

void CKolok2017View::DrawBackground(CDC* pDC)
{
	XFORM form;

	

	CRect img(0 + maxPozadinaX + pozadinaX, 0 + maxPozadinaY + pozadinaY, back2->Width() - maxPozadinaX + pozadinaX, back2->Height() - maxPozadinaY + pozadinaY);

	CRect client;
	GetClientRect(client);
	back2->Draw(pDC,img	 ,client );


	
}

void CKolok2017View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM form = {
		1, 0,
		0, 1,
		dX, dY
	};
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKolok2017View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM form = {
		cos(toRad(angle)), sin(toRad(angle)),
		-sin(toRad(angle)), cos(toRad(angle)),
		0, 0
	};
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKolok2017View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM form = {
		sX, 0,
		0, sY,
		0, 0
	};
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKolok2017View::DrawScene(CDC* pDC, CRect client)
{
	
	DrawBackground(pDC);

	Scale(pDC, 1 + scale, 1 + scale, false);

	Translate(pDC, 100, 100, false);

	DrawImgTransparent(pDC, body);
	
	
	

	

	DrawLeg(pDC,0 + rJedan,72, 168);
	DrawLeg(pDC, 0 + rDva, 72, 168);

	float dodatak = 20;
	
	if (rJedan + dodatak >= -20 )
	{
		dodatak = -20;
	}
	DrawLeg(pDC,  rTri , 280, 168);
	DrawLeg(pDC, rCetri, 280, 168);

	

}

void CKolok2017View::DrawLeg(CDC* pDC, double alpha, double dx, double dy)
{
	XFORM form;
	pDC->GetWorldTransform(&form);
	
	Translate(pDC, dx, dy, false);
	//Translate(pDC, -20, -10, false);

	Translate(pDC, 28, 28, false);
	Rotate(pDC, alpha, false);
	Translate(pDC, -28, -28, false);

	DrawImgTransparent(pDC, leg1);
	
	
	
	Translate(pDC, 0, 4 * leg1->Height() / 5 - 10, false);
	
	Translate(pDC, 29, 29, false);
	Rotate(pDC, -alpha, false);
	Translate(pDC, -29, -29, false);

	DrawImgTransparent(pDC, leg2);

	

	Translate(pDC, -leg3->Width() / 3 + 10, leg2->Height(), false);
	


	DrawImgTransparent(pDC, leg3);

	pDC->SetWorldTransform(&form);

}

void CKolok2017View::OnDraw(CDC* pDC)
{
	CKolok2017Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	CRect client;
	GetClientRect(client);

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	CBitmap memDcBMP;

	memDcBMP.CreateCompatibleBitmap(pDC, client.Width(), client.Height());
	CBitmap* OldMemDcBMP = memDC->SelectObject(&memDcBMP);
	
	int gm = memDC->SetGraphicsMode(GM_ADVANCED);
	XFORM form;
	memDC->GetWorldTransform(&form);

	/*------------------------------------------------------------------00*/
	

	DrawScene(memDC, client);

	/*------------------------------------------------------------------*/
	memDC->SetWorldTransform(&form);
	memDC->SetGraphicsMode(gm);
	pDC->BitBlt(0, 0, client.Width(), client.Height(), memDC, 0, 0, SRCCOPY);
	
	memDC->SelectObject(OldMemDcBMP);
	memDC->DeleteDC();
	delete memDC;


}


// CKolok2017View printing

BOOL CKolok2017View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKolok2017View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKolok2017View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKolok2017View diagnostics

#ifdef _DEBUG
void CKolok2017View::AssertValid() const
{
	CView::AssertValid();
}

void CKolok2017View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKolok2017Doc* CKolok2017View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKolok2017Doc)));
	return (CKolok2017Doc*)m_pDocument;
}
#endif //_DEBUG


// CKolok2017View message handlers


void CKolok2017View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CRect client;
	GetClientRect(client);
	int w = back2->Width();
	int h = back2->Height();
	if (nChar == VK_DOWN)
	{
		this->scale -= 0.1;

	}
	if (nChar == VK_UP)
	{
		this->scale += 0.1;

	}
	if (nChar == VK_RIGHT)
	{
		rJedan += 10.0f;
		if (rJedan >= 20.f)
			rJedan = 20.0f;

		rDva -= 10.0f;
		if (rDva <= -20.0f)
			rDva = -20.0f;

		rTri += 10.0f;
		if (rTri >= 0)
			rTri = 0;

		rCetri -= 10.0f;
		if (rCetri <= 0)
			rCetri = 0;
	}
	if (nChar == VK_LEFT)
	{
		rJedan -= 10.0f;
		if (rJedan <= 0)
			rJedan = 0;

		rDva += 10.0f;
		if (rDva >= 0)
			rDva = 0;

		rTri -= 10.0f;
		if (rTri <= -20.0f)
			rTri = -20.0f;

		rCetri += 10.0f;
		if (rCetri >= 20.f)
			rCetri = 20.0f;
	}

	if (nChar == 'W')
	{
		
		this->pozadinaY -= 200;
		if (pozadinaY <= -maxPozadinaY)
			pozadinaY = -maxPozadinaY;
	}
	if (nChar == 'S')
	{
		
		this->pozadinaY += 200;
		if (pozadinaY >= maxPozadinaY)
			pozadinaY = maxPozadinaY;
	}
	if (nChar == 'D')
	{
		
		this->pozadinaX += 200;
		if (pozadinaX >= maxPozadinaX)
			pozadinaX = maxPozadinaX;

	}
	if (nChar == 'A')
	{
		this->pozadinaX -= 200;
		if (pozadinaX <= -maxPozadinaX)
			pozadinaX = -maxPozadinaX;
	}

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CKolok2017View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
