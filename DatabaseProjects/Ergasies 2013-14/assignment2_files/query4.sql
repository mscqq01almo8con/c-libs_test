#Anti gia 'if' kanoume mia domi me union kai amoivaios apokleiomena apotelesmata
(#an yparxei tainia me ayta ta kritiria, epistrefoume mia stili me "yes"
	select  "yes" as Existence
	from movie as MOV , genre as GEN, movie_has_genre as MHG
	where GEN.genre_name="Drama" and MHG.genre_id=GEN.genre_id
		and MHG.movie_id=MOV.movie_id and MOV.year='1995'
)
union
(#an den yparxei tainia me ayta ta kritiria, epistrefoume mia stili me "no"
	select  "no" as Existence 
	from movie as MOV 
	where not exists	(	select  MOV.movie_id
							from movie as MOV ,genre as GEN,movie_has_genre as MHG
							where GEN.genre_name="Drama" and MHG.genre_id=GEN.genre_id
									and MHG.movie_id=MOV.movie_id and MOV.year='1995')
);