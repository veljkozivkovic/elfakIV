
// Kolok2023View.cpp : implementation of the CKolok2023View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kolok2023.h"
#endif

#include "Kolok2023Doc.h"
#include "Kolok2023View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKolok2023View

IMPLEMENT_DYNCREATE(CKolok2023View, CView)

BEGIN_MESSAGE_MAP(CKolok2023View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKolok2023View construction/destruction

CKolok2023View::CKolok2023View() noexcept
{
	 glava = new DImage();
	 glava->Load(CString("glava.png"));

	 nadkolenica = new DImage();
	 nadkolenica->Load(CString("nadkolenica.png"));

	 nadlaktica = new DImage();
	 nadlaktica->Load(CString("nadlaktica.png"));

	 podkolenica = new DImage();
	 podkolenica->Load(CString("podkolenica.png"));

	 podlaktica = new DImage();
	 podlaktica->Load(CString("podlaktica.png"));
	 
	 saka = new DImage();
	 saka->Load(CString("saka.png"));

	 stopalo = new DImage();
	 stopalo->Load(CString("stopalo.png"));

	 telo = new DImage();
	 telo->Load(CString("telo.png"));

	 pozadina = new DImage();
	 pozadina->Load(CString("pozadina.jpg"));

	 rNadlaktica = 0.0f;
	 rPodlaktica = 0.0f;
	 rSaka = 0.0f;
	 rRobot = 0.0f;
	 scaleRobot = 0.0f;;

}

CKolok2023View::~CKolok2023View()
{
	delete glava;
	delete nadkolenica;
	delete nadlaktica;
	delete	podkolenica;
	delete	podlaktica;
	delete	saka;
	delete	stopalo;
	delete	telo;
	delete	pozadina;


}

BOOL CKolok2023View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKolok2023View drawing

void CKolok2023View::DrawBackground(CDC* pDC)
{
	CRect Rpozadina(0, 0, pozadina->Width(), pozadina->Height());
	CRect client;
	GetClientRect(client);
	pozadina->Draw(pDC, Rpozadina, client);

}

void CKolok2023View::DrawImgTransparent(CDC* pDC, DImage* pImage)
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

void CKolok2023View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM form = {
		1, 0,
		0, 1, 
		dX, dY
	};
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKolok2023View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM form = {
		cos(toRad(angle)), sin(toRad(angle)),
		-sin(toRad(angle)), cos(toRad(angle)),
		0, 0
	};
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKolok2023View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM form = {
		sX, 0,
		0, sY,
		0, 0
	};
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKolok2023View::DrawHalf(CDC* pDC)
{
	

	CRect memRect;
	GetClientRect(memRect);
	Translate(pDC, memRect.Width() / 2 - telo->Width()  , memRect.Height() / 2 - telo->Height() , false);
	DrawImgTransparent(pDC, telo);


	XFORM form;
	pDC->GetWorldTransform(&form);



	Translate(pDC, 25, 65, false);


	// mora u minus da se centrira podlaktica
	Translate(pDC, -35, -35, false);
	Translate(pDC, 35, 35, false);
	Rotate(pDC, this->rNadlaktica, false);
	Translate(pDC, -35, -35, false);

	DrawImgTransparent(pDC, nadlaktica);


	Translate(pDC, 22, 167, false);

	Translate(pDC, -30, -33, false);

	Translate(pDC, 33, 33, false);
	Rotate(pDC, this->rPodlaktica, false);
	Translate(pDC, -33, -33, false);

	DrawImgTransparent(pDC, podlaktica);

	Translate(pDC, 30, 140, false);

	Translate(pDC, -25, -3, false);

	Translate(pDC, 25, 3, false);
	Rotate(pDC, this->rSaka, false);
	Translate(pDC, -25, -3, false);

	DrawImgTransparent(pDC, saka);


	//vracam se na telo
	
	pDC->SetWorldTransform(&form);

	Translate(pDC, 61, 262, false);

	Translate(pDC, -29, -29, false);
	DrawImgTransparent(pDC, nadkolenica);
	
	
	Translate(pDC, 30, 184, false);

	Translate(pDC, -25, -37, false);
	DrawImgTransparent(pDC, podkolenica);
	

	Translate(pDC, 25, 248, false);

	Translate(pDC, -20, -16, false);
	DrawImgTransparent(pDC, stopalo);
	

	pDC->SetWorldTransform(&form);

	Translate(pDC, telo->Width() / 2 , -glava->Height() + 20, false);
	//DrawHead(pDC);

	

}

void CKolok2023View::DrawHead(CDC* pDC)
{
	DrawImgTransparent(pDC, glava);
}

void CKolok2023View::DrawRobot(CDC* pDC)
{
	CRect memRect;
	GetClientRect(memRect);

	Translate(pDC, memRect.Width() / 2, memRect.Height() / 2, false);
	Rotate(pDC, rRobot, false);
	Scale(pDC, 1 + scaleRobot, 1 + scaleRobot, false);
	Translate(pDC, - memRect.Width() / 2,- memRect.Height() / 2, false);


	XFORM form;
	pDC->GetWorldTransform(&form);
	DrawHalf(pDC);
	DrawHead(pDC);
	pDC->SetWorldTransform(&form);



	Translate(pDC, memRect.Width() / 2  -5, 0, false);
	Scale(pDC, -1, 1, false);
	Translate(pDC, -memRect.Width() / 2 + 5, 0, false);
	DrawHalf(pDC);
}

void CKolok2023View::OnDraw(CDC* pDC)
{
	CKolok2023Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);

	CRect client;
	GetClientRect(client);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, client.Width(), client.Height());



	XFORM forma;
	int gm = memDC->SetGraphicsMode(GM_ADVANCED);
	CBitmap* oldmemDCbmp = memDC->SelectObject(&bitmap);
	memDC->GetWorldTransform(&forma);
	/*------------------------------------------------------------------------*/

	DrawBackground(memDC);

	CRect memRect;
	GetClientRect(memRect);
	
	DrawRobot(memDC);

	//Translate(memDC, memRect.Width() / 2, memRect.Height() / 2, false);
	//memDC->Ellipse(-5, -5, 5, 5);
	//Translate(memDC, -memRect.Width() / 2, -memRect.Height() / 2, false);
	
	/*------------------------------------------------------------------------*/
	memDC->SetWorldTransform(&forma);
	pDC->BitBlt(0, 0, client.Width(), client.Height(), memDC, 0, 0, SRCCOPY);
	memDC->SetGraphicsMode(gm);

	memDC->DeleteDC();
	delete memDC;


	
}


