// // This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// // PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once


class PeacockSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(PeacockSettingsDlg)

public:
	PeacockSettingsDlg(CWnd* pParent = nullptr);
	virtual ~PeacockSettingsDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOrientation();
	afx_msg void OnEnUpdateTime();

public:
	BOOL * changeRainbow, expand = true, start = true;
	CEdit Freq_box;
	CEdit BPM_box;
	CBrush m_brusahColor;
	BYTE* red = 0, * green = 0, * blue = 0;
	double *frequency, *duty_cycle, *attack, *delay;
	int* BPM;
	void Draw_pict1();
	void Draw_pict2();
	void Draw_pict3();
	void Draw_pictRes();
	CMFCColorButton m_mfcColorButton;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedMfccolorbutton1();
	afx_msg void OnBnClickedOk();
	CButton Pwm1;
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnNMReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMReleasedcaptureSlider2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMReleasedcaptureSlider3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedCheck2();
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
};
