// // This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// // PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "pch.h"
#include "PeacockSettingsDlg.h"
#include "resource.h"


IMPLEMENT_DYNAMIC(PeacockSettingsDlg, CDialog)

PeacockSettingsDlg::PeacockSettingsDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_SETTINGS, pParent)
	, frequency( nullptr )
{
	
}





PeacockSettingsDlg::~PeacockSettingsDlg()
{
}



void PeacockSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK2, *changeRainbow);
	//DDX_Text(pDX, IDC_EDIT1, *frequency);
	//DDV_MinMaxInt(pDX, *frequency, 0, 60);
	DDX_Control(pDX, IDC_EDIT1, Freq_box);
	DDX_Text(pDX, IDC_EDIT1, *frequency);
	DDX_Control(pDX, IDC_EDIT2, BPM_box);
	DDX_Text(pDX, IDC_EDIT2, *BPM);
	DDV_MinMaxInt(pDX, *BPM, 1, 1800);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, m_mfcColorButton);
	DDX_Control(pDX, IDCANCEL, Pwm1);
}

BEGIN_MESSAGE_MAP(PeacockSettingsDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, &PeacockSettingsDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &PeacockSettingsDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON1, &PeacockSettingsDlg::OnBnClickedMfccolorbutton1)
	ON_BN_CLICKED(IDOK, &PeacockSettingsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK3, &PeacockSettingsDlg::OnBnClickedCheck3)
ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &PeacockSettingsDlg::OnNMReleasedcaptureSlider1)
ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER2, &PeacockSettingsDlg::OnNMReleasedcaptureSlider2)
ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER3, &PeacockSettingsDlg::OnNMReleasedcaptureSlider3)
ON_BN_CLICKED(IDC_CHECK2, &PeacockSettingsDlg::OnBnClickedCheck2)
//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &PeacockSettingsDlg::OnNMCustomdrawSlider2)
//ON_WM_INITMENU()
//ON_WM_ENTERIDLE()
//ON_WM_NCPAINT()
//ON_WM_ACTIVATE()
//ON_WM_NCHITTEST()
ON_WM_NCCREATE()
END_MESSAGE_MAP()



void PeacockSettingsDlg::OnBnClickedOrientation()
{
	UpdateData();
}

void PeacockSettingsDlg::OnEnUpdateTime()
{
	UpdateData();
}


void PeacockSettingsDlg::OnEnChangeEdit1()
{
	CString strValue, a;
	Freq_box.GetWindowText(strValue);

	// Проверка наличия символа и его удаление
	int old = strValue.GetLength(), i;
	bool cnt = false;

	for (i = 0; i < strValue.GetLength(); i++)
	{
		TCHAR Chr = strValue.GetAt(i);
		a = (TCHAR)(48 + i);
		//AfxMessageBox(a);
		if (!isdigit(Chr) && Chr != '.' && Chr != ',')
		{
			strValue.Delete(i);
			i--;
			continue;
		}

		if ((Chr == '.' || Chr == ',') && i == 0)
		{
			i--;
			strValue.Delete(i);
			continue;
		}

		if ((Chr == '.' || Chr == ',') && cnt)
		{
			i--;
			strValue.Delete(i);
			continue;
		}

		if (Chr == '.' || Chr == ',')
			cnt = true;
	}

	if (strValue.GetLength() != old)
		Freq_box.SetWindowText(strValue);

	double value = _ttof(strValue);

	if (value > 20)
		value = 20;

	if (value != *frequency)
	{
		*frequency = value;

		value *= 60; // Перевод в BPM

		int roundedValue = (int)round(value);// Округление числа
		*BPM = roundedValue;
		CString strRoundedValue; // Преобразование округленного числа в CString
		strRoundedValue.Format(_T("%d"), roundedValue);

		BPM_box.SetWindowText(strRoundedValue);	// Запись округленного числа в Edit control
	}

	// Решение бага
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);

	// Получение текущей длины текста
	int textLength = pEdit->GetWindowTextLength();

	// Установка выделения в конец текста
	pEdit->SetSel(textLength, textLength);

	// Установка фокуса на объекте редактирования
	pEdit->SetFocus();
}


void PeacockSettingsDlg::OnEnChangeEdit2()
{
	// BPM_box
	CString strValue, a;
	BPM_box.GetWindowText(strValue);
	double value = _ttof(strValue);
	if (value > 1800)
		value = 1800;
	if (value != *BPM)
	{
		*BPM = value;
		value /= 60;
		*frequency = value;

		CString strRoundedValue;
		strRoundedValue.Format(_T("%g"), value);

		Freq_box.SetWindowText(strRoundedValue);
	}
	// Решение бага
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT2);

	// Получение текущей длины текста
	int textLength = pEdit->GetWindowTextLength();

	// Установка выделения в конец текста
	pEdit->SetSel(textLength, textLength);

	// Установка фокуса на объекте редактирования
	pEdit->SetFocus();


}


