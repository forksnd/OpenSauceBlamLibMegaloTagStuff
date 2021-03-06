/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

namespace ActionResults
{
	/********* change to Yelo::Enums::_player_action_result_yelo to support custom action results *********/
	static void* player_handle_action_jmp_table_yelo[Yelo::Enums::_player_action_result];

	// Replaces PlayerHandleAction function's switch table with our own
	static void InitializePlayerHandleActionJmpTable()
	{
		// how much the stock jmp table is offset from the action_result enum
		int32 offset = Enums::_player_action_result_swap_equipment;

		// write the game's default jump case into our jump table
		/********* change to Yelo::Enums::_player_action_result_yelo to support custom action results *********/
		for(int32 x = 0; x < Yelo::Enums::_player_action_result; x++)
			player_handle_action_jmp_table_yelo[x] = (void*)PLAYER_HANDLE_ACTION_JMP_TABLE[1];

		// copy the game's jmp table to our own and account for the offset
		for(int32 x = offset; x <= *K_PLAYER_HANDLE_ACTION_LAST_ENTRY_TYPE + offset; x++)
			player_handle_action_jmp_table_yelo[x] = (void*)PLAYER_HANDLE_ACTION_JMP_TABLE[x - offset];

		static const byte opcode_null[] = { Enums::_x86_opcode_nop, Enums::_x86_opcode_nop, Enums::_x86_opcode_nop };

		// nullify jmp table size adjustment
		Memory::WriteMemory(GET_FUNC_VPTR(PLAYER_HANDLE_ACTION_JMP_TABLE_ADJUST_SIZE), opcode_null, sizeof(opcode_null));
		// set the game's jump table count to our's
		/********* change to Yelo::Enums::_player_action_result_yelo to support custom action results *********/
		*K_PLAYER_HANDLE_ACTION_LAST_ENTRY_TYPE = Yelo::Enums::_player_action_result - 1;
		// set the game's jump table address to our's
		GET_PTR(player_handle_action_jmp_ptr) = player_handle_action_jmp_table_yelo;
	}
	
	// Initialize custom yelo action results
	static void InitializeYeloActionResults()
	{
		// Example:
		//player_handle_action_jmp_table_yelo[Enums::_player_action_result_yelo] = &JMPFunction;
	}

	void Initialize()
	{
		InitializePlayerHandleActionJmpTable();
		InitializeYeloActionResults();
	}
};