/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Cache/Cache.hpp"

#include "Game/EngineFunctions.hpp"
#include "Memory/Runtime.hpp"

static Yelo::Memory::Opcode::CallRet UNWIND_GAME_STATE_INITIALIZE_FOR_NEW_MAP;

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_CACHE
#define __EL_INCLUDE_FILE_ID	__EL_CACHE_CACHE
#include "Memory/_EngineLayout.inl"

	namespace Cache
	{
		void Initialize()
		{
			YELO_MEM_WRITE(WriteCallRet, 
				&UNWIND_GAME_STATE_INITIALIZE_FOR_NEW_MAP, 
				GET_FUNC_VPTR(GAME_STATE_INITIALIZE_FOR_NEW_MAP_MOD),
				&Cache::InitializeForNewMap);
		}

		void InitializeForNewMap()
		{
			uint32 count = GameState::_TagIndex()->Count;
			s_tag_instance* tag_definition = GameState::_TagInstances();
			byte* tag_data;
			for(uint32 x = 0; x < count; x++, tag_definition++)
			{
				tag_data = CAST_PTR(byte*, tag_definition->Definition);
				switch(tag_definition->GroupTag)
				{
				case 'weap':
					// dual wield everything
					*CAST_PTR(long_flags*, tag_data+0x12C) |= FLAG(22);
					break;

				case 'vehi':
					break;

				case 'cont':
					break;

				case 'proj':
					break;

				case 'bitm':
					break;

				case 'bipd':
					break;

				case 'ligh':
					break;
				}
			}
		}

		void Dispose()
		{
			YELO_MEM_WRITE(WriteRetn, 
				UNWIND_GAME_STATE_INITIALIZE_FOR_NEW_MAP, 
				GET_FUNC_VPTR(GAME_STATE_INITIALIZE_FOR_NEW_MAP_MOD));
		}
	};

	namespace GameState
	{
		Cache::s_header* _CacheHeader()			DPTR_IMP_GET(_CacheHeader);
		Cache::s_tag_index* _TagIndex()
		{
			return CAST_PTR(Cache::s_tag_index*,		0x80061000);
		}
		Cache::s_tag_instance* _TagInstances()	DPTR_IMP_GET(_TagInstances);
	};
};