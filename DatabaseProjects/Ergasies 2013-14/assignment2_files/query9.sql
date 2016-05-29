#Για κάθε ζεύγος ειδών (genre_id’s) ταινιών, βρείτε τον αριθμό των σκηνοθετών
#που έχουν σκηνοθετήσει ταινίες και των δύο ειδών.


/*
select MHD.director_id
from movie_has_genre as MHG,movie_has_director as MHD
where MHG.movie_id=MHD.movie_id
*/
select G1.genre_id as GG1,G2.genre_id as GG2,count(*) as CNTT
from genre as G1,genre as G2,director as DIR
where G1.genre_id<G2.genre_id and
DIR.director_id in 
					(	select MHD.director_id
						from movie_has_genre as MHG,
							movie_has_director as MHD
						where MHG.movie_id=MHD.movie_id
						and MHG.genre_id=G1.genre_id)
and DIR.director_id in 
					(	select MHD.director_id
						from movie_has_genre as MHG,
							movie_has_director as MHD						
						where MHG.movie_id=MHD.movie_id
						and MHG.genre_id=G2.genre_id)
group by G1.genre_id ,G2.genre_id
