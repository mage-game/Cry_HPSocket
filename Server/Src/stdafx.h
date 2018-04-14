// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

//#include "targetver.h"
//
//#include <stdio.h>
//#include <tchar.h>

//#define _USE_MFC
#define _DETECT_MEMORY_LEAK

#include "GeneralHelper.h"
#include "easylogging++.h"

#define SAFE_DELETE(p)\
if(p)\
{\
	delete p;\
	p=nullptr;\
}
#define SAFE_DELETE_ARRAY(p)\
if(p)\
{\
	delete[] p;\
	p=nullptr;\
}

// TODO: reference additional headers your program requires here
