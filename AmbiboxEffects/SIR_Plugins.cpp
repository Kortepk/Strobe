// // This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// // PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "pch.h"
#include "SIR_Plugins.h"
#include "LEDWindow.h"

extern "C"
{
	//---------------------------------------------------------------------------------------------------------------------------
	__declspec(dllexport) wchar_t* __stdcall SIR_GetName(void)
	{
		return L"Стробоскоп";
	}

	__declspec(dllexport) wchar_t* __stdcall SIR_GetVersion(void)
	{
		return L"1.0.0";
	}

	__declspec(dllexport) wchar_t* __stdcall SIR_GetDescription(void)
	{
		return L"Эффект стробоскопа\nОтдельная благодарность:\n\rmaaGames\n\rhttps://vk.com/maagames";
	}

	__declspec(dllexport) int __stdcall SIR_Init( void )
	{
		return 0; // Success, no errors
		//return ErrorCode; // Error code > 0
	}

	__declspec(dllexport) int __stdcall SIR_UnInit( void )
	{
		return 0; // Success, no errors
		//return ErrorCode; // Error code > 0
	}

	__declspec(dllexport) int __stdcall SIR_Start( void )
	{
		if( auto wnd = GetLedWindow() )
			wnd->StartAnimation();

		return 0; // Success, no errors
		//return ErrorCode; // Error code > 0
	}

	__declspec(dllexport) int __stdcall SIR_Stop( void )
	{
		if( auto wnd = GetLedWindow() )
			wnd->StopAnimation();

		return 0; // Success, no errors
		//return ErrorCode; // Error code > 0
	}

	__declspec(dllexport) int __stdcall SIR_SetBacklightSettings( LPBACKLIGHT_PLUGIN_SETTINGS settings )
	{
		if( auto wnd = GetLedWindow() )
			wnd->Load( *settings );

		return 0; // Success, no errors
		//return ErrorCode; // Error code > 0
	}

	__declspec(dllexport) int __stdcall SIR_ShowSettings( HWND hWndParent )
	{
		if( auto wnd = GetLedWindow() )
			wnd->EditSettings();

		return 0;
	}

	__declspec(dllexport) int __stdcall SIR_SaveSettings( void )
	{
		return 0; // Success, no errors
		//return ErrorCode; // Error code > 0
	}
}

