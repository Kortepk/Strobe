// // This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// // PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once

#include <iostream>
#include <thread>
#include <chrono>

struct PluginSettings
{
	BOOL changeRainbow = FALSE;
	BYTE red = 255, green = 255, blue = 255;
	double frequency = 1; 
	int clock_time = 0;
	int BPM = 60; // ����� ������ ��� ����������� ����������� � ��������� �������
	double duty_cycle = 0.5, // ����������, ������� ���������� �������������� ��� ��� [0;1]
		   attack = 0, // �������� ����� [0;1] (������ adsr ������)
		   delay = 0; //  ��������� (������) [0;1]
};
