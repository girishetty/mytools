
// DialUIView.h : interface of the CDialUIView class
//

#pragma once


class CDialUIView : public CView
{
protected: // create from serialization only
	CDialUIView();
	DECLARE_DYNCREATE(CDialUIView)

// Attributes
public:
	CDialUIDoc* GetDocument() const;

// Operations
public:

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
	virtual ~CDialUIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DialUIView.cpp
inline CDialUIDoc* CDialUIView::GetDocument() const
   { return reinterpret_cast<CDialUIDoc*>(m_pDocument); }
#endif

