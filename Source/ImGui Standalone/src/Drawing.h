#ifndef DRAWING_H
#define DRAWING_H

#include <Windows.h>

#include "ImGui/imgui.h"
#include "utility.h"

class Drawing
{
private:
	static LPCSTR lpWindowName;
	static ImVec2 vWindowSize;
	static ImVec2 mini_window_size;
	static ImGuiWindowFlags WindowFlags;
	static bool bDraw;
	static bool is_contraction_of_the_window;

public:
	static ImFont* m_font;
	static ImTextureID click_image_id;
    static GIF_STRUCT run_gif;
	static void Active();
	static bool isActive();
	static void Draw();
	static void set_style();
	static void load_font(float size_pixels = 16.0f);
};

#endif
