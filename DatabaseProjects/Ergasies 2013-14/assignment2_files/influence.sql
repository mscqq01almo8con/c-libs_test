select distinct rol2.actor_id ,/*act1.last_name,act2.last_name, 
	mov1.title,mov1.year,mov2.title,mov2.year,*/gen.genre_name
from role as rol1,role as rol2,movie_has_genre as mhg1,
	movie_has_genre as mhg2,movie as mov1,movie as mov2,
actor as act1,actor as act2,genre as Gen
where rol1.actor_id=353656 and
#o 1 paizei se tainia 1 me eidos 1
rol1.movie_id=mhg1.movie_id and
mov1.movie_id=mhg1.movie_id and
exists (
#yparxei gia tin tainia 1 ithopoios 2
select rol.movie_id from role as rol where
			rol1.movie_id=rol.movie_id and 
			mhg1.movie_id=rol1.movie_id and 
			rol2.actor_id=rol.actor_id) and

#ithopoios 2 != ithopoios 1
rol1.actor_id<>rol2.actor_id and

#o ithopoios 2 paizei se tainia 2 idiou eidous me tin 1
rol2.movie_id=mhg2.movie_id and
mhg1.genre_id=mhg2.genre_id and 
mhg2.movie_id=mov2.movie_id and

#i tainia 2 einai metagenesteri tou 1
mov2.year>mov1.year


and act1.actor_id=rol1.actor_id
and act2.actor_id=rol2.actor_id
and gen.genre_id=mhg1.genre_id
