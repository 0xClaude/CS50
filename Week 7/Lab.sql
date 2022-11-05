-- 1.sql
SELECT name FROM songs;

-- 2.sql
SELECT name FROM songs ORDER BY tempo;

-- 3.sql
SELECT name FROM songs ORDER BY duration_ms DESC LIMIT 5;

-- 4.sql
SELECT name FROM songs WHERE danceability > 0.75 and energy > 0.75 AND valence > 0.75;

-- 5.sql
SELECT AVG(energy) FROM songs;

-- 6.sql
SELECT songs.name FROM songs JOIN artists ON songs.artist_id = artists.id WHERE artists.name = "Post Malone";

-- 7.sql
SELECT AVG(songs.energy) FROM songs JOIN artists ON songs.artist_id = artists.id WHERE artists.name = "Drake";

-- 8.sql
SELECT songs.name FROM songs JOIN artists ON songs.artist_id = artists.id WHERE songs.name LIKE "%feat.%";