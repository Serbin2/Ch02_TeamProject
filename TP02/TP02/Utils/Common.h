// [작성자] : 박재현 
// [DESC] : 리소스 매니저 전용 


// 기본 제공 헤더파일
#pragma once
#include <Windows.h>
#include <filesystem>
#include <wchar.h>
#include <string>
#include <map>
#include <unordered_map>
#include <memory>
#include <assert.h>
#include <iostream>

// 자체 제작 헤더파일 
#include "Utils.h"


// 파일시스템 네임스페이스 별칭 
namespace fs = std::filesystem;

// 사운드 관련 및 라이브러리 포함
#include <MMSystem.h>
#include <dsound.h>
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dsound.lib")