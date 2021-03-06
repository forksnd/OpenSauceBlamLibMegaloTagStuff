/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>

#include "Rasterizer/PostProcessing/PostProcessing.hpp"
#include "Rasterizer/PostProcessing/ShaderSources/c_shader_data_base.hpp"
#include "Rasterizer/PostProcessing/c_quad_manager.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing
	{
		class c_shader_postprocess
			abstract
		{
			/////////////////////////////////////////////////
			// members
		protected:
			struct
			{
				TagGroups::s_shader_postprocess_definition* definition;
				c_shader_data_base* source_data;
			}m_members;

			/////////////////////////////////////////////////
			// member accessors
		public:
			virtual void	SetShaderDefinition(TagGroups::s_shader_postprocess_definition* definition);
			template<class T>
			T*				GetShaderDefinition()
			{ return CAST_PTR(T*, m_members.definition); }
			void			SetSourceData(c_shader_data_base* source);
			LPD3DXEFFECT	GetEffect();
			bool			IsValid();

			/////////////////////////////////////////////////
			// initializers
		public:
			virtual void	Ctor()
			{
				m_members.definition = NULL;
				m_members.source_data = NULL;
			}
			virtual void	Dtor()
			{
				m_members.definition = NULL;
				m_members.source_data = NULL;
			}

			/////////////////////////////////////////////////
			// shader setup
		public:
			virtual void	SetupShader() {}
			HRESULT			LoadShader(IDirect3DDevice9* render_device);
			void			UnloadShader();
			void			OnDeviceLost();
			HRESULT			OnDeviceReset();
			void			Validate();
		protected:
			virtual void	GetHandles();
			virtual void	ClearHandles();
			virtual bool	ValidateImpl();
		private:
			HRESULT			LoadShaderImpl(IDirect3DDevice9* render_device);
			void			UnloadShaderImpl();
			void			OnDeviceLostImpl();
			HRESULT			OnDeviceResetImpl();
			bool			ValidateShaderVersions();
			TagGroups::s_technique_definition*
							GetActiveTechnique();
			D3DXHANDLE		GetTechniqueHandle();

			/////////////////////////////////////////////////
			// shader application
		public:
			virtual void	SetVariables();
			virtual void	UpdateVariables();

			HRESULT			Render(IDirect3DDevice9* render_device, c_quad_instance* render_quad);
		};
	};};
};
#endif