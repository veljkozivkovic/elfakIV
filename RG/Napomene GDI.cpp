//===================1. Kako da nacrtas mnogougao =================================================
void CPrviView::DrawPolygon(CDC* pDC, short n, int r,double angle, CPoint center)
{
	CPoint* points = new CPoint[n];
	
	
	angle = toRad(angle);
	for (int i = 0; i < n; i++, angle+=step)
	{
		points[i].x = center.x + r * cos(angle);
		points[i].y = center.y + r * sin(angle);
	}

	pDC->Polygon(points, n);

	delete[] points;
}

//===================2. Za providan brush==========================================================
// Brush da uzmes da je providan tj da iza njega ostane boja:
oldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
pDC->SelectObject(oldBrush);


//===================3. Za geometrijske transformacije==============================================
// da bi omogucio left multiply u OnDraw(CDC* pDC) nakon onih default linija ubacis:
int oldGM = pDC->GetGraphicsMode();
pDC->SetGraphicsMode(GM_ADVANCED);

// I NA KRAJ 
pDC->SetGraphicsMode(oldGM);


//===================3. Za geometrijske transformacije==============================================
// da se pamti i resetuje
XFORM oldTransform;
pDC->GetWorldTransform(&oldTransform);

pDC->SetWorldTransform(&oldTransform);

// Funkcije:
void CTreciView::Translate(CDC* pDC, int dX, int dY)
{
	XFORM t;
	t.eM11 = 1;
	t.eM12 = 0;
	t.eM21 = 0;
	t.eM22 = 1;
	t.eDx = dX;
	t.eDy = dY;
	pDC->ModifyWorldTransform(&t, MWT_LEFTMULTIPLY);
}

void CTreciView::Rotate(CDC* pDC, float angle)
{
	angle = toRad(angle);
	XFORM r;
	r.eM11 = cos(angle);
	r.eM12 = sin(angle);
	r.eM21 = -sin(angle);
	r.eM22 = cos(angle);
	r.eDx = 0 ;
	r.eDy = 0;
	pDC->ModifyWorldTransform(&r, MWT_LEFTMULTIPLY);
}

void CTreciView::Scale(CDC* pDC, float sX, float sY)
{
	XFORM s;
	s.eM11 = sX;
	s.eM12 = 0;
	s.eM21 = 0;
	s.eM22 = sY;
	s.eDx = 0;
	s.eDy = 0;
	pDC->ModifyWorldTransform(&s, MWT_LEFTMULTIPLY);
}

void CTreciView::Mirror(CDC* pDC, bool x, bool y)
{
	XFORM m;
	m.eM11 = x ? -1 : 1;
	m.eM12 = 0;
	m.eM21 = 0;
	m.eM22 = y ? -1 : 1;
	m.eDx = 0;
	m.eDy = 0;
	pDC->ModifyWorldTransform(&m, MWT_LEFTMULTIPLY);
}

void CKolok2017View::LoadIdentity(CDC* pDC) {
	XFORM identity = { 1, 0, 0, 1, 0, 0 };

	pDC->SetWorldTransform(&identity);
}	


