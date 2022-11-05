-- 1) List the titles of all movies released in 2008.
SELECT title FROM movies
WHERE year = 2008;

-- 2) Determine the birth year of Emma Stone.
SELECT birth FROM people
WHERE name = "Emma Stone";

-- 3) List the titles of all movies with a release date on or after 2018, in alphabetical order.
SELECT title FROM movies
WHERE year > 2017
ORDER BY title;

-- 4) Determine the number of movies with an IMDb rating of 10.0.
SELECT COUNT(*) FROM movies
JOIN ratings ON movies.id = ratings.movie_id
WHERE ratings.rating = 10;

-- 5) List the titles and release years of all Harry Potter movies, in chronological order.
SELECT title, year FROM movies
WHERE title LIKE "Harry Potter%"
ORDER BY year;

-- 6) Determine the average rating of all movies released in 2012.
SELECT AVG(ratings.rating) FROM ratings
JOIN movies ON movies.id = ratings.movie_id
WHERE movies.year = 2012;

-- 7) List all movies released in 2010 and their ratings, in descending order by rating.
-- For movies with the same rating, order them alphabetically by title.
SELECT movies.title, ratings.rating FROM movies
JOIN ratings ON movies.id = ratings.movie_id
WHERE movies.year = 2010
ORDER BY ratings.rating DESC, movies.title;

-- 8) List the names of all people who starred in Toy Story.
SELECT people.name FROM people
JOIN stars ON people.id = stars.person_id
JOIN movies ON stars.movie_id = movies.id
WHERE movies.title = "Toy Story";

-- 9) List the names of all people who starred in a movie released in 2004, ordered by birth year.
SELECT DISTINCT people.name FROM people
JOIN stars ON people.id = stars.person_id
JOIN movies ON stars.movie_id = movies.id
WHERE movies.year = 2004 ORDER BY people.birth;

-- 10) List the names of all people who have directed a movie that received a rating of at least 9.0.
SELECT people.name FROM people
JOIN directors ON people.id = directors.person_id
JOIN movies ON directors.movie_id = movies.id
JOIN ratings ON movies.id = ratings.movie_id
WHERE ratings.rating >= 9.0;

-- 11) List the titles of the five highest rated movies (in order) that Chadwick Boseman starred in, starting with the highest rated.
SELECT movies.title FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
JOIN ratings ON movies.id = ratings.movie_id
WHERE people.name = "Chadwick Boseman"
ORDER BY ratings.rating DESC
LIMIT 5;

-- 12) List the titles of all movies in which both Johnny Depp and Helena Bonham Carter starred
SELECT movies.title FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
WHERE people.name = "Johnny Depp"
INTERSECT
SELECT movies.title FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
WHERE people.name = "Helena Bonham Carter";

-- 13) list the names of all people who starred in a movie in which Kevin Bacon also starred
SELECT name FROM people
WHERE id IN (
    SELECT DISTINCT person_id FROM stars
    WHERE movie_id IN (
        SELECT movie_id FROM stars
        WHERE person_id IN (
            SELECT id FROM people
            WHERE name = "Kevin Bacon" and birth = 1958
)))
AND NAME != "Kevin Bacon";