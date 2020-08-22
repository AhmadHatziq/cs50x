SELECT people.name FROM people
JOIN stars ON people.id = stars.person_id
JOIN movies ON stars.movie_id = movies.id
WHERE movies.id IN (
	SELECT movies.id FROM people
	JOIN stars ON people.id = stars.person_id
	JOIN movies ON stars.movie_id = movies.id
	WHERE name LIKE 'KEVIN BACON'
	AND birth = 1958
)
AND people.name <> 'Kevin Bacon'