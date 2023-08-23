// // This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// // PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "pch.h"
#include "LEDWindow.h"
#include "PeacockSettingsDlg.h"
#include <iostream>
#include <thread>
#include <chrono>

float CorrectValue( float v );
LED hsv2rgb( float h, float s, float v );

// LEDWindow
LEDWindow* g_led_wnd = nullptr;

enum
{
	TIMER_ID = 1234
};

void CreateLedWindow()
{
	if( !g_led_wnd )
	{
		g_led_wnd = new LEDWindow;

		DWORD exStyle = 0;
		DWORD style = 0;
		LPCTSTR wcn = AfxRegisterWndClass( 0 );

		g_led_wnd->CreateEx( exStyle, wcn, L"LED", style, 0, 0, 0, 0, HWND_MESSAGE, 0 );

		g_led_wnd->ShowWindow( SW_HIDE );
		g_led_wnd->UpdateWindow();
	}
}

void DestroyLedWindow()
{
	if( g_led_wnd )
	{
		g_led_wnd->DestroyWindow();
		delete g_led_wnd;
		g_led_wnd = nullptr;
	}
}

LEDWindow* GetLedWindow()
{
	return g_led_wnd;
}


IMPLEMENT_DYNAMIC( LEDWindow, CWnd )

LEDWindow::LEDWindow()
	: _backlightSettings{}
	, _timer( 0 )
	, _waitingTime( 0 )
{

}

LEDWindow::~LEDWindow()
{
}

void LEDWindow::StartAnimation()
{
	if( _backlightSettings.dwLedsCount == 0 )
	{
		AfxMessageBox( L"Не задано количество светодиодов", MB_OK | MB_TOPMOST | MB_ICONERROR );
		return;
	}

	_leds.resize( _backlightSettings.dwLedsCount );
	_ledsHue.resize( _leds.size() );

	const float hueStep = 360.0f / (float)_ledsHue.size();

	for( size_t i = 0; i < _ledsHue.size(); ++i )
	{
		_ledsHue[i] = static_cast<float>( i ) * hueStep;
	}

	// частота обновления ленты не более 25 раз в секунду
	_waitingTime = (std::max)(40u, (UINT)_backlightSettings.dwMinEventTime);
	_timer = SetTimer(TIMER_ID, _waitingTime, nullptr); // 
	//SetTimer(1000, 3000, nullptr); // отладка 
	_pluginSettings.clock_time = 0;
}

void LEDWindow::StopAnimation()


{
	if( _timer )
	{
		KillTimer( _timer );
		_timer = 0;
	}
}

void LEDWindow::Save()
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState() );

	AfxGetApp()->WriteProfileInt(L"", L"frequency", _pluginSettings.frequency);
}

void LEDWindow::Load( BACKLIGHT_PLUGIN_SETTINGS& bs )
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState() );

	_pluginSettings.frequency = (int)AfxGetApp()->GetProfileInt(L"", L"frequency", 1);

	memcpy( &_backlightSettings, &bs, sizeof( _backlightSettings ) );

	if( _backlightSettings.dwEventMethod == 0 )
	{
		if( _backlightSettings.input_func == nullptr )
		{
			;// AfxMessageBox( L"No callback" );
		}
	}
	else
	{
		if( _backlightSettings.hWndEvent == nullptr )
		{
			;// AfxMessageBox( L"No WINDOW handle" );
		}
	}
}

void LEDWindow::EditSettings()
{
	{
		AFX_MANAGE_STATE( AfxGetStaticModuleState() );

		PeacockSettingsDlg dlg;

		const BYTE oldRed = _pluginSettings.red,
			    oldGreen = _pluginSettings.green,
				oldBlue = _pluginSettings.blue;

		const BOOL oldchangeRainbow = _pluginSettings.changeRainbow;
		const double oldfrequency = _pluginSettings.frequency;
		const int oldBPM = _pluginSettings.BPM; 
		const double oldduty_cycle = _pluginSettings.duty_cycle, 
			oldattack = _pluginSettings.attack,
			olddelay = _pluginSettings.delay; 

		dlg.changeRainbow = &_pluginSettings.changeRainbow;

		dlg.red = &_pluginSettings.red;
		dlg.green = &_pluginSettings.green;
		dlg.blue = &_pluginSettings.blue;

		dlg.frequency = &_pluginSettings.frequency;
		dlg.BPM = &_pluginSettings.BPM;
		dlg.attack = &_pluginSettings.attack;
		dlg.delay = &_pluginSettings.delay;
		dlg.duty_cycle = &_pluginSettings.duty_cycle;

		if( dlg.DoModal() != IDOK )
		{
			_pluginSettings.red = oldRed;
			_pluginSettings.blue = oldBlue;
			_pluginSettings.green = oldGreen;
			_pluginSettings.changeRainbow = oldchangeRainbow;
			_pluginSettings.frequency = oldfrequency;
			_pluginSettings.BPM = oldBPM;
			_pluginSettings.duty_cycle = oldduty_cycle;
			_pluginSettings.attack = oldattack;
			_pluginSettings.delay = olddelay;

		}
	}

	Save();
}

BEGIN_MESSAGE_MAP( LEDWindow, CWnd )
	ON_WM_POWERBROADCAST()
	ON_WM_TIMER()
END_MESSAGE_MAP()



