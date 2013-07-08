// Include file for standard system include files
#pragma once

#include <stdio.h>
#include <Windows.h>
#include <TCHAR.H>
#include "shobjidl.h"

// Include SFML headers
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\Audio.hpp>

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

#include "tinyxml2.h"
#include "GlobalFunctions.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_BPP = 32;

const int FRICTION = 1000.0f;
const int GRAVITY = 900.0f;

const COMDLG_FILTERSPEC c_rgSaveTypes[] =
{
    {L"XML (*.xml)",       L"*.xml"}
};