//===================4. Kako sa metafajlovi=========================================================
//Deklaracija:
HENHMETAFILE cactus_part;
//Konstruktor:
cactus_part = GetEnhMetaFile(CString("res\\cactus_part.emf");
//Destruktor:
DeleteEnhMetaFile(cactus_part);
//Koriscenje:
pDC->PlayMetaFile(light ? cactus_part_light : cactus_part,CRect(-width / 2, -height / 2, width / 2, height / 2));
//NAPOMENA: Gledaj da ga crtas kao da je centar objekta u logickoj tacki (0,0)



//===================5. Kako sa slike na pDC=========================================================


// kako grayscale da se uzme:

void CTreciView::ToGray(CBitmap* bmp)
{
	// TOP DOWN JE BITMAPA

	// struktura za opis bitmape
	BITMAP bm;
	
	// bmp smesta u bm informacije o bmp
	bmp->GetBitmap(&bm);

	// smesta bitove iz bitmapu u niz bits
	BYTE* bits = new BYTE[bm.bmWidthBytes * bm.bmHeight];
	bmp->GetBitmapBits(bm.bmWidthBytes * bm.bmHeight, bits);

	// izostavljam zelenu pozadinu, pazi u memoriji se cuva slika BGR 
	// ovo je gornji levi piksel
	COLORREF bkColor = RGB(bits[2], bits[1], bits[0]);

	for (int i = 0; i < bm.bmWidthBytes * bm.bmHeight; i += 4) { // ides plus 4 jer za svaki piksel ima 4 bajta
		if (RGB(bits[i + 2], bits[i + 1], bits[i]) == bkColor) continue; //ne diras zelenu pozadinu
		BYTE greyColor = min(255, float(bits[i + 2] + bits[i + 1] + bits[i]) / 3 + 64); //prosta konverzija...
		bits[i] = bits[i + 1] = bits[i + 2] = greyColor;
	}

	bmp->SetBitmapBits(bm.bmWidthBytes * bm.bmHeight, bits);
	delete[] bits;
}






// kako da se uzme samo 1 kanal:
void CTreciView::ToRGB(CBitmap* bmp, int RGBChannel) // 0 red 1 green 2 blue
{
	BITMAP bm;

	bmp->GetBitmap(&bm);

	BYTE* bits = new BYTE[bm.bmWidthBytes * bm.bmHeight];

	bmp->GetBitmapBits(bm.bmWidthBytes * bm.bmHeight, bits);


	COLORREF bkColor = RGB(bits[2], bits[1], bits[0]);

	for (int i = 0; i < bm.bmWidthBytes * bm.bmHeight; i += 4)
	{
		if (RGB(bits[i + 2], bits[i + 1], bits[i + 0]) == bkColor) continue;
		BYTE greyColor = min(255, float(bits[i + 2] + bits[i + 1] + bits[i]) / 3 + 64); // plus 64 samo zbog vizuelnog efekta
		bits[i + 2] = RGBChannel == 0 ? greyColor : 0;
		bits[i + 1] = RGBChannel == 1 ? greyColor : 0;
		bits[i] = RGBChannel == 2 ? greyColor : 0;
	}

	bmp->SetBitmapBits(bm.bmWidthBytes * bm.bmHeight, bits);

	delete[] bits;

}



// transparent draw
void CKolok2017View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	// RGB(0 , 0, 0) - crno
	// RGB(255,255,255) - belo
	// maska je crno kod objekta pozadina je bela
	CBitmap bmpImage, bmpMask;

	BITMAP bmp;

	pImage->GetBitmap()->GetBitmap(&bmp);

													//br ravni // bita po pikselu // ovo NULL znaci neinicijalizovana bitmapa
	bmpImage.CreateBitmap(bmp.bmWidth, bmp.bmHeight, 1, bmp.bmBitsPixel, NULL); // bitmapa sa pocetnom slikom
	bmpMask.CreateBitmap(bmp.bmWidth, bmp.bmHeight, 1, 1, NULL); // bitmapa gde je svaki piksel 0 ili 1

	// u srcDC slika sa avionom a u dstDC je maska
	CDC* srcDC = new CDC();
	CDC* dstDC = new CDC();

	//
	srcDC->CreateCompatibleDC(pDC);
	dstDC->CreateCompatibleDC(pDC);

	CBitmap* oldSrcBMP = srcDC->SelectObject(&bmpImage);
	CBitmap* oldDstBMP = dstDC->SelectObject(&bmpMask);

	CRect rekt(0, 0, bmp.bmWidth, bmp.bmHeight);
	pImage->Draw(srcDC, rekt, rekt);

	COLORREF topLeftClr = srcDC->GetPixel(2, 2);
	COLORREF OldsrcBkClr = srcDC->SetBkColor(topLeftClr);

	dstDC->BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, srcDC, 0, 0, SRCCOPY);

	
	// prednja boja je bela, pozadniska crna jer bela AND boja daje tu boju
	//crna AND bilo koja daje crnu
	COLORREF oldFkClr = srcDC->SetTextColor(RGB(255, 255, 255));
	srcDC->SetBkColor(RGB(0, 0, 0));
	
	//src dc slika sa originalnim bojama i oko crno sve
	srcDC->BitBlt(0, 0, bmp.bmWidth, bmp.bmWidth, dstDC, 0, 0, SRCAND);
	
	if (toRGB) {
		ToRGB(srcDC->GetCurrentBitmap(), RGBchannelID);
	}
	else {
		ToGray(srcDC->GetCurrentBitmap());
	}
	
	//OR sa crnom ostaje ista boja
	pDC->BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, dstDC, 0, 0, SRCAND); //pdc pozadina postaje crna i ostaje senka slike
	pDC->BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, srcDC, 0, 0, SRCPAINT); //slika je tu a crna je okolo


	srcDC->SetBkColor(OldsrcBkClr);
	srcDC->SetTextColor(oldFkClr);


	dstDC->SelectObject(oldDstBMP);
	srcDC->SelectObject(oldSrcBMP);



	srcDC->DeleteDC();
	dstDC->DeleteDC();
	delete srcDC;
	delete dstDC;
}
//===================6. ANTI FLICKER=========================================================
// U OnDraw(CDC* pDC)

