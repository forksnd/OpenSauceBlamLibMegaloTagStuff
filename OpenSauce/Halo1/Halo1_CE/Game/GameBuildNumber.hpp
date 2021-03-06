/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			// MJ.MN.BD.REV#
			k_game_build_string_length = 15,

			k_game_build_string_major_offset = 0,
			k_game_build_string_major_length = 2,

			k_game_build_string_minor_offset = 3,
			k_game_build_string_minor_length = 2,

			k_game_build_string_build_offset = 6,
			k_game_build_string_build_length = 2,

			k_game_build_string_revision_offset = 9,
			k_game_build_string_revision_length = 4,
		};

		enum game_build_number_index
		{
			_game_build_number_index_100,
			_game_build_number_index_107,
			_game_build_number_index_108,
			_game_build_number_index_109,

			k_max_game_build_number_index,
		};
	};

	typedef char game_build_string_t[Enums::k_game_build_string_length+1];

	namespace BuildNumber
	{
		char*		GameBuildString();			// [k_game_build_string_length+1]
		char*		GamespyGameBuildString();


		void Initialize();
		void Dispose();
		void InitializeForNewMap();

		void InitializeForYeloGameState(bool enabled);


		// Returns true if build_number matches one of the numbers in g_game_build_numbers
		bool StringIsValid(cstring build_number);

		// Parses a major.minor build version string, eg "1.07", to the full build number
		// Returns NULL if given an unidentified version
		cstring VersionToBuildNumberString(cstring maj_minor_str);

		// If [and_game_build] is true, it will also change the GameState::GameBuildStrings
		bool ChangeAdvertisedVersion(cstring version_str, bool and_game_build);
		void ChangeAdvertisedVersionId(long_enum version_id, bool and_game_build);

		// Returns the games current advertised version
		long_enum GetAdvertisedVersion();
	};
};