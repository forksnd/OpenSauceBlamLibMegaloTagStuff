/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Memory/MemoryInterface.hpp"

#include "Game/EngineFunctions.hpp"

namespace Yelo
{
	namespace Main
	{
		bool IsYeloEnabled();
		HMODULE& YeloModuleHandle();
	};

	namespace Memory
	{
		void* s_data_array::IteratorNext(void* iter)
		{
			return Engine::Memory::DataIteratorNext(iter);
		}

		bool IsYeloEnabled()
		{
			return Main::IsYeloEnabled();
		}
		HMODULE YeloModuleHandle()
		{
			return Main::YeloModuleHandle();
		}

#define CRC_TABLE_SIZE 256
#define CRC32_POLYNOMIAL 0xEDB88320L
		static uint32 crc_table[CRC_TABLE_SIZE];
		static bool crc_table_initialized = false;

		static void BuildCrcTable()
		{
			for(int16 index = 0; index < CRC_TABLE_SIZE; ++index)
			{
				uint32 crc = index;
				for(int16 j = 0; j < 8; j++)
				{
					if(crc & 1) crc = (crc>>1) ^ CRC32_POLYNOMIAL;
					else crc >>= 1;
				}
				crc_table[index] = crc;
			}
		}

		uint32 CRC(uint32& crc_reference, const void* buffer, int32 size)
		{
			if( !crc_table_initialized ) BuildCrcTable();

			const byte* p;
			uint32 a;
			uint32 b;

			p = CAST_PTR(const byte*, buffer);
			while (size--) 
			{
				a = (crc_reference >> 8) & 0x00FFFFFFL;
				b = crc_table[( (int32) crc_reference ^ *p++) & 0xFF];
				crc_reference = a^b;
			}

			return crc_reference;
		}
	};
};