CRect rect;
GetClientRect(&rect);

CDC* memDC = new CDC();


CBitmap bmp;
bmp.  (pDC, rect.Width(), rect.Height());
memDC->SelectObject(&bmp);


XFORM transOld;
int gm = memDC->SetGraphicsMode(GM_ADVANCED);

memDC->GetWorldTransform(&transOld);


/ rotacije i ti kurci...




memDC->SetWorldTransform(&transOld);
memDC->SetGraphicsMode(gm);

pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0 , SRCCOPY);
memDC->DeleteDC();
delete memDC;

/I DODAJ IZ CLASS VIEW:
BOOL CKol2014View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}





//============================================KOL 14 KOLA===================================

sin = naspramna / hipotenuza
cos = nalegla / hipotenuza
tan = naspramna / nalegla
cth = nalegla / naspramna 





//============================================KOL 17 R2D2===================================

//SCENA KAKO JEBENO DA SE POMERA

	else if (nChar == 'A')
	{
		rightOffset -= 200;
		if (rightOffset < 200)
			rightOffset = 0;
		Invalidate();

	}
	else if (nChar == 'D')
	{
		CRect clientRect;
		GetClientRect(&clientRect);
		rightOffset += 200;
		if (rightOffset > Back2->Width() - clientRect.Width())
			rightOffset = Back2->Width() - clientRect.Width();
		Invalidate();

	}
	else if (nChar == 'W')
	{
		topOffset -= 200;
		if (topOffset < 200)
		{
			topOffset = 0;
		}
		Invalidate();

	}
	else if (nChar == 'S')
	{
		CRect clientRect;
		GetClientRect(&clientRect);
		topOffset += 200;
		if (topOffset > Back2->Height() - clientRect.Height())
			topOffset = Back2->Height() - clientRect.Height();
		Invalidate();

	}
void CKolok2017View::DrawBg(CDC* pDC, CRect rect) {
	back2->Draw(pDC, CRect(rightOffset, topOffset, rect.Width() + rightOffset, rect.Height() + topOffset), rect);
}



//KAKO DA NAMESTIM KAD SAMO ODREDJENE VREDNOSTI MOZE DA IMA NOGA
	float legAngles[4];
	float legPrevAngles[4];
	
	KONSTRUKTOR:
	legAngles[0] = -20;
	legAngles[1] = 20;
	legAngles[2] = legAngles[3] = 0;

	legPrevAngles[0] = -10;
	legPrevAngles[1] = 10;
	legPrevAngles[2] = 10;
	legPrevAngles[3] = -10;
	
	
	void CKol2017View::ChangeAngle(float* angle, float* prevAngle)
{
	if ((*angle == -20 || *angle > *prevAngle) && *angle != 20)
	{
		*prevAngle = *angle;
		*angle += 10;
	}
	else {
		*prevAngle = *angle;
		*angle -= 10;
	}
}


	if (nChar == VK_LEFT)
	{
		for (int i = 0; i < 4; i++)
			ChangeAngle(&legAngles[i], &legPrevAngles[i]);
		dist -= 20;
		Invalidate();
	}
	else if (nChar == VK_RIGHT)
	{
		for (int i = 0; i < 4; i++)
			ChangeAngle(&legAngles[i], &legPrevAngles[i]);
		dist += 20;
		Invalidate();

	}
	
	
void CKolok2017View::LoadIdentity(CDC* pDC) {
	XFORM identity = { 1, 0, 0, 1, 0, 0 };

	pDC->SetWorldTransform(&identity);
}

	
//======================================KOL 23 ROBOT========================================

KAD KAZE POZADINA U PRIRODNOJ VELICINI:
void CKolok2023View::DrawBackground(CDC* pDC) {
	pozadina->Draw(pDC, CRect(0, 0, pozadina->Width(), pozadina->Height()), CRect(0, 0, pozadina->Height(), pozadina->Width()));
}


