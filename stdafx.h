#pragma once

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
#define MAX_REG_KEY_VALUE 32767

#include <windows.h>
#include <atlbase.h>
#include <comdef.h>
#include <statreg.h>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <exception>
#include <cassert>
#include "resource.h"

#include "GameProtocol.h"
#include "GameWindow.h"
#include "Player.h"
#include "ConnectFour.h"
#include "ConnectFourGame.h"

using std::string;