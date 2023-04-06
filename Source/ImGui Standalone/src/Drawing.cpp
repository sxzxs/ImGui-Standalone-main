#include "Drawing.h"
#include <string>
#include "simhei.h"
//#include "icons.h"
//#include "custom.h"
#include "icon_font_awesome4.h"

LPCSTR Drawing::lpWindowName = "ImGui Standalone";
ImVec2 Drawing::vWindowSize = { 800, 500 };
ImVec2 Drawing::mini_window_size = { 150, 30 };
ImGuiWindowFlags Drawing::WindowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
										ImGuiWindowFlags_NoResize;
bool Drawing::bDraw = true;

bool Drawing::is_contraction_of_the_window = false;

ImFont *Drawing::m_font = nullptr;

ImTextureID Drawing::click_image_id{};

GIF_STRUCT Drawing::run_gif{};

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
		static bool is_first_run = true;
		if (is_first_run)
		{
			ImGui::SetNextWindowSize(vWindowSize);
			ImGui::SetNextWindowPos({ 0, 0 });
			is_first_run = false;
		}
		ImGui::SetNextWindowBgAlpha(1.0f);
		ImGui::Begin(lpWindowName, &bDraw, WindowFlags);
		{
			auto windows_pos = ImGui::GetWindowPos();
			auto windows_size = ImGui::GetWindowSize();
			auto win_draw_list = ImGui::GetWindowDrawList();
			ImGui::SetCursorPosY(0);

			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
			if(ImGui::Button(ICON_FA_THEMEISLE))
			{
				if (!is_contraction_of_the_window)
				{
					ImGui::SetWindowSize(mini_window_size);
					WindowFlags = WindowFlags | ImGuiWindowFlags_NoScrollbar;
				}
				else
				{
					ImGui::SetWindowSize(vWindowSize);
					WindowFlags = WindowFlags & ~ImGuiWindowFlags_NoScrollbar;
				}
				is_contraction_of_the_window = !is_contraction_of_the_window;
			}
			ImGui::PopStyleVar();

			ImGui::SetCursorPos({windows_size.x - 32, 0});
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
			if (ImGui::Button(ICON_FA_WINDOW_CLOSE))
			{
				bDraw = !bDraw;
			}
			ImGui::PopStyleVar();
			ImGui::Separator();

			ImGui::Columns(2);
			static int with_colum1 = 90;
			static FUNC_MENU_ENUM current_menu_index = MENU_MAIN;
			ImGui::SetColumnOffset(1, with_colum1);
			{
				if (ImGui::Button(ICON_FA_LIST, ImVec2(60, 60)))
				{
					current_menu_index = MENU_MAIN;
				}
				if (current_menu_index == MENU_MAIN)
				{
					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 0.6f, 1.0f));
					ImGui::Text(ICON_FA_HAND_O_RIGHT);
					ImGui::PopStyleColor();
				}

				if (ImGui::Button(ICON_FA_SEARCH, ImVec2(60, 60)))
				{
					current_menu_index = MENU_SEARCH;
				}
				if (current_menu_index == MENU_SEARCH)
				{
					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 0.6f, 1.0f));
					ImGui::Text(ICON_FA_HAND_O_RIGHT);
					ImGui::PopStyleColor();
				}

				if (ImGui::Button(ICON_FA_HOME, ImVec2(60, 60)))
				{
					current_menu_index = MENU_HELP;
				}
				if (current_menu_index == MENU_HELP)
				{
					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 0.6f, 1.0f));
					ImGui::Text(ICON_FA_HAND_O_RIGHT);
					ImGui::PopStyleColor();
				}


				if (!is_contraction_of_the_window)
				{
					//ImGui::Text("%f %f", windows_size.x, windows_size.y);
					//ImGui::Text("%f %f", windows_pos.x, windows_pos.y);
					//ImGui::SetCursorPos({0, windows_size.y - 70});
					ImGui::SetCursorPosY(windows_size.y - 70);
					//ImGui::ImageButton(run_gif.srv_array[run_gif.current_index], { 50, 50 });
					win_draw_list->AddImage(run_gif.srv_array[run_gif.current_index], { windows_pos.x, windows_pos.y + windows_size.y - 70 }, {windows_pos.x + 60, windows_pos.y + windows_size.y});
				}
			}
			ImGui::NextColumn();
			{
				if (ImGui::BeginTabBar("MyTabBar"))
				{
					if (ImGui::BeginTabItem("Tab1"))
					{
						ImGui::Text("This is my first tab!");
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Tab2"))
					{
						ImGui::Text("This is my second tab!");
						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
				}
			}
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
	style.WindowMinSize = mini_window_size;

	style.WindowPadding = { 0, 0 };
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
	ImGuiIO& io = ImGui::GetIO();
	auto fontAtlas = ImGui::GetIO().Fonts;
	//fontAtlas->ClearFonts();
	auto glyphRange = fontAtlas->GetGlyphRangesVietnamese();
	glyphRange = fontAtlas->GetGlyphRangesChineseFull();
	ImVector<ImWchar> myRange;
	static std::string ranges_string(u8"卡牌切牌 关闭 取色1234567890 .*!@#$%^&*()_-=+ "
		"abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ "
		"显示隐藏终端 坐标, y等待时间 主界面 其他 保存配置 颜色设置 用户名 密码 登录");
	ImFontGlyphRangesBuilder myGlyph;
	myGlyph.AddText(ranges_string.c_str());
	myGlyph.BuildRanges(&myRange);

	ImFont* font = nullptr;
	// 首先加载一个字体，后面的用merge模式， merge进默认字体
	// 或者不调用，用户加载的第一个字体载入
    //io.Fonts->AddFontDefault();

	ImFontConfig config_words{};
	config_words.MergeMode = false;
	config_words.OversampleH = 2;
	config_words.OversampleV = 1;
	config_words.FontDataOwnedByAtlas = false; //从内存加载的font，需要自己管理内存

	//ImFont* font = fontAtlas->AddFontFromMemoryTTF(simhei_font(), simhei_size, size_pixels, &config_words, myRange.Data);
	font = fontAtlas->AddFontFromMemoryTTF(simhei_font(), simhei_size, size_pixels, &config_words, glyphRange);	
	m_font = font;

    //static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig icons_config;

    ImFontConfig CustomFont;
    CustomFont.FontDataOwnedByAtlas = false;

    icons_config.MergeMode = true;
    icons_config.PixelSnapH = false;
    icons_config.OversampleH = 2;
    icons_config.OversampleV = 1;

    //io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom), sizeof(Custom), 15, &CustomFont);
    //io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, size_pixels, &icons_config, icons_ranges);
	io.Fonts->AddFontFromFileTTF("fonts/fontawesome-webfont.ttf", 13.0f, &icons_config, icons_ranges);
	io.Fonts->Build();

	int width, height;
	unsigned char* pixels = NULL;
	fontAtlas->GetTexDataAsAlpha8(&pixels, &width, &height);
	IM_ASSERT(font != NULL);
	return;
}
