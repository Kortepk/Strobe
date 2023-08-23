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
	int BPM = 60; // Нужно только для нормального отображения в настройке плагина
	double duty_cycle = 0.5, // Скважность, базовый стробоскоп представляется как шим [0;1]
		   attack = 0, // Значение атаки [0;1] (Аналог adsr кривой)
		   delay = 0; //  затухание (выпуск) [0;1]
};
