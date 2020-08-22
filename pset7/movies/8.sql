SELECT name FROM stars JOIN people ON people.id = stars.person_id
WHERE stars.movie_id = (
	SELECT id FROM movies WHERE movies.title LIKE "TOY STORY"
)