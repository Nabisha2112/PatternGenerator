// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// PtrnGeneratorView.cpp : implementation of the CPtrnGeneratorView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PtrnGenerator.h"
#endif

#include "PtrnGeneratorDoc.h"
#include "PtrnGeneratorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPtrnGeneratorView

IMPLEMENT_DYNCREATE(CPtrnGeneratorView, CView)

BEGIN_MESSAGE_MAP(CPtrnGeneratorView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPtrnGeneratorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BUTTON2, &CPtrnGeneratorView::OnButtonStart)
	ON_COMMAND(ID_BUTTON3, &CPtrnGeneratorView::OnButtonStop)
	ON_COMMAND(ID_BUTTON4, &CPtrnGeneratorView::OnButtonResume)
END_MESSAGE_MAP()

// CPtrnGeneratorView construction/destruction

CPtrnGeneratorView::CPtrnGeneratorView() noexcept
{
	// TODO: add construction code here
	m_pCurrentThread = NULL;
	m_iCounter = 0;
}

CPtrnGeneratorView::~CPtrnGeneratorView()
{
}

BOOL CPtrnGeneratorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CPtrnGeneratorView drawing

void CPtrnGeneratorView::OnDraw(CDC* pDC)
{
	CPtrnGeneratorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CString strCounter;
	strCounter.Format(L"The Counter is %d", m_iCounter);
	pDC->TextOut(100, 100, strCounter);
	for (int i = 0; i<m_PointArray.GetSize() - 2; i++)
	{
		pDC->Rectangle(10,20,1000,1200);
		pDC->MoveTo(m_PointArray[i].x, m_PointArray[i].y);
		pDC->LineTo(m_PointArray[i+1].x, m_PointArray[i+1].y);
	}

	// TODO: add draw code for native data here
}


// CPtrnGeneratorView printing


void CPtrnGeneratorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPtrnGeneratorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPtrnGeneratorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPtrnGeneratorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CPtrnGeneratorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPtrnGeneratorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPtrnGeneratorView diagnostics

#ifdef _DEBUG
void CPtrnGeneratorView::AssertValid() const
{
	CView::AssertValid();
}

void CPtrnGeneratorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPtrnGeneratorDoc* CPtrnGeneratorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPtrnGeneratorDoc)));
	return (CPtrnGeneratorDoc*)m_pDocument;
}
#endif //_DEBUG


// CPtrnGeneratorView message handlers


void CPtrnGeneratorView::OnButtonStart()
{
	// TODO: Add your command handler code here
	m_pCurrentThread = AfxBeginThread(CPtrnGeneratorView::StartThread, this);
}


void CPtrnGeneratorView::OnButtonStop()
{
	// TODO: Add your command handler code here
	m_pCurrentThread->SuspendThread();
}


void CPtrnGeneratorView::OnButtonResume()
{
	// TODO: Add your command handler code here
	m_pCurrentThread->ResumeThread();
}


UINT CPtrnGeneratorView::StartThread(LPVOID Param)
{
	// TODO: Add your implementation code here.
	CPtrnGeneratorView* pView = (CPtrnGeneratorView*)Param;
	CPoint MyPoint(0, 0);
	while (1)
	{
		MyPoint.y = 200 + 50 * sin(pView->m_iCounter);
		MyPoint.x = 100 + pView->m_iCounter;
		pView->m_PointArray.Add(MyPoint);
		pView->m_iCounter = pView->m_iCounter + 5;
		pView->Invalidate();
		Sleep(100);

	}
	return 0;
}
