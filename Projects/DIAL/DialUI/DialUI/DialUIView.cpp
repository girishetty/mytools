
// DialUIView.cpp : implementation of the CDialUIView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DialUI.h"
#endif

#include "DialUIDoc.h"
#include "DialUIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDialUIView

IMPLEMENT_DYNCREATE(CDialUIView, CView)

BEGIN_MESSAGE_MAP(CDialUIView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CDialUIView construction/destruction

CDialUIView::CDialUIView()
{
	// TODO: add construction code here

}

CDialUIView::~CDialUIView()
{
}

BOOL CDialUIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDialUIView drawing

void CDialUIView::OnDraw(CDC* /*pDC*/)
{
	CDialUIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CDialUIView printing

BOOL CDialUIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDialUIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDialUIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CDialUIView diagnostics

#ifdef _DEBUG
void CDialUIView::AssertValid() const
{
	CView::AssertValid();
}

void CDialUIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDialUIDoc* CDialUIView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDialUIDoc)));
	return (CDialUIDoc*)m_pDocument;
}
#endif //_DEBUG


// CDialUIView message handlers
