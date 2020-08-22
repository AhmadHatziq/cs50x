SELECT movies.title FROM people
JOIN stars ON people.id = stars.person_id
JOIN movies on stars.movie_id = movies.id
WHERE people.name LIKE 'JOHNNY DEPP'
INTERSECT
SELECT movies.title FROM people
JOIN stars ON people.id = stars.person_id
JOIN movies on stars.movie_id = movies.id
WHERE people.name LIKE 'HELENA BONHAM CARTER'
