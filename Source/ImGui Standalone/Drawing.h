#ifndef DRAWING_H
#define DRAWING_H

#include <Windows.h>

#include "ImGui/imgui.h"

class Drawing
{
private:
	static LPCSTR lpWindowName;
	static ImVec2 vWindowSize;
	static ImGuiWindowFlags WindowFlags;
	static bool bDraw;

public:
	static ImFont* m_font;
	static void Active();
	static bool isActive();
	static void Draw();
	static void set_style();
	static void load_font(float size_pixels = 16.0f);
};

#endif
