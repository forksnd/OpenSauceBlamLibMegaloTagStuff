/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

static bool CacheFileReadHeaderHackImpl(cstring relative_map_name, s_cache_header& out_header, const void* return_address)
{
	bool exception_on_fail = true;
	bool read_for_map_list_add_map = false; // true if this read call is for map_list_add_map

	// HACK: we assume the first entry is the precache_map call site
	const void* k_cache_files_precache_map_ra = CAST_PTR(byte*,K_CACHE_FILE_READ_HEADER_CALLS[0])+sizeof(Memory::Opcode::s_call);
	if(return_address == k_cache_files_precache_map_ra)
	{
		const uint32* stack_mem = (&out_header.footer_signature)+1; // skip the header data on the stack
		stack_mem += 1; // skip the return address on the stack
		exception_on_fail = *stack_mem != FALSE; // get arg_0 of cache_files_precache_map, which determines if the game's stock code exceptions on error (we'll thus inherit this trait)
	}

	// HACK: we assume the second entry is the map_list_add_map call site
	const void* k_map_list_add_map_ra = CAST_PTR(byte*,K_CACHE_FILE_READ_HEADER_CALLS[1])+sizeof(Memory::Opcode::s_call);
	read_for_map_list_add_map = return_address == k_map_list_add_map_ra;

	bool yelo_is_ok;
	return ReadHeader(relative_map_name, out_header, yelo_is_ok, exception_on_fail, read_for_map_list_add_map);
}

static API_FUNC_NAKED bool __cdecl CacheFileReadHeaderHack()
{
	__asm {
		// eax (client), ecx (dedi) = relative_map_name
		// esi = out_header

		push	edi

		mov		edi, [esp+4]	// essentially, edi = _ReturnAddress()
		push	edi
		push	esi
		push	PLATFORM_VALUE(eax, ecx)
		call	CacheFileReadHeaderHackImpl

		pop		edi
		retn
	}
}

class c_cache_format_path_hacks
{
	enum e_phase { // default file search phase order
		_phase_map ,	// by default, .map's are searched for first
		_phase_yelo ,	// by default, .yelo's come second
	};

	// Get the format override based on the current file search phase
	static cstring GetFormat(cstring format, _enum phase)
	{
		static cstring k_yelo_map_format = "%s%s%s.yelo";
		static cstring k_yelo_map_format_subdir = "%s\\%s%s.yelo";

		if((!g_yelo_settings.check_for_yelo_files_first && phase == _phase_yelo) ||
			(g_yelo_settings.check_for_yelo_files_first && phase == _phase_map)) // we want to use .yelo first, so use it in the first phase
			format = format[2] == '\\' ? k_yelo_map_format_subdir : k_yelo_map_format;

		return format;
	}

	// returns [access]
	static int PathHackImpl(string256 buffer, cstring format,
		cstring root_directory, cstring maps_folder, cstring map_name,
		__out int& sprintf_result, __out int& access, _enum phase)
	{
		sprintf_result = sprintf_s(buffer, sizeof(string256), GetFormat(format, phase), root_directory, maps_folder, map_name);
		access = _access_s(buffer, 0);

		return access;
	}
public:
	static int __cdecl PathHack(string256 buffer, cstring format,
		cstring root_directory, cstring maps_folder, cstring map_name)
	{
		int access, result;

		PathHackImpl(buffer, format, root_directory, maps_folder, map_name, 
			result, access, _phase_map);

		if(ENOENT == access)
		{
			PathHackImpl(buffer, format, root_directory, maps_folder, map_name,
				result, access, _phase_yelo);
		}

		if(ENOENT == access)
		{
			YELO_DEBUG_FORMAT("CacheFormatPathHack is about to fail on [%s] %d %d", buffer, access, errno);
		}

		return result;
	}
private:
	// returns [access]
	static int PathHackNImpl(char* buffer, cstring format,
		cstring root_directory, cstring maps_folder, cstring map_name, 
		__out int& sprintf_result, __out int& access, const size_t buffer_size, _enum phase)
	{
		sprintf_result = _snprintf_s(buffer, buffer_size, _TRUNCATE, GetFormat(format, phase), root_directory, maps_folder, map_name);
		access = _access_s(buffer, 0);

		return access;
	}
	static int __cdecl PathHackN(char* buffer, size_t max_count, cstring format,
		cstring root_directory, cstring maps_folder, cstring map_name)
	{
		const size_t buffer_size = max_count+1;
		int access, result;

		PathHackNImpl(buffer, format, root_directory, maps_folder, map_name, 
			result, access, buffer_size, _phase_map);

		if(ENOENT == access)
		{
			PathHackNImpl(buffer, format, root_directory, maps_folder, map_name, 
				result, access, buffer_size, _phase_yelo);
		}

		if(ENOENT == access)
		{
			YELO_DEBUG_FORMAT("CacheFormatPathHackN is about to fail on [%s] %d %d", buffer, access, errno);
		}

		return result;
	}

public:
	static void Initialize()
	{
		for(int32 x = 0; x < NUMBEROF(K_CACHE_PATH_SPRINTF_CALLS); x++)
			Memory::WriteRelativeCall(PathHack, K_CACHE_PATH_SPRINTF_CALLS[x]);
		for(int32 x = 0; x < NUMBEROF(K_CACHE_PATH_SNPRINTF_CALLS)-1; x++)
			Memory::WriteRelativeCall(PathHackN, K_CACHE_PATH_SNPRINTF_CALLS[x]);

		// Redirect all game calls to cache_file_read_header to our implementation which supports .yelo validation
		for(int32 x = 0; x < NUMBEROF(K_CACHE_FILE_READ_HEADER_CALLS); x++)
			Memory::WriteRelativeCall(CacheFileReadHeaderHack, K_CACHE_FILE_READ_HEADER_CALLS[x]);
	}
};


static void MapListInitializeYelo()
{
	WIN32_FIND_DATAA fd;
	HANDLE h = FindFirstFileA("maps\\*.yelo", &fd);
	if(h != INVALID_HANDLE_VALUE)
	{
		do Engine::Cache::MapListAddMap(fd.cFileName, ".yelo");
		while(FindNextFileA(h, &fd));

		FindClose(h);
	}
}