void PeacockSettingsDlg::OnBnClickedMfccolorbutton1()
{
	m_brusahColor.DeleteObject();
	m_brusahColor.CreateSolidBrush(m_mfcColorButton.GetColor());

	LOGBRUSH logBrush;
	m_brusahColor.GetLogBrush(&logBrush);

	COLORREF colorRef = logBrush.lbColor;

	*red = GetRValue(colorRef);
	*green = GetGValue(colorRef);
	*blue = GetBValue(colorRef);

	CString strColor;
	strColor.Format(_T("rgb(%d, %d, %d)"), *red, *green, *blue);

	AfxMessageBox(strColor);
}


void PeacockSettingsDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	CDialog::OnOK();
}



void PeacockSettingsDlg::OnBnClickedCheck3()
{
	// Получение размеров клиентской области окна
	CRect clientRect;
	GetClientRect(&clientRect);
	int clientWidth = clientRect.Width(); // Ширина клиентской области окна
	int clientHeight = clientRect.Height(); // Высота клиентской 

	if (expand)
	{
		CWnd* pWnd = GetDlgItem(IDD_SETTINGS); // Замените IDD_MY_DIALOG на идентификатор вашего диалогового окна

		// Задание новых размеров окна
		CRect rect;
		GetWindowRect(&rect);  // Получение текущих размеров окна

		int newWidth = 536; // Новая ширина окна
		int newHeight = 315; // Новая высота окна

		SetWindowPos(NULL, 0, 0, newWidth, newHeight, SWP_NOMOVE);


		CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1); // IDC_SLIDER - идентификатор слайдера

		int nRangeMin, nRangeMax;
		pSlider->GetRange(nRangeMin, nRangeMax); // Получаем диапазон значений слайдера

		int nValue = (nRangeMax - nRangeMin) / 2; // Устанавливаем значение слайдера в середину диапазона
		pSlider->SetPos(nValue); // Устанавливаем значение слайдера


		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER2); // IDC_SLIDER - идентификатор слайдера
		pSlider->SetPos(0); // Устанавливаем значение слайдера

		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER3); // IDC_SLIDER - идентификатор слайдера
		pSlider->SetPos(0); // Устанавливаем значение слайдера

		Draw_pict1();
		Draw_pict2();
		Draw_pict3();

		expand = false;
	}
	else
	{
		CRect rect;
		GetWindowRect(&rect);  // Получение текущих размеров окна

		int newWidth = 234; // Новая ширина окна
		int newHeight = 315; // Новая высота окна

		SetWindowPos(NULL, 0, 0, newWidth, newHeight, SWP_NOMOVE);
		
		expand = true;
	}
	
}





int CalcIndex(int x, int y, int pitch)
{
	return y * pitch + x * 4;
}

void SetArea(BYTE* img, int pitch, int x, int y, BYTE red, BYTE green, BYTE blue)
{
	byte phase = 0;
	try
	{
		for (phase = 0; phase < 9; phase++)
		{
			switch (phase)
			{
				case 0: // Центр
				{
					break;
				}
				case 1: // Право-нижний угол и далее по часовой идем
				{
					x++;
					y++;
					break;
				}
				case 2:
				{
					x--;
					break;
				}
				case 3: // Лево-нижний угол и далее по часовой идем
				{
					x--;
					break;
				}
				case 4:
				{
					y--;
					break;
				}
				case 5: // Лево-верхний угол и далее по часовой идем
				{
					y--;
					break;
				}
				case 6:
				{
					x++;
					break;
				}
				case 7: // Право-верхний угол и далее по часовой идем
				{
					x++;
					break;
				}
				case 8:
				{
					y++;
					break;
				}
			}

			//CString strColor;
			//strColor.Format(_T("(%d), (%d, %d)"), phase, x, y);

			//AfxMessageBox(strColor);

			int pixelIndex = CalcIndex(x, y, pitch);
			img[pixelIndex] = 240;
			img[pixelIndex + 1] = 194;
			img[pixelIndex + 2] = 125;
		}
	}
	catch (CString error_message)
	{
		AfxMessageBox(error_message);
	}

}

