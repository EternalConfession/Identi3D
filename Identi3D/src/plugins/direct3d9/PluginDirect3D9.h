//
// File: PluginDirect3D9.h
// =======================
// Direct3D 9 renderer for Identi3D.
//

#ifndef IDENTI3D_SRC_PLUGINS_DIRECT3D9_PLUGINDIRECT3D9_H
#define IDENTI3D_SRC_PLUGINS_DIRECT3D9_PLUGINDIRECT3D9_H

#include <src/identi3d/General.h>
#include <src/plugins/direct3d9/Setting.h>
#include <src/renderer/RenderDevice.h>
#include <src/utils/DebugFrame.h>

#if defined (_DEBUG)
# if !defined (D3D_DEBUG_INFO)
#  define D3D_DEBUG_INFO	// Enable Direct3D9 debug output.
# endif // !defined (D3D_DEBUG_INFO)
#endif // defined (_DEBUG)

#include <d3d9.h>

namespace Identi3D
{

	class PluginDirect3D9 : public RenderDevice, public DebugFrame
	{
	private:
		RenderWindow			*_render_target;
		HINSTANCE				_plugin_handle;

		LPDIRECT3D9				_direct3d;
		LPDIRECT3DDEVICE9		_direct_device;
		D3DPRESENT_PARAMETERS	_present_parameters;
		D3DCOLOR				_clear_color;

		Direct3D9SettingManager _settings;
		
		bool _is_running;
		bool _is_scene_running;

	private:
		bool run(D3DFORMAT format);
		bool checkPrerequisite(D3DFORMAT format);

	public:

		PluginDirect3D9(HMODULE plugin, DebugManager *debugger = NULL);
		~PluginDirect3D9(void);
		
		/*
		 * Load configuration.
		 */
		bool reloadConfig(OptionTree *option);

		/*
		 * Initialize the render device.
		 */
		bool init(RenderWindow &target);

		/*
		 * Release the render device.
		 */
		void release(void);

		/*
		 * Is device online.
		 */
		bool isRunning(void);

		/*
		 * Start the rendering procedure.
		 */
		bool startRendering(bool clear_pixel, bool clear_depth, bool clear_stencil);

		/*
		 * End the rendering procedure.
		 */
		void endRendering(void);

		/*
		 * Clear screen buffer.
		 */
		bool clear(bool clear_pixel, bool clear_depth, bool clear_stencil);

		/*
		 * Set clear color.
		 */
		void setClearColor(float red, float green, float blue);

		/*
		 * Get screen width.
		 */
		const UINT getWidth(void) const { return _settings._screen_width; }

		/*
		 * Get screen height.
		 */
		const UINT getHeight(void) const { return _settings._screen_height; }

	};

};

#endif // IDENTI3D_SRC_PLUGINS_DIRECT3D9_PLUGINDIRECT3D9_H
