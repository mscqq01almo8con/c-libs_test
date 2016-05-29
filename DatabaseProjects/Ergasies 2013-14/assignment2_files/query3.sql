/*-----------------------------------------------------------------------*/
select distinct ACT.last_name 
from actor as ACT, genre as GEN,movie as MOV
#katarxin o ithopoios paizei se tainia me skinotheti me to idio eponymo
where  exists(	select distinct ACT.last_name 
				from  director as DIR ,
					movie_has_director as MHD,role as ROLE
				where MOV.movie_id=MHD.movie_id
						and MHD.director_id=DIR.director_id
						and ACT.actor_id=ROLE.actor_id
						and ROLE.movie_id=MOV.movie_id
						and ACT.last_name=DIR.last_name) 
#o ithopoios paizei se tainia skinotheti me allo eponymo
and exists ( 	select distinct ACT.last_name 
				from  movie as MOV, director as OTHER_DIR ,
					movie_has_director as MHOND,role as ROLE,
					movie_has_genre as MHG
			/*MHOND=Movie Has 'Other Name' Director*/
				where MOV.movie_id=MHOND.movie_id
						and MHOND.director_id=OTHER_DIR.director_id
						and ACT.actor_id=ROLE.actor_id
						and ROLE.movie_id=MOV.movie_id
						and ACT.last_name<>OTHER_DIR.last_name
						and MOV.movie_id=MHG.movie_id 
						and MHG.genre_id=GEN.genre_id)
#kai yparxei tainia idiou eidous (GEN2.genre_id=GEN.genre_id) pou exei skinothetisei
#skinothetis (DIR) me to idio eponymo (ACT.last_name), alla den paizei o ithopoios (ACT)
and exists(	select MOV2.movie_id
				from director as DIR,movie as MOV2, 
					actor as ACT,genre as GEN2,
					movie_has_director as MHD, 
					movie_has_genre as MHG#
				where GEN2.genre_name=GEN.genre_name and 
						GEN2.genre_id=MHG.genre_id and
						MHG.movie_id=MOV2.movie_id and
						DIR.last_name=ACT.last_name and
						DIR.director_id=MHD.director_id and
						MOV2.movie_id=MHD.movie_id and
						Not exists (select ACT.Actor_id
									 from role as ROLE1
									 where ACT.actor_id = ROLE1.actor_id
										 and ROLE1.movie_id=MOV2.movie_id)
			);