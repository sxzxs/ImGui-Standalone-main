#include "utility.h"
#include "stb_image.h"
#include "stb_gif.h"
#include <iostream>

namespace
{
	std::map<std::wstring, ImTextureID> MapImageFromFile;
}
ImTextureID ImageFromFile(const wchar_t* path, ID3D11Device *g_pd3dDevice)
{
	auto find = MapImageFromFile.find(path);
	if ( find == MapImageFromFile.end()) {
		ID3D11ShaderResourceView* texture;
		HRESULT result2;
		HRESULT result1 = D3DX11CreateShaderResourceViewFromFileW(g_pd3dDevice, path, NULL, NULL, &texture, &result2);
		ImTextureID tid = texture;

		if (result1 != S_OK) return 0;

		MapImageFromFile[path] = tid;
		return tid;
	}
	else {
		return find->second;
	}
}

ImVec2 GetImageDimension(ImTextureID image) 
{

	ID3D11ShaderResourceView* a = (ID3D11ShaderResourceView*)image;
	ID3D11Texture2D* pTextureInterface = 0;
	ID3D11Resource* res;
	a->GetResource(&res);
	res->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
	D3D11_TEXTURE2D_DESC desc;
	pTextureInterface->GetDesc(&desc);
	return { float(desc.Width), float(desc.Height) };
}

void ImageGetSize(ImTextureID image, float *x, float*y)
{
	ImVec2 pos = GetImageDimension(image); *x = pos.x; *y = pos.y;
}

bool imgui_stbi__load_gif(const wchar_t* filename, ID3D11Device* g_pd3dDevice, GIF_STRUCT &out_gif)
{
	int out_w{0};
	int out_h{0};
	int out_delay{0};

	int index = 0;
	int req_comp = 4;
	int c = 4;
	stbgif_data out_gif_data{};
	ID3D11ShaderResourceView* out_srv = nullptr;
	stbgif__context *s = new stbgif__context;
	ZeroMemory(s, sizeof(stbgif__context));
	stbi__result_info gg = {};
	int** delays = NULL;
	if (stbgif_test(s))
		std::cout << " error" << std::endl;
	unsigned char* image_data = NULL;

	FILE *f = stbgif__fopen(wstr_utf8(filename).c_str(), "rb");

	stbgif__start_file(s, f);
	bool is_set_data = false;


	D3D11_TEXTURE2D_DESC desc{};
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	ID3D11Texture2D *pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource{};
	while (image_data = stbgif__load_next(s, &out_gif_data, &c, req_comp, NULL))
	{
		if (image_data == (stbgif_uc*)s)
		{
			image_data = 0;
			break;
		}
         image_data = stbgif__convert_format(image_data, 4, req_comp, out_gif_data.w, out_gif_data.h);
		if (!is_set_data)
		{
			out_w = out_gif_data.w;
			out_h = out_gif_data.h;
			out_delay = out_gif_data.delay;
			is_set_data = true;
		}
		// Create texture
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = out_gif_data.w;
		desc.Height = out_gif_data.h;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		pTexture = NULL;
		ZeroMemory(&subResource, sizeof(subResource));
		subResource.pSysMem = image_data;
		subResource.SysMemPitch = desc.Width * 4;
		subResource.SysMemSlicePitch = 0;
		g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, &out_srv);
		out_gif.srv_array.push_back(out_srv);
		index++;
		if(pTexture != NULL)
			pTexture->Release();

		//stbgif_image_free(image_data);
	}
	if (s != nullptr)
		delete s;
	out_gif.delay = out_delay;
	out_gif.h = out_h;
	out_gif.w = out_w;
	return true;
}

//È¡Ïûsd¼ì²é
std::string wstr_utf8(const std::wstring& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.to_bytes(str);
}
std::wstring utf8_wstr(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.from_bytes(str);
}

std::u16string to_utf16(std::string str) // utf-8 to utf16
{
	return std::wstring_convert< std::codecvt_utf8_utf16<char16_t>, char16_t >{}.from_bytes(str);
}

std::string to_utf8(std::u16string str16)
{
	return std::wstring_convert< std::codecvt_utf8_utf16<char16_t>, char16_t >{}.to_bytes(str16);
}

std::u32string to_utf32(std::string str)
{
	return std::wstring_convert< std::codecvt_utf8<char32_t>, char32_t >{}.from_bytes(str);
}

std::string to_utf8(std::u32string str32)
{
	return std::wstring_convert< std::codecvt_utf8<char32_t>, char32_t >{}.to_bytes(str32);
}

std::wstring to_wchar_t(std::string str)
{
	return std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t >{}.from_bytes(str);
}

std::string to_utf8(std::wstring wstr)
{
	return std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t >{}.to_bytes(wstr);
}
