CREATE TABLE users (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    username TEXT NOT NULL,
    hash TEXT NOT NULL,
    cash NUMERIC NOT NULL DEFAULT 10000.00
);

CREATE TABLE sqlite_sequence(name, seq);

CREATE UNIQUE INDEX username ON users (username);

CREATE TABLE transactions(
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    buyer INTEGER NOT NULL,
    stock TEXT NOT NULL,
    price NUMERIC NOT NULL,
    amount INTEGER NOT NULL,
    total NUMERIC NOT NULL,
    action TEXT NOT NULL,
    timestamp TEXT NOT NULL
);

CREATE UNIQUE INDEX id ON transactions (id);

CREATE TABLE portfolio (
    userid INTEGER NOT NULL,
    stock TEXT NOT NULL,
    amount INTEGER NOT NULL
);