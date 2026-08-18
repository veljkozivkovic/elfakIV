
// Kolok2018View.cpp : implementation of the CKolok2018View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kolok2018.h"
#endif

#include "Kolok2018Doc.h"
#include "Kolok2018View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKolok2018View

IMPLEMENT_DYNCREATE(CKolok2018View, CView)

BEGIN_MESSAGE_MAP(CKolok2018View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKolok2018View construction/destruction

CKolok2018View::CKolok2018View() noexcept
{
	viljuska = GetEnhMetaFile(CString("viljuska.emf"));

	bager = new DImage();
	bager->Load(CString("bager.png"));

	arm1 = new DImage();
	arm1->Load(CString("arm1.png"));

	arm2 = new DImage();
	arm2->Load(CString("arm2.png"));

	pozadina = new DImage();
	pozadina->Load(CString("pozadina.png"));

	
	this->gridSize = 50;
	this->drawGrid = true;
	this->xPomeraj = 0.0f;
	this->rArm1 = 0.0f;
	this->rArm2 = 0.0f;
	this->rViljuska = 0.0f;
}

CKolok2018View::~CKolok2018View()
{
	DeleteEnhMetaFile(viljuska);

	delete bager;
	delete arm1;
	delete arm2;
	delete pozadina;

}

BOOL CKolok2018View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKolok2018View drawing

void CKolok2018View::Translate(CDC* pDC, int dX, int dY, bool rightMultiply)
{
	XFORM form;
	form.eM11 = 1;
	form.eM12 = 0;
	form.eM21 = 0;
	form.eM22 = 1;
	form.eDx = dX;
	form.eDy = dY;
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CKolok2018View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM form;
	form.eM11 = cos(toRad(angle));
	form.eM12 = sin(toRad(angle));
	form.eM21 = -sin(toRad(angle));
	form.eM22 = cos(toRad(angle));
	form.eDx = 0;
	form.eDy = 0;
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CKolok2018View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM form;
	form.eM11 = sX;
	form.eM12 = 0;
	form.eM21 = 0;
	form.eM22 = sY;
	form.eDx = 0;
	form.eDy = 0;
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CKolok2018View::DrawBackGround(CDC* pDC)
{
	CRect client;
	GetClientRect(client);

	int w = pozadina->Width();
	int h = pozadina->Height();

	pozadina->Draw(pDC, CRect(0, 0, w, h), client);
}

void CKolok2018View::DrawImgTransparent(CDC* pDC, DImage* pImage)
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

void CKolok2018View::DrawGrid(CDC* pDC)
{
	if (this->drawGrid)
	{

		CPen gridPen(PS_SOLID, 1, RGB(195, 230, 245)); // sto manje vrednosti tamnije je
		CPen* pOldPen = pDC->SelectObject(&gridPen);

		// Po vertikali
		for (int x = 0; x <= 10000; x += this->gridSize)
		{
			pDC->MoveTo(x, 0);
			pDC->LineTo(x, 10000);
		}

		// herozi
		for (int y = 0; y <= 10000; y += this->gridSize)
		{
			pDC->MoveTo(0, y);
			pDC->LineTo(10000, y);
		}


		pDC->SelectObject(pOldPen);
		gridPen.DeleteObject();
	}
}

void CKolok2018View::OnDraw(CDC* pDC)
{
	CKolok2018Doc* pDoc = GetDocument();
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

	DrawBackGround(memDC);
	DrawBody(memDC);
	DrawArm1(memDC);
	DrawArm2(memDC);
	DrawFork(memDC);
	/*----------------------------------------------------------------------------*/
	memDC->SetWorldTransform(&oldForm);
	DrawGrid(memDC);
	pDC->BitBlt(0, 0, client.Width(), client.Height(), memDC, 0, 0, SRCCOPY);
	memDC->SetGraphicsMode(gm);
	memDC->DeleteDC();
	delete memDC;
}


// CKolok2018View printing

BOOL CKolok2018View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKolok2018View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKolok2018View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CKolok2018View::DrawBody(CDC* pDC)
{
	CRect cRect;
	GetClientRect(cRect);
	Translate(pDC, cRect.right - bager->Width() - xPomeraj, cRect.bottom  - bager->Height(), false);
	DrawImgTransparent(pDC, bager);
	Translate(pDC, 0, 100, false);
}
void CKolok2018View::DrawArm1(CDC* pDC)
{

	// ovo samo oko ose oko koje treba da se rotira

	Translate(pDC, 58, 60, false);
	Rotate(pDC, -90 + this->rArm1, false);
	Translate(pDC, -58, -60, false);



	DrawImgTransparent(pDC, arm1);

	Translate(pDC, 309, 61, false); // odes do tacke gde treba se rotira arm2

}

void CKolok2018View::DrawArm2(CDC* pDC)
{



	Translate(pDC, -36, -40, false); // namestis - 36, - 40 da cetriras taj krug pocetni sa njihovim




	Translate(pDC, 36, 40, false);
	Rotate(pDC, -90 + this->rArm2, false);
	Translate(pDC, -36, -40, false);



	DrawImgTransparent(pDC, arm2);


	// Translate namesta poziciju za sledecu tacku
	Translate(pDC, 272, 40, false);


}
void CKolok2018View::DrawFork(CDC* pDC)
{
	XFORM form;

	Scale(pDC, 2.5, 2.5, false); // ovako se radi sa scale boli te k

	Translate(pDC, -8, -12, false);

	CRect rectVilj(0,0 ,44 , 34);
	Translate(pDC, 8, 12, false);
	Rotate(pDC, - 90 + this->rViljuska, false);
	Translate(pDC, -8, -12, false);

	pDC->PlayMetaFile(viljuska, rectVilj);

	//Translate(pDC, -21 * gridSize, -12 * gridSize, false);

	pDC->SetWorldTransform(&form);
}






// CKolok2018View diagnostics

#ifdef _DEBUG
void CKolok2018View::AssertValid() const
{
	CView::AssertValid();
}

void CKolok2018View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKolok2018Doc* CKolok2018View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKolok2018Doc)));
	return (CKolok2018Doc*)m_pDocument;
}
#endif //_DEBUG


// CKolok2018View message handlers


void CKolok2018View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == '1')
	{
		rArm1 -= 10.0f;
	}
	if (nChar == '2')
	{
		rArm1 += 10.0f;

	}
	if (nChar == '3')
	{
		rArm2 -= 10.0f;
	}
	if (nChar == '4')
	{
		rArm2 += 10.0f;
	}
	if (nChar == '5')
	{
		rViljuska -= 10.0f;
	}
	if (nChar == '6')
	{
		rViljuska += 10.0f;
	}
	if (nChar == VK_LEFT)
	{
		xPomeraj += 10.0f;
	}
	if (nChar == VK_RIGHT)
	{
		
		xPomeraj -= 10.0f;
	}
	if (nChar == VK_SPACE)
	{
		drawGrid = !drawGrid;
	}
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CKolok2018View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
