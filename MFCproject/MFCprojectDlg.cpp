
// MFCprojectDlg.cpp : implementation file
//

#include <string.h>
#include "stdafx.h"
#include "MFCproject.h"
#include "MFCprojectDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <sstream>

// CMFCprojectDlg dialog
CMFCprojectDlg::CMFCprojectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCPROJECT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCprojectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCprojectDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_CBN_SELCHANGE(IDC_COMBO2, &CMFCprojectDlg::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_CHECK1, &CMFCprojectDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(ID_FILE_NEW, &CMFCprojectDlg::OnFileNew)
	ON_BN_CLICKED(ID_FILE_SAVE, &CMFCprojectDlg::OnFileSave)
	ON_BN_CLICKED(ID_FILE_LOAD, &CMFCprojectDlg::OnFileLoad)
	ON_BN_CLICKED(IDC_RADIO1, &CMFCprojectDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CMFCprojectDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_CHECK2, &CMFCprojectDlg::OnBnClickedCheck2)
END_MESSAGE_MAP()


// CMFCprojectDlg message handlers

BOOL CMFCprojectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_ShapeSelect = (CComboBox*)GetDlgItem(IDC_COMBO2);
	m_ShapeSelect->SetCurSel(0);
	m_EraseCB = (CButton*)GetDlgItem(IDC_CHECK1);
	m_EraseOBRadio = (CButton*)GetDlgItem(IDC_RADIO1);
	m_EraseFreeRadio = (CButton*)GetDlgItem(IDC_RADIO2);
	m_MoveCB = (CButton*)GetDlgItem(IDC_CHECK2);
	m_CoordsTxt = (CStatic*)GetDlgItem(IDC_STATIC2);
	m_BGColorSelect = (CMFCColorButton*)GetDlgItem(IDC_MFCCOLORBUTTON1);
	m_BGColorSelect->SetColor(0xFFFFFF);
	m_SColorSelect = (CMFCColorButton*)GetDlgItem(IDC_MFCCOLORBUTTON2);
	m_SColorSelect->SetColor(0x000000);
	m_WidthSelect = (CComboBox*)GetDlgItem(IDC_COMBO3);
	m_WidthSelect->SetCurSel(1);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCprojectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // device context for painting
		for (int i = 0; i < figs.GetSize(); i++)
			figs[i]->Draw(&dc);
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCprojectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCprojectDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	start = point;
	isPressed = true;
	if (isErase) {
		for (int i = 0; i < figs.GetSize(); i++) {
			Figure* fig = figs.GetAt(i);
			if (fig->isInside(point)) {
				figs.RemoveAt(i);
				Invalidate();
				break;
			}
		}
	} else if (isMove) {
		for (int i = 0; i < figs.GetSize(); i++) {
			Figure* fig = figs.GetAt(i);
			if (fig->isInside(point)) {
				movingFig = fig;
				break;
			}
		}
	} else {
		switch (futureFigureKind) {
			default:
			case 0:
				figs.Add(new RectangleF(start, start));
				break;
			case 1:
				figs.Add(new EllipseF(start, start));
				break;
			case 2:
				figs.Add(new TriangleF(start, start));
				break;
			case 3:
				figs.Add(new LineF(start, start));
				break;
		}
		figs[figs.GetSize() - 1]->SetBGColor(m_BGColorSelect->GetColor());
		figs[figs.GetSize() - 1]->SetSColor(m_SColorSelect->GetColor());
		figs[figs.GetSize() - 1]->SetSWidth(m_WidthSelect->GetCurSel());
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMFCprojectDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (isPressed) {
		isPressed = false;
		if (isMove) {
			movingFig = NULL;
		} else if (!isErase) {
			end = point;
			figs[figs.GetSize() - 1]->Redefine(start, end);
			Invalidate(); //simulates the WM_PAINT message to redraw window
		}
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CMFCprojectDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	std::stringstream str;
	str << point.x << ":" << point.y;
	char* cstr = new char[20];
	strcpy(cstr, str.str().c_str());
	m_CoordsTxt->SetWindowText(CA2W(cstr));
	delete[] cstr;

	if (isPressed) {
		if (isErase) {
			for (int i = 0; i < figs.GetSize(); i++) {
				Figure* fig = figs.GetAt(i);
				if (fig->isInside(point)) {
					figs.RemoveAt(i);
					Invalidate();
					break;
				}
			}
		} else if (isMove) {
			if (movingFig != NULL) {
				end = point;
				movingFig->Shift(end.x - start.x, end.y - start.y);
				start = end;
				Invalidate();
			}
		} else {
			end = point;
			figs[figs.GetSize() - 1]->Redefine(start, end);
			Invalidate(); //simulates the WM_PAINT message to redraw window
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void CMFCprojectDlg::OnCbnSelchangeCombo2() {
	futureFigureKind = m_ShapeSelect->GetCurSel();
}

void CMFCprojectDlg::OnBnClickedCheck1() {
	isErase = m_EraseCB->GetCheck() == BST_CHECKED;

	if (isErase) {
		m_EraseOBRadio->EnableWindow(TRUE);
		m_EraseOBRadio->SetCheck(BST_CHECKED);
		m_EraseFreeRadio->EnableWindow(TRUE);
		m_EraseFreeRadio->SetCheck(BST_UNCHECKED);
		m_MoveCB->SetCheck(BST_UNCHECKED);
		isMove = false;
		isEraseFreeFrm = false;
	} else {
		m_EraseOBRadio->EnableWindow(FALSE);
		m_EraseFreeRadio->EnableWindow(FALSE);
	}
}

void CMFCprojectDlg::OnBnClickedRadio1()
{
	isEraseFreeFrm = m_EraseFreeRadio->GetCheck() == BST_CHECKED;
}

void CMFCprojectDlg::OnBnClickedRadio2()
{
	isEraseFreeFrm = m_EraseFreeRadio->GetCheck() == BST_CHECKED;
}

void CMFCprojectDlg::OnBnClickedCheck2()
{
	isMove = m_MoveCB->GetCheck() == BST_CHECKED;
	if (isMove) {
		m_EraseOBRadio->EnableWindow(FALSE);
		m_EraseFreeRadio->EnableWindow(FALSE);
		m_EraseCB->SetCheck(BST_UNCHECKED);
		isErase = false;
	}
}

// menu

void CMFCprojectDlg::OnFileNew() {
	figs.RemoveAll();
	Invalidate();
}

void CMFCprojectDlg::OnFileSave() {
	// FALSE to SAVE
	CFileDialog dlg(FALSE, _T(".figs"), NULL, 0, _T("Figures (*.figs)|*.figs|All Files (*.*)|*.*||"));
	CString filename;
	if (dlg.DoModal() == IDOK)
	{
		filename = dlg.GetPathName(); // return full path and filename
		CFile file(filename, CFile::modeCreate | CFile::modeWrite);
		CArchive ar(&file, CArchive::store);
		figs.Serialize(ar);
		ar.Close();
		file.Close();
	}
}

void CMFCprojectDlg::OnFileLoad() {
	// TRUE to LOAD
	CFileDialog dlg(TRUE, _T(".figs"), NULL, 0, _T("Figures (*.figs)|*.figs|All Files (*.*)|*.*||"));
	CString filename;
	if (dlg.DoModal() == IDOK)
	{
		filename = dlg.GetPathName(); // return full path and filename
		CFile file(filename, CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		figs.Serialize(ar);
		ar.Close();
		file.Close();
		Invalidate();
	}
}