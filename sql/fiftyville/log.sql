-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Problem link: https://cs50.harvard.edu/x/2022/psets/7/fiftyville/

-- Look for a crime scene report that matches the date and the location of the crime (July 28, 2021 on Humphrey Street)
SELECT description
FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day = 28 AND street = "Humphrey Street";
-- Theft took place at 10:15am at the bakery - 3 witnesses

-- Look at transcription of witnesses' interviews
SELECT transcript
FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28 AND transcript LIKE "%bakery%";
-- 10 minutes after theft thief got into a car in the bakery parking lot - check security footage
-- Ealier that morning thief seen withdrawing some money from ATM on Leggett street
-- Thief made phone call (< 1 minute) while leaving bakery - planning to take earliest flight away next day

-- Check bakery security logs to get license plates of suspects
SELECT license_plate
FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = "exit";

-- Check ATM transactions from that day
SELECT account_number
FROM atm_transactions
WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw";

-- Get the id of people that withdrew money on that day
SELECT person_id FROM bank_accounts
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw";

-- Check phone records
SELECT caller,
FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60;

-- Compare people that withdrew money with licence plates and phone records
SELECT name
FROM people
WHERE id IN (
    SELECT person_id FROM bank_accounts
    JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
    WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"
) AND license_plate IN (
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = "exit"
) AND phone_number IN (
    SELECT caller
    FROM phone_calls
    WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60
);
-- Suspects: Diana, Bruce

-- Check next morning flights
SELECT id
FROM flights
WHERE origin_airport_id = (
    SELECT id
    FROM airports
    WHERE city = "Fiftyville"
) AND year = 2021 AND month = 7 AND day = 29
ORDER BY hour
LIMIT 1;

-- Check destination of flight
SELECT city
FROM airports
WHERE id = (
    SELECT destination_airport_id
    FROM flights
    WHERE origin_airport_id = (
        SELECT id
        FROM airports
        WHERE city = "Fiftyville"
    ) AND year = 2021 AND month = 7 AND day = 29
    ORDER BY hour
    LIMIT 1
);
-- Destination = New York City

-- Check passangers on flight
SELECT passport_number
FROM passengers
WHERE flight_id = (SELECT id
    FROM flights
    WHERE origin_airport_id = (
        SELECT id
        FROM airports
        WHERE city = "Fiftyville"
    ) AND year = 2021 AND month = 7 AND day = 29
    ORDER BY hour
    LIMIT 1
);

-- Compare people that withdrew money with licence plates, phone records and passengers on first flight in the morning
SELECT name
FROM people
WHERE id IN (
    SELECT person_id FROM bank_accounts
    JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
    WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"
) AND license_plate IN (
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = "exit"
) AND phone_number IN (
    SELECT caller
    FROM phone_calls
    WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60
) AND passport_number in (
    SELECT passport_number
    FROM passengers
    WHERE flight_id = (SELECT id
        FROM flights
        WHERE origin_airport_id = (
            SELECT id
            FROM airports
            WHERE city = "Fiftyville"
        ) AND year = 2021 AND month = 7 AND day = 29
        ORDER BY hour
        LIMIT 1
    )
);
-- Thief: Bruce

-- Check who Bruce called (accomplice)
SELECT name
FROM people
WHERE phone_number = (
    SELECT receiver
    FROM phone_calls
    WHERE caller = (
        SELECT phone_number
        FROM people
        WHERE name = "Bruce"
    ) AND year = 2021 AND month = 7 AND day = 28 AND duration < 60
);
-- Accomplice: Robin