void DrawLine(BYTE* img, int pitch, int x1, int y1, int x2, int y2, BYTE red, BYTE green, BYTE blue)
{
	float dx = x2 - x1, dy = y2 - y1, dt;

	if (dy != 0) // Тут можно сделать более нормальную частную обработку случаев 
		dt = dx / dy;
	else
		return;

	if (dy > 0)
		for (int y = 0, x = 0; y < dy; y++)
		{
			x = (int)(dt * y);
			SetArea(img, pitch, x1 + x, y1 + y, red, green, blue);
		}
	else
		for (int y = 0, x = 0; y > dy; y--)
		{
			x = (int)(dt * y);
			SetArea(img, pitch, x1 + x, y1 + y, red, green, blue);
		}
}

void ClearImage(BYTE* imageData, int pitch, int wid, int hei)
{
	for (int x = 0; x < wid; x++)
		for (int y = 0; y < hei; y++)
		{
			int pixelIndex = y * pitch + x * 4;

			if (x == 0 || x == wid - 1 || y == 0 || y == hei - 1)
			{
				imageData[pixelIndex] = 220;
				imageData[pixelIndex + 1] = 220;
				imageData[pixelIndex + 2] = 220;
			}
			else
			{ 
				imageData[pixelIndex] = 240;
				imageData[pixelIndex + 1] = 240;
				imageData[pixelIndex + 2] = 240;
			}
		}
}

void PeacockSettingsDlg::Draw_pict1()
{
	CImage image;
	image.Create(100, 50, 32); // создание изображения размером 100x100 и глубиной цвета 32 бита

	// получение указателя на байты пикселей изображения
	BYTE* imageData = (BYTE*)image.GetBits();
	int pitch = image.GetPitch();

	//=========
	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1); // IDC_SLIDER - идентификатор слайдера

	//=========
	int nRangeMin, nRangeMax;
	pSlider->GetRange(nRangeMin, nRangeMax);

	int Sl1Val = pSlider->GetPos();
	*duty_cycle = (double)(Sl1Val - nRangeMin) / (nRangeMax - nRangeMin);

	float tmp = Sl1Val * 0.9f + 5;
	int tm = (int)tmp, pixelIndex;
	
	ClearImage(imageData, pitch, image.GetWidth(), image.GetHeight());

	for (int x = 5; x < image.GetWidth() - 3; x++)
	{ 
		tm = (x - 6) * 100 / 90;
		if (tm < Sl1Val) // 125, 194, 240
		{
			for (int y = 4; y < 7; y++)
			{
				pixelIndex = y * pitch + x * 4;
				imageData[pixelIndex] = 240;
				imageData[pixelIndex + 1] = 194;
				imageData[pixelIndex + 2] = 125;
			}
		}
		tm = (x - 3) * 100 / 90;
		if (tm > Sl1Val) 
		{
			for (int y = image.GetHeight() - 6; y < image.GetHeight() - 3; y++)
			{
				pixelIndex = y * pitch + x * 4;
				imageData[pixelIndex] = 240;
				imageData[pixelIndex + 1] = 194;
				imageData[pixelIndex + 2] = 125;
			}
		}
	}

	tm = tmp;
	for (int y = 5; y < image.GetHeight() - 4; y++)
	{
		for (int x = 4; x <= 6; x ++)
		{
			pixelIndex = y * pitch + x * 4;
			imageData[pixelIndex] = 240;
			imageData[pixelIndex + 1] = 194;
			imageData[pixelIndex + 2] = 125;
		}

		for (int x = tm - 1; x <= tm + 1; x++)
		{
			pixelIndex = y * pitch + x * 4;
			imageData[pixelIndex] = 240;
			imageData[pixelIndex + 1] = 194;
			imageData[pixelIndex + 2] = 125;
		}

		for (int x = image.GetWidth() - 6; x <= image.GetWidth() - 4; x++)
		{
			pixelIndex = y * pitch + x * 4;
			imageData[pixelIndex] = 240;
			imageData[pixelIndex + 1] = 194;
			imageData[pixelIndex + 2] = 125;
		}
	}

	// Отображение изображения в окне CView
	CDC* pViewDC = GetDC();

	CRect rect;
	GetClientRect(&rect);
	image.BitBlt(pViewDC->GetSafeHdc(), 360, 50);

	//image.BitBlt(pViewDC->GetSafeHdc(), 360, 120);
	//image.BitBlt(pViewDC->GetSafeHdc(), 360, 190);
	//image.BitBlt(pViewDC->GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 10, 10);

	// Освобождение контекста устройства окна
	ReleaseDC(pViewDC);
}


