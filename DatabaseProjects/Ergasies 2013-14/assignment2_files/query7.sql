/*Για κάθε ταινία που έχει ακριβώς ένα είδος, βρείτε το είδος καθώς και τον αριθμό
των σκηνοθετών που έχουν σκηνοθετήσει αυτό το είδος.
*/


#TAINIES ME AKRIVOS ENA EIDOS
/*
select distinct movie_id
from movie_has_genre

group by movie_id
having count(*)=1
*/

#ARITHMOS SKINOTHETON POU GYRISAN ENA EIDOS

select distinct GEN.genre_id as r,count(distinct MHD.director_id) as c
from movie_has_genre as MHG,movie_has_director MHD,genre as GEN
where MHG.movie_id=MHD.movie_id and GEN.genre_id=MHG.genre_id

and MHG.genre_id in (
						select distinct MHG.genre_id
						from movie_has_genre as MHG
						where MHG.movie_id in (
									select distinct MHG.movie_id
									from movie_has_genre as MHG
									group by MHG.movie_id
									having count(*)=1)


)
group by GEN.genre_id