POSETI SE OVO SA SKALIRANJE ROTIRANJE ZAJEBAO SI SE BIO
void CKol2023View::DrawRobot(CDC* pDC)
{
	XFORM old;
	pDC->GetWorldTransform(&old);

	Translate(pDC, pozadina->Width() / 2, pozadina->Height() / 2, false); // ides u centar slike da zarotiras i skaliras
	Rotate(pDC, rotRobot, false);
	Scale(pDC, sleRobot  , sleRobot, false);

	Translate(pDC,  - telo->Width() ,  - telo->Height() , false);
	DrawHalf(pDC); // crta u logicku (0,0)
	Translate(pDC, telo->Width(), telo->Height(), false);
	Scale(pDC, -1, 1, false);
	Translate(pDC, -telo->Width(), -telo->Height(), false);
	DrawHalf(pDC); // crta u logicku (0,0)
	Translate(pDC, telo->Width() - glava->Width() / 2, -glava->Height() + 20, false);
	DrawImgTransparent(pDC, glava);





	pDC->SetWorldTransform(&old);
}

//======================================KOL 18 BAGER========================================
Napisati funkciju void DrawBackground(CDC* pDC), koja iscrtava sliku pozadina.png, tako da
je uvek centrirana po X-osi, prilikom promene veličine prozora, a donja ivica se poklapa sa
donjom ivicom klijentskog dela prozora. [5 poena] 
void CKol2018View::DrawBackground(CDC* pDC)
{
	CRect rekt;
	GetClientRect(&rekt);

	int rW = rekt.Width();
	int rH = rekt.Height();

	int pW = pozadina->Width();
	int pH = pozadina->Height();

	int dW = (rW - pW) / 2; // jer kaze da treba da bude centriran po x osi slika
	int dH = rH - pH; // jer kaze da treba dno slike da se poklapa sa dnom prozora

	pozadina->Draw(pDC, CRect(0, 0, pW, pH), CRect(dW, dH, dW + pW, dH+ pH)); // vidis kad crtas crtas od tacke 
																			//razlike koja je negde levo gore iznad (0,0)
																			// pa sabiras razliku sa sirine i visine slike taman dole desno 
}


//KAKO DA UZMES INFORMACIJE O VELICINI HENHEMTAFILEA:
	viljuska = GetEnhMetaFile(CString("viljuska.emf"));
	GetEnhMetaFileHeader(viljuska, sizeof(viljuska_header), &viljuska_header);
	int width = viljuska_header.rclBounds.right - viljuska_header.rclBounds.left;
	int height = viljuska_header.rclBounds.bottom - viljuska_header.rclBounds.top;
	Scale(pDC, 2.5, 2.5, false); // vidis scale uradi pre rotate i translate da se poklopi lepo

	Rotate(pDC, a_fork, false);
	Translate(pDC, -14 , -20 , false);
	pDC->PlayMetaFile(viljuska, CRect(0, 0, width,height));
	
"ZNACI KAD SE NALAZIS U JEDNOJ TACKI OKO KOJE TREBA DA SE ROTIRA NOVA SLIKA URADI TRANSLATE (-X,-Y) za X i Y KOJI SU TI DALI DA JE TAJ NEKI KRUG U SLICI I POSLE TOGA GLEDAS APSOLUTNO X I Y OSTALIH TACAKA"



//======================================KOL 19 TRANSFORMERS======================================= MORA IH URADIS OPETTT

/KAD KAZE PORAVNJATI GORNJU LEVU IVICU
void CKol2019View::DrawBackground(CDC* pDC, CRect rc) // rc je clientrect
{
	background->Draw(pDC, rc, rc);
}





//=====================================KOL 24 SCHIZO===============================================


"ALOOOOOOOOOOO FONTOVI"
CFont font;
font.CreateFont(
	(int)(0.9 * r),   // visina fonta
	0,                 // širina (default)
	0,					// escapement = -30°
	0,                 // orientation
	FW_BOLD,           // bold
	FALSE,             // italic
	FALSE,             // underline
	FALSE,             // strikeout
	ANSI_CHARSET,
	FALSE,
	FALSE,
	FALSE,
	DEFAULT_PITCH,
	_T("Verdana")
);
CFont* pOldFont = pDC->SelectObject(&font);
int oldBkMode = pDC->GetBkMode();
pDC->SetBkMode(TRANSPARENT);
COLORREF oldTextClr = pDC->GetTextColor();
pDC->SetTextColor(RGB(0, 0, 139));
CString text = _T("R_G");
CSize sz = pDC->GetTextExtent(text);
pDC->SetTextAlign(TA_CENTER);
Translate(pDC, 0, 30, false);
pDC->TextOut(0, 0, text);

pDC->SelectObject(pOldFont);
pDC->SetBkMode(oldBkMode);
pDC->SetTextColor(oldTextClr);
pDC->SetWorldTransform(&old);




//=================================ARC BESIEOVE KRIVE=============================================



kurac
