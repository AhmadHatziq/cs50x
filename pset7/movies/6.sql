SELECT AVG(rating) FROM movies
JOIN ratings on movies.id = ratings.movie_id
WHERE movies.year = 2012