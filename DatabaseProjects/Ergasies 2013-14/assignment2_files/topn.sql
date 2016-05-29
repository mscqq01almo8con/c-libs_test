/*selectTopNactors: Η συνάρτηση αυτή δέχεται ως όρισμα έναν ακέραιο Ν. Για κάθε
είδος ταινίας, βρίσκει τους N ηθοποιούς που έχουν παίξει στις περισσότερες ταινίες του
είδους. Επιστρέφει τα εξής:
○ To όνομα του είδους.
○ Toν κωδικο κάθε ηθοποιού.
○ Τον αριθμό των ταινιών του είδους στις οποίες έχει παίξει ο κάθε ηθοποιός.*/

/*ayto to eidos to epaikse o ithopoios se 'count' tainies. */
/*select distinct mhg.genre_id,rol.actor_id,count(*) as cnt
from movie_has_genre as mhg,role as rol
where mhg.movie_id=rol.movie_id

group by mhg.genre_id,rol.actor_id
order by mhg.genre_id asc,count(distinct mhg.movie_id) desc*/
select gen.genre_name,rol.actor_id,count(*)
from movie_has_genre as mhg,role as rol,genre as gen
where mhg.movie_id=rol.movie_id
and mhg.genre_id=43  and mhg.genre_id=gen.genre_id
group by mhg.genre_id,rol.actor_id
order by mhg.genre_id asc,count(distinct mhg.movie_id) desc limit 10
