#include "Drawing.h"
#include <string>
#include "simhei.h"

LPCSTR Drawing::lpWindowName = "ImGui Standalone";
ImVec2 Drawing::vWindowSize = { 500, 500 };
ImGuiWindowFlags Drawing::WindowFlags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;
bool Drawing::bDraw = true;
ImFont *Drawing::m_font = nullptr;

void Drawing::Active()
{
	bDraw = true;
}

bool Drawing::isActive()
{
	return bDraw == true;
}

void Drawing::Draw()
{
	if (isActive())
	{
		ImGui::SetNextWindowSize(vWindowSize);
		ImGui::SetNextWindowBgAlpha(1.0f);
		ImGui::Begin(lpWindowName, &bDraw, WindowFlags);
		{
			ImGui::Text("Test");
		}
		ImGui::End();
	}

	#ifdef _WINDLL
	if (GetAsyncKeyState(VK_INSERT) & 1)
		bDraw = !bDraw;
	#endif
}

void Drawing::set_style()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowBorderSize = 0;
	style.WindowTitleAlign = ImVec2(0.5, 0.5);
	style.WindowMinSize = ImVec2(400, 230);

	style.FramePadding = ImVec2(8, 6);

	style.TabBorderSize = 1.0f;
	style.FrameBorderSize = 1.0f;

	style.WindowRounding = 12;
	style.ChildRounding = 6;
	style.FrameRounding = 6;
	style.PopupRounding = 6;
	style.ScrollbarRounding = 6;
	style.ScrollbarRounding = 6;
	style.GrabRounding = 6;
	style.LogSliderDeadzone = 6;
	style.TabRounding = 6;

	style.Colors[ImGuiCol_TitleBg] = ImColor(14, 4, 1, 255);
	style.Colors[ImGuiCol_TitleBgActive] = ImColor(17, 5, 1, 255);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(0, 0, 130, 0);

	style.Colors[ImGuiCol_Tab] = ImColor(0, 0, 0, 255);
	style.Colors[ImGuiCol_TabHovered] = ImColor(40, 213, 204, 255);
	style.Colors[ImGuiCol_TabActive] = ImColor(19, 123, 49, 255);

	style.Colors[ImGuiCol_Button] = ImColor(31, 30, 31, 255);
	style.Colors[ImGuiCol_ButtonActive] = ImColor(41, 40, 41, 255);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(66, 150, 250, 255);

	style.Colors[ImGuiCol_Separator] = ImColor(70, 70, 70, 255);
	style.Colors[ImGuiCol_SeparatorActive] = ImColor(76, 76, 76, 255);
	style.Colors[ImGuiCol_SeparatorHovered] = ImColor(76, 76, 76, 255);

	style.Colors[ImGuiCol_FrameBg] = ImColor(37, 36, 37, 255);
	style.Colors[ImGuiCol_FrameBgActive] = ImColor(37, 36, 37, 255);
	style.Colors[ImGuiCol_FrameBgHovered] = ImColor(37, 36, 37, 255);

	style.Colors[ImGuiCol_Header] = ImColor(0, 0, 0, 0);
	style.Colors[ImGuiCol_HeaderActive] = ImColor(0, 0, 0, 0);
	style.Colors[ImGuiCol_HeaderHovered] = ImColor(46, 46, 46, 255);
}

void Drawing::load_font(float size_pixels /*= 16.0f*/)
{
	auto fontAtlas = ImGui::GetIO().Fonts;
	fontAtlas->ClearFonts();
	auto glyphRange = fontAtlas->GetGlyphRangesVietnamese();
	glyphRange = fontAtlas->GetGlyphRangesChineseFull();
	ImVector<ImWchar> myRange;
	static std::string ranges_string(u8"卡牌切牌 关闭 取色1234567890 .*!@#$%^&*()_-=+ "
		"abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ "
		"显示隐藏终端 坐标, y等待时间 主界面 其他 保存配置 颜色设置 用户名 密码 登录");
	ImFontGlyphRangesBuilder myGlyph;
	myGlyph.AddText(ranges_string.c_str());
	myGlyph.BuildRanges(&myRange);

	ImFontConfig config_words{};
	config_words.OversampleH = 1;
	config_words.OversampleV = 1;
	config_words.FontDataOwnedByAtlas = false;

	//ImFont* font = fontAtlas->AddFontFromMemoryTTF(simhei_font(), simhei_size, size_pixels, &config_words, myRange.Data);
	ImFont* font = fontAtlas->AddFontFromMemoryTTF(simhei_font(), simhei_size, size_pixels, &config_words, glyphRange);	
	m_font = font;
	int width, height;
	unsigned char* pixels = NULL;
	fontAtlas->GetTexDataAsAlpha8(&pixels, &width, &height);
	IM_ASSERT(font != NULL);
	return;
}