void LEDWindow::GetRainbow(int x) { // По значению x мы выбираем цвет (и изменяем цвета в g_pluginSettings)
	x %= 1530; //ограничиваем 

	int layer = x / 255; // узнаём какой сектор

	x %= 255; // делаем до цветового диапазона

	switch (layer) {
	case 0: {
		_pluginSettings.red = 255;
		_pluginSettings.green = x;
		_pluginSettings.blue = 0;
		break;
	}
	case 1: {
		_pluginSettings.red = 255 - x;
		_pluginSettings.green = 255;
		_pluginSettings.blue = 0;
		break;
	}
	case 2: {
		_pluginSettings.red = 0;
		_pluginSettings.green = 255;
		_pluginSettings.blue = x;
		break;
	}
	case 3: {
		_pluginSettings.red = 0;
		_pluginSettings.green = 255 - x;
		_pluginSettings.blue = 255;
		break;
	}
	case 4: {
		_pluginSettings.red = x;
		_pluginSettings.green = 0;
		_pluginSettings.blue = 255;
		break;
	}
	case 5: {
		_pluginSettings.red = 255;
		_pluginSettings.green = 0;
		_pluginSettings.blue = 255 - x;
		break;
	}
	}
}

// LEDWindow message handlers

void LEDWindow::OnTimer( UINT_PTR nIDEvent ) // добавить фактор максимального положения кривой и минимальной
{
	double freq_sec = 1000. / _pluginSettings.frequency;
	int time_pwm = freq_sec * _pluginSettings.duty_cycle, freq_int = (int)freq_sec;
	double k, n;

	if( nIDEvent == TIMER_ID )
	{
		const size_t numLeds = _ledsHue.size();

		//

		_pluginSettings.clock_time += _waitingTime;


		if (_pluginSettings.changeRainbow)
		{
			GetRainbow(clock() / 30);
		}
		//
	
		LED ledrgb;

		if (_pluginSettings.clock_time % freq_int < time_pwm)
		{
			if (_pluginSettings.attack == 0) // Прямой фронт у шим
			{
				ledrgb.rgb[0] = _pluginSettings.red;
				ledrgb.rgb[1] = _pluginSettings.green;
				ledrgb.rgb[2] = _pluginSettings.blue;
			}
			else
			{
				k = (time_pwm * _pluginSettings.attack);
				if (((_pluginSettings.clock_time % freq_int) < k))
				{
					n = (double)_pluginSettings.red * ((double)(_pluginSettings.clock_time % freq_int) / k);
					ledrgb.rgb[0] = (int)n;
					n = (double)_pluginSettings.green * ((double)(_pluginSettings.clock_time % freq_int) / k);
					ledrgb.rgb[1] = (int)n;
					n = (double)_pluginSettings.blue * ((double)(_pluginSettings.clock_time % freq_int) / k);
					ledrgb.rgb[2] = (int)n;
				}
				else
				{
					ledrgb.rgb[0] = _pluginSettings.red;
					ledrgb.rgb[1] = _pluginSettings.green;
					ledrgb.rgb[2] = _pluginSettings.blue;
				}
			}
		}
		else
		{
			if (_pluginSettings.delay == 0) // Прямой спад у шим
			{
				ledrgb.rgb[0] = 0;
				ledrgb.rgb[1] = 0;
				ledrgb.rgb[2] = 0;
			}
			else
			{
				k = (freq_int - time_pwm) * _pluginSettings.delay;

				if ( (_pluginSettings.clock_time % freq_int) < (time_pwm + k) )
				{
					n = (double)_pluginSettings.red * (1 - (double)(_pluginSettings.clock_time % freq_int - time_pwm) / k);
					ledrgb.rgb[0] = (int)n;
					n = (double)_pluginSettings.green * (1 - (double)(_pluginSettings.clock_time % freq_int - time_pwm) / k);
					ledrgb.rgb[1] = (int)n;
					n = (double)_pluginSettings.blue * (1 - (double)(_pluginSettings.clock_time % freq_int - time_pwm) / k);
					ledrgb.rgb[2] = (int)n;
				}
				else
				{
					ledrgb.rgb[0] = 0;
					ledrgb.rgb[1] = 0;
					ledrgb.rgb[2] = 0;
				}
			}
		}
		

		for( size_t i = 0; i < numLeds; ++i )
		{
			_leds[i] = ledrgb;
		}

		if( _backlightSettings.dwEventMethod == 0 )
		{
			if( _backlightSettings.input_func )
			{
				_backlightSettings.input_func( (DWORD*)_leds.data(), ((DWORD)_leds.size()) * 3 );
			}
		}
		else
		{
			::SendMessage(
				_backlightSettings.hWndEvent,
				_backlightSettings.dwMessageID,
				(WPARAM)(_leds.size()) * 3,
				(LPARAM)(_leds.data())
			);
		}
	}
	if (nIDEvent == 1000)
	{
		CString strColor;
		strColor.Format(_T("%d, %d, %d"), _pluginSettings.clock_time, freq_int, time_pwm);

		AfxMessageBox(strColor);
	}//_pluginSettings.clock_time % freq_int < time_pwm

	CWnd::OnTimer( nIDEvent );
}



UINT LEDWindow::OnPowerBroadcast( UINT nPowerEvent, LPARAM nEventData )
{
	/*
	 wParam

PBT_APMPOWERSTATUSCHANGE
10 (0xA)

	Power status has changed.

PBT_APMRESUMEAUTOMATIC
18 (0x12)

	Operation is resuming automatically from a low-power state. This message is sent every time the system resumes.

PBT_APMRESUMESUSPEND
7 (0x7)

	Operation is resuming from a low-power state. This message is sent after PBT_APMRESUMEAUTOMATIC if the resume is triggered by user input, such as pressing a key.

PBT_APMSUSPEND
4 (0x4)

	System is suspending operation.

PBT_POWERSETTINGCHANGE
32787 (0x8013)

	A power setting change event has been received.
	 */
	return CWnd::OnPowerBroadcast( nPowerEvent, nEventData );
}


