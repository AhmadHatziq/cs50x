SELECT DISTINCT(name) FROM stars
JOIN movies on movies.id = stars.movie_id
JOIN people on stars.person_id = people.id
WHERE movies.year = 2004
ORDER BY people.birth