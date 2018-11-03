#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include "FRAME_EXPORTS.h"

namespace ElayGraphics
{
#ifdef _DEBUG
	#define _WARNING(condition, response) if(condition) {std::clog << response << std::endl;}
#else
	#define _WARNING(condition, response) ((void)0)
#endif

#define MAX_VALUE 3.402823466e+38F
#define MIN_VALUE 1.175494351e-38F
	const float PI = 3.14159265359f;

	namespace WINDOW_KEYWORD
	{
		extern int WINDOW_WIDTH;
		extern int WINDOW_HEIGHT;
		extern int VIEWPORT_WIDTH;
		extern int VIEWPORT_HEIGHT;
		extern int VIEWPORT_LEFTBOTTOM_X;
		extern int VIEWPORT_LEFTBOTTOM_Y;
		extern int NUM_SAMPLES;
		extern bool CURSOR_DISABLE;
		extern std::string WINDOW_TITLE;
	}

	namespace COMPONENT_CONFIG
	{
		extern bool IS_ENABLE_GUI;
	}

	struct FRAME_DLLEXPORTS STexture
	{
		enum class ETextureType
		{
			Texture2D,
			Texture3D,
			Texture2DArray,
			TextureCubeMap
		};
		enum class ETextureAttachmentType
		{
			ColorTexture,
			DepthTexture,
			StencilTexture,
			DepthAndStencilTexture
		};
		GLint					InternalFormat;
		GLenum					ExternalFormat;
		GLenum					DataType;
		GLsizei					Width;
		GLsizei					Height;
		GLsizei					Depth;
		std::vector<GLvoid*>	pDataSet;
		GLint					Type4WrapS;
		GLint					Type4WrapT;
		GLint					Type4WrapR;
		GLint					Type4MinFilter;
		GLint					Type4MagFilter;
		GLboolean				isMipmap;
		GLint					TextureID;
		std::string				TextureName;
		ETextureType			TextureType;
		ETextureAttachmentType	TextureAttachmentType;
		std::vector<float>		BorderColor;

		STexture() : TextureID(-1), InternalFormat(GL_RGBA), ExternalFormat(GL_RGBA), DataType(GL_UNSIGNED_BYTE), Width(WINDOW_KEYWORD::WINDOW_WIDTH),
			Height(WINDOW_KEYWORD::WINDOW_HEIGHT), Depth(1), Type4WrapS(GL_REPEAT), Type4WrapT(GL_REPEAT), Type4WrapR(GL_REPEAT), Type4MinFilter(GL_LINEAR),
			Type4MagFilter(GL_LINEAR), isMipmap(GL_TRUE), TextureType(ETextureType::Texture2D), TextureAttachmentType(ETextureAttachmentType::ColorTexture), BorderColor{ 1,1,1,1 }
		{
		}
	};

	enum class EDataType
	{
		DataType_I32,      // int
		DataType_U32,      // unsigned int
		DataType_I64,      // long long, __int64
		DataType_U64,      // unsigned long long, unsigned __int64
		DataType_Float,    // float
		DataType_Double,   // double
	};

	enum class EGUIItemColor
	{
		GUIItemColor_Text,
		GUIItemColor_TextDisabled,
		GUIItemColor_WindowBg,              // Background of normal windows
		GUIItemColor_ChildBg,               // Background of child windows
		GUIItemColor_PopupBg,               // Background of popups, menus, tooltips windows
		GUIItemColor_Border,
		GUIItemColor_BorderShadow,
		GUIItemColor_FrameBg,               // Background of checkbox, radio button, plot, slider, text input
		GUIItemColor_FrameBgHovered,
		GUIItemColor_FrameBgActive,
		GUIItemColor_TitleBg,
		GUIItemColor_TitleBgActive,
		GUIItemColor_TitleBgCollapsed,
		GUIItemColor_MenuBarBg,
		GUIItemColor_ScrollbarBg,
		GUIItemColor_ScrollbarGrab,
		GUIItemColor_ScrollbarGrabHovered,
		GUIItemColor_ScrollbarGrabActive,
		GUIItemColor_CheckMark,
		GUIItemColor_SliderGrab,
		GUIItemColor_SliderGrabActive,
		GUIItemColor_Button,
		GUIItemColor_ButtonHovered,
		GUIItemColor_ButtonActive,
		GUIItemColor_Header,
		GUIItemColor_HeaderHovered,
		GUIItemColor_HeaderActive,
		GUIItemColor_Separator,
		GUIItemColor_SeparatorHovered,
		GUIItemColor_SeparatorActive,
		GUIItemColor_ResizeGrip,
		GUIItemColor_ResizeGripHovered,
		GUIItemColor_ResizeGripActive,
		GUIItemColor_PlotLines,
		GUIItemColor_PlotLinesHovered,
		GUIItemColor_PlotHistogram,
		GUIItemColor_PlotHistogramHovered,
		GUIItemColor_TextSelectedBg,
		GUIItemColor_DragDropTarget,
		GUIItemColor_NavHighlight,          // Gamepad/keyboard: current highlighted item
		GUIItemColor_NavWindowingHighlight, // Highlight window when using CTRL+TAB
		GUIItemColor_NavWindowingDimBg,     // Darken/colorize entire screen behind the CTRL+TAB window list, when active
		GUIItemColor_ModalWindowDimBg,      // Darken/colorize entire screen behind a modal window, when one is active
		GUIItemColor_COUNT

		// Obsolete names (will be removed)
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
		, GUIItemColor_ChildWindowBg = GUIItemColor_ChildBg, GUIItemColor_Column = GUIItemColor_Separator, GUIItemColor_ColumnHovered = GUIItemColor_SeparatorHovered, GUIItemColor_ColumnActive = GUIItemColor_SeparatorActive
		, GUIItemColor_ModalWindowDarkening = GUIItemColor_ModalWindowDimBg
		//GUIItemColor_CloseButton, GUIItemColor_CloseButtonActive, GUIItemColor_CloseButtonHovered, // [unused since 1.60+] the close button now uses regular button colors.
		//GUIItemColor_ComboBg,                                                              // [unused since 1.53+] ComboBg has been merged with PopupBg, so a redirect isn't accurate.
#endif
	};
}