// CKolok2023View printing

BOOL CKolok2023View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKolok2023View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKolok2023View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKolok2023View diagnostics

#ifdef _DEBUG
void CKolok2023View::AssertValid() const
{
	CView::AssertValid();
}

void CKolok2023View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKolok2023Doc* CKolok2023View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKolok2023Doc)));
	return (CKolok2023Doc*)m_pDocument;
}
#endif //_DEBUG


// CKolok2023View message handlers


BOOL CKolok2023View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CKolok2023View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//rNadlaktica, rPodlaktica, rSaka, rRobot, scaleRobot;

	if (nChar == 'A')
	{
		rSaka -= 10.0f;
		if (rSaka <= -10.0f)
			rSaka = -10.0f;
	}
	else if (nChar == 'S')
	{
		rSaka += 10;
		if (rSaka >= 30.0f)
			rSaka = 30.0f;
	}


	else if (nChar == 'D')
	{
		rPodlaktica -= 10.0f;
		if (rPodlaktica <= -10.0f)
			rPodlaktica = -10.0f;
	}
	else if (nChar == 'F')
	{
		rPodlaktica += 10;
		if (rPodlaktica >= 80.0f)
			rPodlaktica = 80.0f;
	}

	else if (nChar == 'G')
	{
		rNadlaktica -= 10.0f;
		if(rNadlaktica <= -10.0f)
			rNadlaktica = -10.0f;

	}

	else if (nChar == 'H')
	{
		rNadlaktica += 10;
		if (rNadlaktica >= 90.0f)
			rNadlaktica = 90.0f;
	}
	else if (nChar == '1')
	{
		rRobot += 10.0f;
		scaleRobot += 0.1f;
	}
	else if (nChar == '2')
	{
		rRobot -= 10.0f;
		scaleRobot -= 0.1f;
	}
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
