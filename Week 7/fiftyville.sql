-- Keep a log of any SQL queries you execute as you solve the mystery.

-- First, let's check the crimes that happened on 2021/07/28 in Humphrey Street:
.schema crime_scene_reports

SELECT description FROM crime_scene_reports
WHERE year=2021 AND month=7 AND day=28 AND street="Humphrey Street";

-- There are two crimes that took place at this day on Humphrey Street:
-- One was littering with no witnesses, the other was the theft of the cs50 duck at 10:15am
-- The description for the second crime says "Each of the interview transcripts mentions the bakery."

-- Let's have a look at the interview transcripts where the bakery is mentioned.
.schema interviews
-- We can check for the transcripts:
SELECT transcript FROM interviews
WHERE transcript LIKE "%bakery%"
AND year=2021 AND month=7 AND day=28;

-- We now know that the three witnesses are called Ruth, Eugene, and Raymond.
-- These are their statements:
-- Ruth says the thief got into a car in the parking lot of the bakrey around ten minutes after the crime. She points to the security footage.
-- Eugene says he recognizes the thief, but doesn't know his name. However, he remembers the thief withdrew some money at the ATM at Leggett Street the same day.
-- Raymond says the thief talked to someone and they were planning to take the first flight out of town the day after. The other person is supposed to buy the flight tickets.

-- We can now follow three leads. Let's start with Ruth's lead. As she mentioned the parking lot, let's check the bakery_security_logs
.schema bakery_security_logs

-- We can check for the license plate here: they left around 10:25am
SELECT license_plate FROM bakery_security_logs
WHERE year=2021 AND month=7 AND day=28 AND hour=10 AND minute>=15 AND minute<=25 AND activity="exit";

-- We now have 8 licence plates that could belong to our thief.
-- At this moment, this is a dead end. Let's follow Eugene's lead.
-- Recall, Eugene said the Thief withdrew money at the ATM in Leggett Street the same day. Let's check out the ATM database.
.schema atm_transactions;

-- We can check who withdrew money
SELECT account_number FROM atm_transactions
WHERE year=2021 AND month=7 AND day=28
AND atm_location="Leggett Street" AND transaction_type="withdraw";

-- We now know that this day, eight people withdrew money from this ATM
-- We also know their person_ids:
SELECT person_id FROM bank_accounts
    JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
    WHERE atm_transactions.year=2021 AND atm_transactions.month=7 AND atm_transactions.day=28
    AND atm_transactions.atm_location="Leggett Street" AND atm_transactions.transaction_type="withdraw";

-- For now, this is a dead end, too. Let's check the last testimonial.
-- The thief and his/her helper are supposed to leave the city the next day by plane.
-- Right after the crime, the thief called someone for less than a minute. Let's check the phone_calls
.schema phone_calls

-- Let's look for the calls:
SELECT caller, receiver FROM phone_calls WHERE year=2021 AND month=7 AND day=28 AND duration<60;
-- We have 9 results.
-- If we want to get their person_ids:
SELECT id FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year=2021 AND month=7 AND day=28 AND duration<60);
-- We can already cross reference some people
-- These people with the ATM transaction AND with the call
SELECT person_id FROM bank_accounts
    JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
    WHERE atm_transactions.year=2021 AND atm_transactions.month=7 AND atm_transactions.day=28
    AND atm_transactions.atm_location="Leggett Street" AND atm_transactions.transaction_type="withdraw"
    INTERSECT
    SELECT id FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year=2021 AND month=7 AND day=28 AND duration<60);
-- This leaves us with five options
-- We can narrow it down by checking it with the license plate:

SELECT person_id FROM bank_accounts
    JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
    WHERE atm_transactions.year=2021 AND atm_transactions.month=7 AND atm_transactions.day=28
    AND atm_transactions.atm_location="Leggett Street" AND atm_transactions.transaction_type="withdraw"
    INTERSECT
    SELECT id FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year=2021 AND month=7 AND day=28 AND duration<60)
    INTERSECT
    SELECT id FROM people WHERE license_plate IN(
        SELECT license_plate FROM bakery_security_logs
        WHERE year=2021 AND month=7 AND day=28 AND hour=10 AND minute>=15 AND minute<=25 AND activity="exit");

