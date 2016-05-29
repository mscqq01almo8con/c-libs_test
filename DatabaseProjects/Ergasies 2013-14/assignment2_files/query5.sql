#epilegoume eponyma zeygon skinotheton pou gyrisan tainies
#anamesa sto 2000 kai to 2006 kai exoun gyrisei 
#toulaxiston 6 alla eidi o kathenas
select distinct DIR2.last_name as director1,DIR1.last_name as director2
from movie as MOV,director as DIR1, director as DIR2,
	movie_has_director as MHD1,movie_has_director as MHD2
where DIR1.director_id=MHD1.director_id and DIR2.director_id=MHD2.director_id
	and MHD1.movie_id = MOV.movie_id and MHD2.movie_id = MOV.movie_id 
	and DIR1.director_id<DIR2.director_id
	and MHD1.director_id<MHD2.director_id 
	and (MOV.year between 2000 and 2006) 
/* prepei o skinothetis na exei gyrisei toulaxiston 6 eidi*/
	and (	select COUNT(distinct GEN.genre_name)
			from movie as MOV1 ,
				genre as GEN,movie_has_genre as MHG1,
				movie_has_director as MHD1,movie_has_director as MHD2
			where  DIR1.director_id=MHD1.director_id
			and DIR2.director_id=MHD2.director_id
			and MHD1.movie_id = MOV1.movie_id
			and	MHD2.movie_id = MOV1.movie_id 
			and DIR1.director_id<DIR2.director_id
			and MHD1.director_id<MHD2.director_id
			and	GEN.genre_id=MHG1.genre_id
			and MHG1.movie_id=MOV1.movie_id)>=6;