CREATE TABLE users(
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    username TEXT NOT NULL,
    hash TEXT NOT NULL,
    email TEXT NOT NULL,
    activated TEXT
);

CREATE TABLE sqlite_sequence(name, seq);

CREATE TABLE post(
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    author INTEGER NOT NULL,
    timestamp INTEGER NOT NULL,
    text TEXT
);