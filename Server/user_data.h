#pragma once

#include <sqlite3.h>

namespace Server
{
	class user_data
	{
	public:
		
	private:
		sqlite3* db_user_data;
		sqlite3_stmt* stmt_user_data;

	};
}
