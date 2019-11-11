DB=vrsa.db
SQLITE3=sqlite3

rm $DB

$SQLITE3 $DB "\
  PRAGMA foreign_keys=ON; \
  BEGIN TRANSACTION; \
  CREATE TABLE session( \
    id INTEGER PRIMARY KEY, \
    timestamp DATETIME
  ); \
  CREATE TABLE answer( \
    id INTEGER PRIMARY KEY, \
    session_id INTEGER, \
    timestamp DATETIME, \
    interval INTEGER, \
    first INTEGER, \
    second INTEGER, \
    offset INTEGER, \
    answer INTEGER, \
    repeats INTEGER, \
    FOREIGN KEY(session_id) REFERENCES session(id) \
  ); \
  INSERT INTO session VALUES(1, datetime()); \
  INSERT INTO answer VALUES(1, last_insert_rowid(), datetime(), 3, 1, 4, 0, 3, 1); \
  COMMIT; \
"

$SQLITE3 $DB .dump
