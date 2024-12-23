
// LAB2View.cpp : implementation of the CLAB2View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "LAB2.h"
#endif

#include "LAB2Doc.h"
#include "LAB2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLAB2View

IMPLEMENT_DYNCREATE(CLAB2View, CView)

BEGIN_MESSAGE_MAP(CLAB2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CLAB2View construction/destruction

CLAB2View::CLAB2View() noexcept
{
	this->emf_cactus = GetEnhMetaFile(CString("C:\\Users\\HP\\Downloads\\RESURSI\\cactus_part.emf"));
	this->emf_cactus_light = GetEnhMetaFile(CString("C:\\Users\\HP\\Downloads\\RESURSI\\cactus_part_light.emf"));
	this->gridSize = 25;
	this->keyPressed = false;
	this->partAngle = 0;
	this->wholeAngle = 0;

	if (!this->emf_cactus || !this->emf_cactus_light)
		AfxMessageBox(_T("Neuspesno uchitani emf fajlovi, majmunu@@@!"));
}

CLAB2View::~CLAB2View()
{
	DeleteEnhMetaFile(emf_cactus);
	DeleteEnhMetaFile(emf_cactus_light);
}

BOOL CLAB2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLAB2View drawing

void CLAB2View::SetBackgroundColor500x500(CDC* pDC)
{
	COLORREF backgroundCol = RGB(135, 206, 235);

	CBrush backgroundBrush(backgroundCol);
	CPen backgroundPen(PS_NULL, 1, backgroundCol);

	CBrush* oldBrush = pDC->SelectObject(&backgroundBrush);
	CPen* oldPen = pDC->SelectObject(&backgroundPen);

	CRect rect(0, 0, 500, 500);

	pDC->Rectangle(&rect);

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
}

void CLAB2View::DrawGrid(CDC* pDC)
{
	if (this->keyPressed)
	{

		CPen gridPen(PS_SOLID, 1, RGB(195, 230, 245)); // sto manje vrednosti tamnije je
		CPen* pOldPen = pDC->SelectObject(&gridPen);

		// Po vertikali
		for (int x = 0; x <= gridSize * 20; x += this->gridSize)
		{
			pDC->MoveTo(x, 0);
			pDC->LineTo(x, gridSize * 20);
		}

		// herozi
		for (int y = 0; y <= gridSize * 20; y += this->gridSize)
		{
			pDC->MoveTo(0, y);
			pDC->LineTo(gridSize * 20, y);
		}


		pDC->SelectObject(pOldPen);
		gridPen.DeleteObject();
	}
}

void CLAB2View::Translate(CDC* pDC, float dX, float dY)
{
	XFORM xForm;
	xForm.eM11 = 1.0f;
	xForm.eM12 = 0.0f;
	xForm.eM21 = 0.0f;
	xForm.eM22 = 1.0f;
	xForm.eDx = dX;
	xForm.eDy = dY;
	pDC->ModifyWorldTransform(&xForm,MWT_LEFTMULTIPLY);
}

void CLAB2View::Scale(CDC* pDC, float sX, float sY)
{
	XFORM xForm;
	xForm.eM11 = sX;
	xForm.eM12 = 0.0f;
	xForm.eM21 = 0.0f;
	xForm.eM22 = sY;
	xForm.eDx = 0.0f;
	xForm.eDy = 0.0f;
	pDC->ModifyWorldTransform(&xForm, MWT_LEFTMULTIPLY);
}

void CLAB2View::Rotate(CDC* pDC, float angle)
{
	XFORM xForm;
	float rad = angle * (3.14159265f / 180.0f);
	xForm.eM11 = cos(rad);
	xForm.eM12 = sin(rad);
	xForm.eM21 = -sin(rad);
	xForm.eM22 = cos(rad);
	xForm.eDx = 0.0f;
	xForm.eDy = 0.0f;
	pDC->ModifyWorldTransform(&xForm, MWT_LEFTMULTIPLY);
}

void CLAB2View::DrawPot(CDC* pDC)
{
	POINT point1 = { gridSize * 8.25, gridSize * 20 };
	POINT point2 = { gridSize * 8, gridSize * 18 };
	POINT point3 = { gridSize * 7.65, gridSize * 18 };
	POINT point4 = { gridSize * 7.65, gridSize * 17.1 };
	POINT point5 = { gridSize * 12.4, gridSize * 17.1 };
	POINT point6 = { gridSize * 12.4, gridSize * 18 };
	POINT point7 = { gridSize * 12, gridSize * 18 };
	POINT point8 = { gridSize * 11.75, gridSize * 20 };

	POINT points[8] = { point1, point2, point3, point4, point5, point6, point7, point8 };
	POINT line[2] = { point2, point7 };
	CPen newPen(PS_SOLID | PS_GEOMETRIC, 1, RGB(0, 0, 0));
	CBrush newBrush(RGB(222, 148, 0));

	CPen* oldPen = pDC->SelectObject(&newPen);
	CBrush* oldBrush = pDC->SelectObject(&newBrush);



	pDC->Polygon(points, 8);
	pDC->Polyline(line, 2);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CLAB2View::DrawCactus(CDC* pDC, float scalex, float scaley, POINT translate, int selfRotationAngle, HENHMETAFILE mf)
{
	XFORM prevTransform;
	GetWorldTransform(pDC->GetSafeHdc(), &prevTransform);

	this->Translate(pDC, translate.x, translate.y);
	this->Rotate(pDC, selfRotationAngle);
	this->Scale(pDC, scalex, scaley);

	PlayEnhMetaFile(pDC->GetSafeHdc(), mf, CRect(POINT{ -gridSize / 2, -gridSize }, SIZE{ gridSize, gridSize }));

	SetWorldTransform(pDC->GetSafeHdc(), &prevTransform);
}

void CLAB2View::DrawCircle(CDC* pDC, POINT point, int r, COLORREF color, COLORREF fill)
{
	CPen newPen(PS_SOLID | PS_GEOMETRIC, 1, color);
	CBrush newBrush(fill);

	CPen* prevPen = pDC->SelectObject(&newPen);
	CBrush* prevBrush = pDC->SelectObject(&newBrush);

	pDC->Ellipse(point.x - r / 2, point.y + r / 2, point.x + r / 2, point.y - r / 2);

	pDC->SelectObject(prevBrush);
	pDC->SelectObject(prevPen);
}

void CLAB2View::RotateAroundPoint(CDC* pDC, POINT point, float angle)
{
	this->Translate(pDC, point.x, point.y);
	this->Rotate(pDC, angle);
	this->Translate(pDC, -point.x, -point.y);
}

void CLAB2View::DrawFigures(CDC* pDC)
{
	POINT bottomCircle = { gridSize * 10, gridSize * 17 };
	POINT upperCircle = { gridSize * 10 , gridSize * 14 };
	POINT leftBottomCircle = { gridSize * 7.8, gridSize * 11.9 };
	POINT leftUpperCircle = { gridSize * 7.8, gridSize * 8.9 };
	POINT rightBottomCircle = { gridSize * 12.2, gridSize * 11.9 };
	POINT rightUpperCircle = { gridSize * 15.2, gridSize * 11.9 };
	POINT proba = { gridSize * 13, gridSize * 4 };

	COLORREF CIRCLE_GREEN = RGB(0, 204, 0);

	float thinCactusWidth = 1;
	float thinCactusHeight = 3;

	SIZE thinCactus = { thinCactusWidth, thinCactusHeight };

	float mediumCactusWidth = 2;
	float mediumCactusHeight = 3;

	SIZE mediumCactus = { mediumCactusWidth, mediumCactusHeight };

	double thickCactusWidth = 2.5;
	float thickCactusHeight = 3;

	SIZE thickCactus = { thickCactusWidth, thickCactusHeight };

	XFORM xFormOld;
	GetWorldTransform(pDC->GetSafeHdc(), &xFormOld);

	RotateAroundPoint(pDC, bottomCircle, this->wholeAngle);

	// donji svetao kaktus
	DrawCactus(pDC, thickCactusWidth, thinCactusHeight, bottomCircle, 0, emf_cactus_light);

	// 3 mala kaktusa tamna

	DrawCactus(pDC, thinCactusWidth, thinCactusHeight, upperCircle, 0, emf_cactus);
	DrawCactus(pDC, thinCactusWidth, thinCactusHeight, upperCircle, 45, emf_cactus);
	DrawCactus(pDC, thinCactusWidth, thinCactusHeight, upperCircle, -45, emf_cactus);

	// 2 srednja kaktusa tamna levo

	DrawCactus(pDC, mediumCactusWidth, mediumCactusHeight, leftBottomCircle, 0, emf_cactus);
	DrawCactus(pDC, mediumCactusWidth, mediumCactusHeight, leftBottomCircle, 270, emf_cactus);

	// jedan veliki gore levo tamni
	DrawCactus(pDC, thickCactusWidth, thinCactusHeight, leftUpperCircle, 0, emf_cactus);

	// dva desno srednja jedan tamni jedan svetli
	DrawCactus(pDC, mediumCactusWidth, mediumCactusHeight, rightBottomCircle, 0 + this->partAngle, emf_cactus_light);
	DrawCactus(pDC, mediumCactusWidth, mediumCactusHeight, rightBottomCircle, 90, emf_cactus);

	// dva desno srednja tamna
	DrawCactus(pDC, mediumCactusWidth, mediumCactusHeight, rightUpperCircle, 45, emf_cactus);
	DrawCactus(pDC, mediumCactusWidth, mediumCactusHeight, rightUpperCircle, 135, emf_cactus);

	//SetWorldTransform(pDC->GetSafeHdc(), &xFormOld);

	//statik krug
	DrawCircle(pDC, bottomCircle, gridSize, RGB(0, 0, 0), CIRCLE_GREEN);
	


	//dinamik krugovi
	DrawCircle(pDC, upperCircle, gridSize, RGB(0, 0, 0), CIRCLE_GREEN);
	DrawCircle(pDC, leftBottomCircle, gridSize, RGB(0, 0, 0), CIRCLE_GREEN);
	DrawCircle(pDC, leftUpperCircle, gridSize, RGB(0, 0, 0), CIRCLE_GREEN);
	DrawCircle(pDC, rightBottomCircle, gridSize, RGB(0, 0, 0), CIRCLE_GREEN);
	DrawCircle(pDC, rightUpperCircle, gridSize, RGB(0, 0, 0), CIRCLE_GREEN);


	SetWorldTransform(pDC->GetSafeHdc(), &xFormOld);

}

void CLAB2View::OnDraw(CDC* pDC)
{
	CLAB2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int oldGraphicsMode = pDC->SetGraphicsMode(GM_ADVANCED);
	SetBackgroundColor500x500(pDC);
	
	DrawPot(pDC);
	DrawFigures(pDC);

	DrawGrid(pDC);
	pDC->SetGraphicsMode(oldGraphicsMode);
}


// CLAB2View printing

BOOL CLAB2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLAB2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLAB2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CLAB2View diagnostics

#ifdef _DEBUG
void CLAB2View::AssertValid() const
{
	CView::AssertValid();
}

void CLAB2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

void CLAB2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_SPACE:
			this->keyPressed = !this->keyPressed;
			break;

		case VK_LEFT:
			this->partAngle -= 6;
			break;

		case VK_RIGHT:
			this->partAngle += 6;
			break;

		case 'A':
			this->wholeAngle-= 6;
			break;

		case 'D':
			this->wholeAngle += 6;
			break;
		default:
			break;
	}
	Invalidate();
	//InvalidateRect(NULL);

	CView::OnKeyDown(nChar, nRepCnt, nFlags); //mora zoves za svaki slucaj ako radi to dugme nesto
}

CLAB2Doc* CLAB2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLAB2Doc)));
	return (CLAB2Doc*)m_pDocument;
}
#endif //_DEBUG


// CLAB2View message handlers
