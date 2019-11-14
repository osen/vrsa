#include "Database.h"

#include <unistd.h>

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

  ss << "CREATE TABLE IF NOT EXISTS session(";
  ss << "id INTEGER PRIMARY KEY,";
  ss << "host TEXT,";
  ss << "timestamp DATETIME";
  ss << ");";
  ss << std::endl;

  ss << "CREATE TABLE IF NOT EXISTS question(";
  ss << "id INTEGER PRIMARY KEY,";
  ss << "session_id INTEGER,";
  ss << "session_host TEXT,";
  ss << "interval INTEGER,";
  ss << "first_key INTEGER,";
  ss << "second_key INTEGER,";
  ss << "offset INTEGER,";
  ss << "answer INTEGER,";
  ss << "repeats INTEGER";
  ss << ");";
  ss << std::endl;

  execute(ss.str());
}

int Database::queryInt(const std::string& sql)
{
  sqlite3_stmt* res = NULL;
  int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, NULL);

  if(rc != SQLITE_OK)
  {
    throw Exception("SQL query failed");
  }

  while(sqlite3_step(res) != SQLITE_ROW)
  {
    throw Exception("Failed to retrieve row");
  }

  int rtn = sqlite3_column_int(res, 0);
  sqlite3_finalize(res); res = NULL;

  return rtn;
}

void Database::execute(const std::string& sql)
{
  std::cout << sql << std::endl;

  char* errMsg = NULL;
  int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);

  if(rc != SQLITE_OK)
  {
    std::string err = errMsg;
    sqlite3_free(errMsg);

    throw Exception("Failed to execute SQL [" + err + "]");
  }
}

Database::~Database()
{
  sqlite3_close(db);
}

std::string Database::getHostname()
{
  std::vector<char> hn;
  hn.resize(64, 0);

  if(gethostname(&hn.at(0), 63) == -1)
  {
    throw Exception("Failed to obtain hostname");
  }

  return &hn.at(0);
}