void PeacockSettingsDlg::Draw_pict2()
{
	CImage image;
	image.Create(100, 50, 32); // создание изображения размером 100x100 и глубиной цвета 32 бита

	// получение указателя на байты пикселей изображения
	BYTE* imageData = (BYTE*)image.GetBits();
	int pitch = image.GetPitch();

	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1); // IDC_SLIDER - идентификатор слайдера
	
	int nRangeMin, nRangeMax;
	pSlider->GetRange(nRangeMin, nRangeMax);

	int Sl1Val = pSlider->GetPos();
	*duty_cycle = (double)(Sl1Val - nRangeMin) / (nRangeMax - nRangeMin);

	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER2); 
	pSlider->GetRange(nRangeMin, nRangeMax);
	int Sl2Val = pSlider->GetPos();
	*delay = (double)(Sl2Val - nRangeMin) / (nRangeMax - nRangeMin);

	float tmp = Sl1Val * (image.GetWidth() - 10) * 0.01f + 5;
	int pixelIndex, tm = tmp, remains;
	remains = image.GetWidth() - 10 - tm;

	ClearImage(imageData, pitch, image.GetWidth(), image.GetHeight());

	for (int x = 5; x < image.GetWidth() - 3; x++)
	{
		if (x < tmp)
		{
			for (int y = 4; y <= 6; y++)
			{
				pixelIndex = y * pitch + x * 4;
				imageData[pixelIndex] = 240;
				imageData[pixelIndex + 1] = 194;
				imageData[pixelIndex + 2] = 125;
			}
		}

		if (x > tmp + (remains + 3) * (Sl2Val * 0.01f))
		{
			for (int y = image.GetHeight() - 6; y <= image.GetHeight() - 4; y++)
			{
				pixelIndex = y * pitch + x * 4;
				imageData[pixelIndex] = 240;
				imageData[pixelIndex + 1] = 194;
				imageData[pixelIndex + 2] = 125;
			}
		}
	}

	for (int y = 5; y < image.GetHeight() - 4; y++)
		for (int x = 4; x <= 6; x++)
		{
			pixelIndex = y * pitch + x * 4;
			imageData[pixelIndex] = 240;
			imageData[pixelIndex + 1] = 194;
			imageData[pixelIndex + 2] = 125;
		}

	DrawLine(imageData, pitch, tm, 5, tm + ( (remains + 5)*(Sl2Val * 0.01f) ), image.GetHeight() - 5, 125, 194, 240);
	
	// Отображение изображения в окне CView
	CDC* pViewDC = GetDC();

	CRect rect;
	GetClientRect(&rect);
	image.BitBlt(pViewDC->GetSafeHdc(), 360, 120);
	ReleaseDC(pViewDC);
}



void PeacockSettingsDlg::Draw_pict3()
{
	CImage image;
	image.Create(100, 50, 32); // создание изображения размером 100x100 и глубиной цвета 32 бита

	// получение указателя на байты пикселей изображения
	BYTE* imageData = (BYTE*)image.GetBits();
	int pitch = image.GetPitch();

	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1); // IDC_SLIDER - идентификатор слайдера
	int nRangeMin, nRangeMax;
	pSlider->GetRange(nRangeMin, nRangeMax);

	int Sl1Val = pSlider->GetPos();
	*duty_cycle = (double)(Sl1Val - nRangeMin) / (nRangeMax - nRangeMin);

	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER2);
	int Sl2Val = pSlider->GetPos();
	pSlider->GetRange(nRangeMin, nRangeMax);
	*delay = (double)(Sl2Val - nRangeMin) / (nRangeMax - nRangeMin);

	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER3);
	int Sl3Val = pSlider->GetPos();
	pSlider->GetRange(nRangeMin, nRangeMax);
	*attack = (double)(Sl3Val - nRangeMin) / (nRangeMax - nRangeMin);

	float tmp = Sl1Val * (image.GetWidth() - 10) * 0.01f + 5;
	int pixelIndex, tm = tmp, remains, attack;
	remains = image.GetWidth() - 10 - tm;
	remains = tm + (int)((remains + 4) * (Sl2Val * 0.01f));
	attack = 5 + (int)((tm - 5) * Sl3Val * 0.01f);

	ClearImage(imageData, pitch, image.GetWidth(), image.GetHeight());

	for (int x = 5; x < image.GetWidth() - 3; x++)
	{
		if (attack <=  x && x < tm)
		{
			for (int y = 4; y <= 6; y++)
			{
				pixelIndex = y * pitch + x * 4;
				imageData[pixelIndex] = 240;
				imageData[pixelIndex + 1] = 194;
				imageData[pixelIndex + 2] = 125;
			}
		}

		if (x >= remains)
		{
			for (int y = image.GetHeight() - 6; y <= image.GetHeight() - 4; y++)
			{
				pixelIndex = y * pitch + x * 4;
				imageData[pixelIndex] = 240;
				imageData[pixelIndex + 1] = 194;
				imageData[pixelIndex + 2] = 125;
			}
		}
	}


	DrawLine(imageData, pitch, tm, 5, remains, image.GetHeight() - 5, 125, 194, 240);
	DrawLine(imageData, pitch, 5, image.GetHeight() - 5, attack, 5, 125, 194, 240);

	// Отображение изображения в окне CView
	CDC* pViewDC = GetDC();

	CRect rect;
	GetClientRect(&rect);
	image.BitBlt(pViewDC->GetSafeHdc(), 360, 190);
	ReleaseDC(pViewDC);
}



