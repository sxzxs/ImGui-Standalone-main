#pragma once
#include <map>
#include <string>
#include "ImGui/imgui.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <codecvt>
#include <vector>

ImTextureID ImageFromFile(const wchar_t* path, ID3D11Device* g_pd3dDevice);
ImVec2 GetImageDimension(ImTextureID image);

typedef struct GIF_STRUCT
{
	std::vector<ID3D11ShaderResourceView*> srv_array{};
	int w;
	int h;
	int delay;
	int current_index{ 0 };
}GIF_STRUCT;

bool imgui_stbi__load_gif(const wchar_t* filename, ID3D11Device* g_pd3dDevice, GIF_STRUCT &out_gif);

void ImageGetSize(ImTextureID image, float* x, float* y);
//È¡Ïûsd¼ì²é
std::string wstr_utf8(const std::wstring& str);
std::wstring utf8_wstr(const std::string& str);

std::u16string to_utf16(std::string str); // utf-8 to utf16

std::string to_utf8(std::u16string str16);

std::u32string to_utf32(std::string str);

std::string to_utf8(std::u32string str32);

std::wstring to_wchar_t(std::string str);

std::string to_utf8(std::wstring wstr);
