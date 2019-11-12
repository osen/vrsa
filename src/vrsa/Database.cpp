#include "Database.h"

#include <sstream>

void Database::onInitialize()
{
  getEntity()->addTag("database");
  getEntity()->setImmutable(true);

  sqlite3* db = NULL;
  int rc = sqlite3_open("vrsa.db", &db);

  if(rc != SQLITE_OK)
  {
    throw Exception("Failed to open database");
  }

  this->db = db;

  prepare();
}

void Database::prepare()
{
  std::stringstream ss;

  ss << "CREATE TABLE IF NOT EXISTS session(id INTEGER PRIMARY KEY, timestamp DATETIME);";
  ss << std::endl;

  ss << "CREATE TABLE IF NOT EXISTS question(";
  ss << "id INTEGER PRIMARY KEY,";
  ss << "session_id INTEGER";
  ss << ");";
  ss << std::endl;

  execute(ss.str());
}

void Database::execute(const std::string& sql)
{
  std::cout << sql << std::endl;

  sqlite3_stmt* res = NULL;
  int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, NULL);

  if(rc != SQLITE_OK)
  {
    throw Exception("SQL query failed");
  }

  while(sqlite3_step(res) == SQLITE_ROW)
  {
    std::cout << "Row" << std::endl;
  }

  sqlite3_finalize(res); res = NULL;
}

Database::~Database()
{
  sqlite3_close(db);
}