void PeacockSettingsDlg::Draw_pictRes()
{
	CImage image;
	image.Create(100, 50, 32); // создание изображения размером 100x100 и глубиной цвета 32 бита

	// получение указателя на байты пикселей изображения
	BYTE* imageData = (BYTE*)image.GetBits();
	int pitch = image.GetPitch();

	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1); // IDC_SLIDER - идентификатор слайдера
	int Sl1Val = pSlider->GetPos();

	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER2);
	int Sl2Val = pSlider->GetPos();

	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER3);
	int Sl3Val = pSlider->GetPos();

	float tmp = Sl1Val * (image.GetWidth() - 10) * 0.01f + 5;
	int pixelIndex, tm = tmp, remains, attack;
	remains = image.GetWidth() - 10 - tm;
	remains = tm + (int)((remains + 4) * (Sl2Val * 0.01f));
	attack = 5 + (int)((tm - 5) * Sl3Val * 0.01f);

	ClearImage(imageData, pitch, image.GetWidth(), image.GetHeight());

	for (int x = 5; x < image.GetWidth() - 3; x++)
	{
		if (attack <= x && x < tm)
		{
			for (int y = 4; y <= 6; y++)
			{
				pixelIndex = y * pitch + x * 4;
				imageData[pixelIndex] = 240;
				imageData[pixelIndex + 1] = 194;
				imageData[pixelIndex + 2] = 125;
			}
		}

		if (x >= remains)
		{
			for (int y = image.GetHeight() - 6; y <= image.GetHeight() - 4; y++)
			{
				pixelIndex = y * pitch + x * 4;
				imageData[pixelIndex] = 240;
				imageData[pixelIndex + 1] = 194;
				imageData[pixelIndex + 2] = 125;
			}
		}
	}


	DrawLine(imageData, pitch, tm, 5, remains, image.GetHeight() - 5, 125, 194, 240);
	DrawLine(imageData, pitch, 5, image.GetHeight() - 5, attack, 5, 125, 194, 240);

	// Отображение изображения в окне CView
	CDC* pViewDC = GetDC();

	CRect rect;
	GetClientRect(&rect);
	image.BitBlt(pViewDC->GetSafeHdc(), 65, 210);
	ReleaseDC(pViewDC);
}


void PeacockSettingsDlg::OnNMReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult)
{
	Draw_pict1();
	Draw_pict2();
	Draw_pict3();

	*pResult = 0;
}


void PeacockSettingsDlg::OnNMReleasedcaptureSlider2(NMHDR* pNMHDR, LRESULT* pResult)
{
	Draw_pict2();
	Draw_pict3();

	*pResult = 0;
}


void PeacockSettingsDlg::OnNMReleasedcaptureSlider3(NMHDR* pNMHDR, LRESULT* pResult)
{
	Draw_pict3();

	*pResult = 0;
}
 



void PeacockSettingsDlg::OnBnClickedCheck2()
{
	UpdateData();
}






//LRESULT PeacockSettingsDlg::OnNcHitTest(CPoint point)
//{
//	
//
//	return CDialog::OnNcHitTest(point);
//}


BOOL PeacockSettingsDlg::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (expand && start)
	{
		CRect rect;
		GetWindowRect(&rect);  // Получение текущих размеров окна

		int newWidth = 234; // Новая ширина окна
		int newHeight = 315; // Новая высота окна

		SetWindowPos(NULL, 0, 0, newWidth, newHeight, SWP_NOMOVE);
		start = false;
	}



	if (!CDialog::OnNcCreate(lpCreateStruct))
		return FALSE;

	// TODO:  Добавьте специализированный код создания

	return TRUE;
}
