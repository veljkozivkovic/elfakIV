
// GDIView.cpp : implementation of the CGDIView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GDI.h"
#endif

#include "GDIDoc.h"
#include "GDIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGDIView

IMPLEMENT_DYNCREATE(CGDIView, CView)

BEGIN_MESSAGE_MAP(CGDIView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()//ovo mora se doda zbog onkeydown funkcije
END_MESSAGE_MAP()

// CGDIView construction/destruction

CGDIView::CGDIView() noexcept
{
	// TODO: add construction code here
	this->keyPressed = false;
	this->gridSize = 25;
}

CGDIView::~CGDIView()
{
}

BOOL CGDIView::PreCreateWindow(CREATESTRUCT& cs)
{
	//ne radi??!?!?!?
	cs.cx = 500;
	cs.cy = 500;

	//mora obavezno da se zove osnovna klasa
	return CView::PreCreateWindow(cs);
}

// CGDIView drawing

void CGDIView::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle)
{
	//cx, cy - centar poligona
	//r - poluprecnik
	//n - broj stranica
	//rotAngle - ugao rotacije u stepenima

	//ugao izrazen u radijanima
	float angle = 2 * 3.14159265359 / n;

	//rotacija
	float rot = rotAngle * 3.14159265359 / 180;

	//niz tacaka poligona
	POINT* points = new POINT[n];

	for (int i = 0; i < n; i++)
	{
		points[i].x = cx + r * cos(angle * i + rot);
		points[i].y = cy + r * sin(angle * i + rot);
	}

	COLORREF bgColor = pDC->GetPixel(cx, cy);
	CBrush bgBrush(bgColor); 
	CBrush* pOldBrush = pDC->SelectObject(&bgBrush);

	
	CPen redSmallerPen(PS_SOLID, 2, RGB(255, 0, 0)); 
	CPen* pOldPen = pDC->SelectObject(&redSmallerPen);

	
	pDC->Polygon(points, n);

	
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	bgBrush.DeleteObject();
	redSmallerPen.DeleteObject();
	
	
	delete[] points;
}

POINT CGDIView::GetTriangleCenter(POINT points[3])
{
	POINT centar;
	
	centar.x = (points[0].x + points[1].x + points[2].x) / 3;
	centar.y = (points[0].y + points[1].y + points[2].y) / 3;
		
	return centar;
}

void CGDIView::OnDraw(CDC* pDC)
{
	CGDIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	

	
	POINT centar;
	//==========roze trougao==========
	CPen redPen(PS_SOLID, 4, RGB(255, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&redPen);

	CBrush pinkBrush(RGB(255,153, 204));
	CBrush* pOldBrush = pDC->SelectObject(&pinkBrush);

	POINT rozeT[3] = { {25, 250}, {137, 137}, {137, 362} };
	pDC->Polygon(rozeT, 3);

	//pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pinkBrush.DeleteObject();

	centar = GetTriangleCenter(rozeT);
	DrawRegularPolygon(pDC, centar.x, centar.y, 25, 8, 45);

	//==========crveni kvadrat==========
	CBrush redBrush(RGB(255, 0, 0));
	pOldBrush = pDC->SelectObject(&redBrush);

	POINT crveniK[4] = { {137, 137}, {250, 137}, {250, 250}, {137, 250} };
	
	//pDC->Rectangle(137, 137, 250, 250); bolje je da ne radim s ovim jer ne pogadja piksele lepo
	pDC->Polygon(crveniK, 4);
	pDC->SelectObject(pOldBrush);
	redBrush.DeleteObject();


	//==========zeleni trougao==========
	CBrush greenBrush(RGB(50, 205, 50));
	pOldBrush = pDC->SelectObject(&greenBrush);

	POINT zeleniT[3] = { {137,137} , {250, 25}, {250,137} };
	pDC->Polygon(zeleniT, 3);

	pDC->SelectObject(pOldBrush);
	greenBrush.DeleteObject();

	centar = GetTriangleCenter(zeleniT);
	DrawRegularPolygon(pDC, centar.x, centar.y, 17, 4, 0);

	//==========ljubicast trougao==========
	CBrush purpleBrush(RGB(153, 0, 204));
	pOldBrush = pDC->SelectObject(&purpleBrush);

	POINT ljubicastT[3] = { {137, 250}, {250, 362}, {250, 250} };
	pDC->Polygon(ljubicastT, 3);
	
	pDC->SelectObject(pOldBrush);
	purpleBrush.DeleteObject();

	centar = GetTriangleCenter(ljubicastT);
	DrawRegularPolygon(pDC, centar.x, centar.y, 17, 5, 0);

	//==========HS_DIAGCROSS paralelogram==========
	CBrush blueBrush(HS_DIAGCROSS, RGB(101, 153, 255));
	pOldBrush = pDC->SelectObject(&blueBrush);

	POINT hs_diagcross[4] = { {137, 362}, {250, 475}, {250, 362}, {137, 250} };
	pDC->Polygon(hs_diagcross, 4);
	
	pDC->SelectObject(pOldBrush);
	blueBrush.DeleteObject();


	//==========zut trougao==========
	CBrush yellowBrush(RGB(255, 255, 0));
	pOldBrush = pDC->SelectObject(&yellowBrush);

	POINT yellowT[3] = { {250, 25}, {475, 25}, {250, 250} };
	pDC->Polygon(yellowT, 3);

	pDC->SelectObject(pOldBrush);
	yellowBrush.DeleteObject();

	centar = GetTriangleCenter(yellowT);
	DrawRegularPolygon(pDC, centar.x, centar.y, 35, 6, 0);

	//==========oker trougao==========
	CBrush okerBrush(RGB(204, 153, 51));
	pOldBrush = pDC->SelectObject(&okerBrush);

	POINT okerT[3] = { {250, 250}, {250, 475}, {475, 475} };
	pDC->Polygon(okerT, 3);
	
	pDC->SelectObject(pOldBrush);
	okerBrush.DeleteObject();

	centar = GetTriangleCenter(okerT);
	DrawRegularPolygon(pDC, centar.x, centar.y, 35, 7, 0);

	//==========GRID==========
	if (this->keyPressed)
	{

		CPen gridPen(PS_SOLID, 1, RGB(150, 150, 150)); // sto manje vrednosti tamnije je
		CPen* pOldPen = pDC->SelectObject(&gridPen);


		//CRect rect;
		//GetClientRect(&rect);
		//rect.Height() i rect.Width() su visina i sirina prozora


		// Po vertikali
		for (int x = 0; x <= 500; x += this->gridSize)
		{
			pDC->MoveTo(x, 0);
			pDC->LineTo(x, 500);
		}

		// herozi
		for (int y = 0; y <= 500; y += this->gridSize)
		{
			pDC->MoveTo(0, y);
			pDC->LineTo(500, y);
		}

		// OBAVEZNO BEEEE
		pDC->SelectObject(pOldPen);
		gridPen.DeleteObject();
	}
}


// CGDIView printing

BOOL CGDIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGDIView diagnostics

#ifdef _DEBUG
void CGDIView::AssertValid() const
{
	CView::AssertValid();
}

void CGDIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

void CGDIView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_SPACE)
	{
		this->keyPressed = !this->keyPressed;
		Invalidate(); // Ovo je potrebno da bi se prozor ponovo iscrtavao
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags); //mora zoves za svaki slucaj ako radi to dugme nesto
}

CGDIDoc* CGDIView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDIDoc)));
	return (CGDIDoc*)m_pDocument;
}
#endif //_DEBUG


// CGDIView message handlers