-- This now leaves us with two people, which is not bad.
-- Let's check for the passengers next day, let's first check the flights:
-- It's for the 29th, the day after the robbery
SELECT * FROM flights WHERE year=2021 AND month=7 and day=29;

-- let's check all the passengers for these flights
SELECT passport_number FROM passengers WHERE flight_id IN (SELECT id FROM flights WHERE year=2021 AND month=7 AND day=29);

-- Let's narrow it down and intersect it with the previous query
SELECT person_id FROM bank_accounts
    JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
    WHERE atm_transactions.year=2021 AND atm_transactions.month=7 AND atm_transactions.day=28
    AND atm_transactions.atm_location="Leggett Street" AND atm_transactions.transaction_type="withdraw"
    INTERSECT
    SELECT id FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year=2021 AND month=7 AND day=28 AND duration<60)
    INTERSECT
    SELECT id FROM people WHERE license_plate IN(
        SELECT license_plate FROM bakery_security_logs
        WHERE year=2021 AND month=7 AND day=28 AND hour=10 AND minute>=15 AND minute<=25 AND activity="exit")
    INTERSECT
    SELECT id FROM people WHERE passport_number IN (
        SELECT passport_number FROM passengers WHERE flight_id IN (
            SELECT id FROM flights WHERE year=2021 AND month=7 AND day=29
        )
    );
-- This returns us two people, which is not good enough. :(
-- Let's check which airport we fly FROM:
SELECT abbreviation FROM airports WHERE full_name LIKE "%Fiftyville%";
-- The abbreviation is CSF and the ID is 8.

-- Let's narrow it down even more.
SELECT person_id FROM bank_accounts
    JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
    WHERE atm_transactions.year=2021 AND atm_transactions.month=7 AND atm_transactions.day=28
    AND atm_transactions.atm_location="Leggett Street" AND atm_transactions.transaction_type="withdraw"
    INTERSECT
    SELECT id FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year=2021 AND month=7 AND day=28 AND duration<60)
    INTERSECT
    SELECT id FROM people WHERE license_plate IN(
        SELECT license_plate FROM bakery_security_logs
        WHERE year=2021 AND month=7 AND day=28 AND hour=10 AND minute>=15 AND minute<=25 AND activity="exit")
    INTERSECT
    SELECT id FROM people WHERE passport_number IN (
        SELECT passport_number FROM passengers WHERE flight_id IN (
            SELECT id FROM flights WHERE year=2021 AND month=7 AND day=29 AND origin_airport_id=8
        )
    );
-- This doesn't really help, still two people:
-- 514354;
-- 686048

-- Let's recall the interviews: the suspect wanted to leave the earliest possible, so we only have one flight:
SELECT * FROM flights WHERE year=2021 AND month=7 and day=29;
-- SO it's flight number 36.
-- Let's check if either person is on this particular flight

SELECT person_id FROM bank_accounts
    JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
    WHERE atm_transactions.year=2021 AND atm_transactions.month=7 AND atm_transactions.day=28
    AND atm_transactions.atm_location="Leggett Street" AND atm_transactions.transaction_type="withdraw"
    INTERSECT
    SELECT id FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year=2021 AND month=7 AND day=28 AND duration<60)
    INTERSECT
    SELECT id FROM people WHERE license_plate IN(
        SELECT license_plate FROM bakery_security_logs
        WHERE year=2021 AND month=7 AND day=28 AND hour=10 AND minute>=15 AND minute<=25 AND activity="exit")
    INTERSECT
    SELECT id FROM people WHERE passport_number IN (
        SELECT passport_number FROM passengers WHERE flight_id=36
    );

-- TADA, we only have one person left: ID 686048
SELECT name FROM people WHERE id=686048;
-- The suspect's name is BRUCE!
-- Let's now find which city he was planning to fly to. We know it's flight_id 36
SELECT city FROM airports WHERE ID IN(
SELECT destination_airport_id FROM flights WHERE id=36);
-- We know he wants to escape to New York City

-- Now let's find his accomplice. The easiest way is to check who he called that day
SELECT name FROM people WHERE phone_number IN (
    SELECT receiver FROM phone_calls WHERE caller IN (
        SELECT phone_number FROM people WHERE name="Bruce"
    ) AND year=2021 AND month=7 AND day=28 AND duration<60);
-- So we finally found out: the accomplice's name is ROBIN!