#ifndef DRAWING_H
#define DRAWING_H

#include <Windows.h>

#include "ImGui/imgui.h"
#include "utility.h"

typedef enum FUNC_MENU
{
	MENU_MAIN = 0,
	MENU_HELP,
	MENU_SEARCH
}FUNC_MENU_ENUM;

typedef struct IMAGE_SISE
{
	float w;
	float h;
}IMAGE_SIZE_STRUCT;

class Drawing
{
private:
	static LPCSTR lpWindowName;
	static ImVec2 vWindowSize;
	static ImVec2 mini_window_size;
	static ImGuiWindowFlags WindowFlags;
	static bool bDraw;
	static bool is_contraction_of_the_window;
	static unsigned int current_choose_tab[2];

public:
	static ImFont* m_font;
	static ImTextureID click_image_id;
	static ImTextureID back_image_id;
	static IMAGE_SIZE_STRUCT back_image_size;
    static GIF_STRUCT run_gif;
	static void Active();
	static bool isActive();
	static void Draw();
	static void set_style();
	static void load_font(float size_pixels = 16.0f);
};

#endif
