/*
2. colleaguesOfColleagues: Η συνάρτηση αυτή παίρνει
 ως όρισμα ένα ζευγάρι
 (a,b) από κωδικούς ηθοποιών (πεδίο id). 
Βρίσκει τις ταινίες που παίζουν 
μαζί δύο άλλοι ηθοποιοί
(c,d) με την προϋπόθεση ότι ο c έχει παίξει σε ταινία
 μαζί με τον a και ο d έχει παίξει σε
ταινία μαζί με τον b. Επιστρέφει τα εξής:
○ Τον τίτλο της κάθε ταινίας των (c,d).
○ Τον κωδικό του ηθοποιού c.
○ Τον κωδικό του ηθοποιού d.
○ Τον κωδικό του ηθοποιού a.
○ Τον κωδικό του ηθοποιού b.



*/
select distinct mov.title,rolc.actor_id,rold.actor_id,
					rola.actor_id,rolb.actor_id
from role as ROLC,role as ROLD,role as rola, role as rolb,movie as mov 
where 
rolc.movie_id=mov.movie_id and 
rola.movie_id=rolb.movie_id and rola.actor_id<rolb.actor_id 
and rolc.actor_id<rold.actor_id and rola.actor_id<>rolc.actor_id 
and rolb.actor_id<>rold.actor_id and rola.actor_id<>rold.actor_id 
and rolb.actor_id<>rolc.actor_id 
and exists (
			select ac.movie_id from role as ac where 
				rola.movie_id=rolc.movie_id and ac.movie_id=rola.movie_id) 
and exists (
			select bd.movie_id from role as bd where  
				rolb.movie_id=rold.movie_id and bd.movie_id=rolb.movie_id) 
and rolc.movie_id=rold.movie_id  
and rola.actor_id=933 and rolb.actor_id=2898 
group by rolc.actor_id,rold.actor_id
