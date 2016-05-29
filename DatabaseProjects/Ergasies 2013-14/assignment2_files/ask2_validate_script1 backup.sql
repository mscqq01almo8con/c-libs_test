# --------------------------------------
# --------------------------------------
DROP PROCEDURE IF EXISTS ValidateQuery;
DELIMITER //
CREATE PROCEDURE ValidateQuery(IN qNum INT, IN queryTableName VARCHAR(255))
BEGIN
	DECLARE cname VARCHAR(64);
	DECLARE done INT DEFAULT FALSE;
	DECLARE cur CURSOR FOR SELECT c.column_name FROM information_schema.columns c WHERE 
c.table_schema='movies' AND c.table_name=queryTableName;
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

	# Add the column fingerprints into a tmp table
	DROP TABLE IF EXISTS cFps;
	CREATE TABLE cFps (
  	  `val` VARCHAR(50) NOT NULL
	) 
	ENGINE = InnoDB;

	OPEN cur;
	read_loop: LOOP
		FETCH cur INTO cname;
		IF done THEN
      			LEAVE read_loop;
    		END IF;
		
		DROP TABLE IF EXISTS ordered_column;
		SET @order_by_c = CONCAT('CREATE TABLE ordered_column as SELECT ', cname, ' FROM ', queryTableName, ' ORDER BY ', cname);
		PREPARE order_by_c_stmt FROM @order_by_c;
		EXECUTE order_by_c_stmt;
		
		SET @query = CONCAT('SELECT md5(group_concat(', cname, ', "")) FROM ordered_column INTO @cfp');
		PREPARE stmt FROM @query;
		EXECUTE stmt;

		INSERT INTO cFps values(@cfp);
		DROP TABLE IF EXISTS ordered_column;
	END LOOP;
	CLOSE cur;

	# Order fingerprints
	DROP TABLE IF EXISTS oCFps;
	SET @order_by = 'CREATE TABLE oCFps as SELECT val FROM cFps ORDER BY val'; 
	PREPARE order_by_stmt FROM @order_by;
	EXECUTE order_by_stmt;

	# Read the values of the result
	SET @q_yours = 'SELECT md5(group_concat(val, "")) FROM oCFps INTO @yours';
	PREPARE q_yours_stmt FROM @q_yours;
	EXECUTE q_yours_stmt;

	SET @q_fp = CONCAT('SELECT fp FROM fingerprints WHERE qnum=', qNum,' INTO @rfp');
	PREPARE q_fp_stmt FROM @q_fp;
	EXECUTE q_fp_stmt;

	SET @q_diagnosis = CONCAT('select IF(@rfp = @yours, "OK", "ERROR") into @diagnosis');
	PREPARE q_diagnosis_stmt FROM @q_diagnosis;
	EXECUTE q_diagnosis_stmt;

	INSERT INTO results values(qNum, @rfp, @yours, @diagnosis);

	DROP TABLE IF EXISTS cFps;
	DROP TABLE IF EXISTS oCFps;
END//
DELIMITER ;

# --------------------------------------

# Execute queries (Insert here your queries).

# Validate the queries
drop table if exists results;
CREATE TABLE results (
  `qnum` INTEGER  NOT NULL,
  `rfp` VARCHAR(50)  NOT NULL,
  `yours` VARCHAR(50)  NOT NULL,
  `diagnosis` VARCHAR(10)  NOT NULL
)
ENGINE = InnoDB;


# -------------
# Q1
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

#select * from actor a where a.actor_id = 933;
select MOV.title 
	from movie as MOV,
		 actor as ACT ,
		 role AS ROLE ,
		 genre as GEN ,
		movie_has_genre as MHG
	where 	ACT.last_name="Allen" 
		AND ACT.actor_id=ROLE.actor_id 
		AND ROLE.movie_id=MOV.movie_id

		AND GEN.genre_name="Comedy"
		AND MHG.genre_id=GEN.genre_id
		AND MHG.movie_id=MOV.movie_id;

CALL ValidateQuery(1, 'q');
drop table if exists q;
# -------------


# -------------
# Q2
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

#select * from actor a where a.actor_id = 933;
select DIR.last_name , MOV.title
from director as DIR , movie as MOV,movie_has_director
where  
	MOV.title IN	(
			select MOV1.title 
			from movie as MOV1,actor as ACT,role as ROLE
			where ACT.last_name="Allen"
				and ACT.actor_id=ROLE.actor_id 
				and ROLE.movie_id=MOV1.movie_id) 

	
and(
select COUNT(DISTINCT GEN.genre_id)
			from movie as MOV2,genre as GEN,
				movie_has_director,movie_has_genre
			where   DIR.director_id=movie_has_director.director_id
				and movie_has_director.movie_id=MOV2.movie_id
				and MOV2.movie_id=movie_has_genre.movie_id
				and movie_has_genre.genre_id=GEN.genre_id)>1
and DIR.director_id=movie_has_director.director_id
and MOV.movie_id=movie_has_director.movie_id

;
CALL ValidateQuery(2, 'q');
drop table if exists q;
# -------------


# -------------
# Q3
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

select distinct ACT.last_name 
from actor as ACT, genre as GEN,movie as MOV
where  exists
(
	select distinct ACT.last_name 
	from  director as DIR ,
		movie_has_director as MHD,role as ROLE
	where MOV.movie_id=MHD.movie_id
			and MHD.director_id=DIR.director_id
			and ACT.actor_id=ROLE.actor_id
			and ROLE.movie_id=MOV.movie_id
			and ACT.last_name=DIR.last_name) 
and exists 
(
	select distinct ACT.last_name 
	from  movie as MOV1, director as DIR1 ,
		movie_has_director as MHD1,role as ROLE1,
		movie_has_genre as MHG1

	where MOV1.movie_id=MHD1.movie_id
			and MHD1.director_id=DIR1.director_id
			and ACT.actor_id=ROLE1.actor_id
			and ROLE1.movie_id=MOV1.movie_id
			and ACT.last_name<>DIR1.last_name
			and MOV1.movie_id=MHG1.movie_id 
			and MHG1.genre_id=GEN.genre_id
)
#vres tis tainies me dedomeno genre poy paizoun ithopoioi me 
#idio onoma me to skinotheti

and exists(
	select MOV2.movie_id
	from director as DIR,movie as MOV2, actor as ACT2,genre as GEN2,
	movie_has_director as MHD, movie_has_genre as MHG#
	where GEN2.genre_name=GEN.genre_name and 
			GEN2.genre_id=MHG.genre_id and
			MHG.movie_id=MOV2.movie_id and
			DIR.last_name=ACT2.last_name and
			DIR.director_id=MHD.director_id and
			MOV2.movie_id=MHD.movie_id and
			Not exists (
									select ACT2.Actor_id
									from role as ROLE1
									where ACT2.actor_id = ROLE1.actor_id
										and ROLE1.movie_id=MOV2.movie_id
								 )
);

CALL ValidateQuery(3, 'q');
drop table if exists q;
# -------------


# -------------
# Q4
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.
/*
(select  "yes"
from movie as MOV , genre as GEN, movie_has_genre as MHG
where GEN.genre_name="Drama" and MHG.genre_id=GEN.genre_id
and MHG.movie_id=MOV.movie_id and MOV.year='1995')
union(
select  "no" from movie as MOV where MOV.movie_id<0)*/
(
	select  "yes" as Existence
	from movie as MOV , genre as GEN, movie_has_genre as MHG
	where GEN.genre_name="Drama" and MHG.genre_id=GEN.genre_id
	and MHG.movie_id=MOV.movie_id and MOV.year='1995'
)
union
(
	select  "no" as Existence 
	from movie as MOV 
	where not exists	(
							select  MOV.movie_id
							from movie as MOV , genre as GEN, 
									movie_has_genre as MHG
							where GEN.genre_name="Drama" and MHG.genre_id=GEN.genre_id
									and MHG.movie_id=MOV.movie_id and MOV.year='1995'
						)
);


CALL ValidateQuery(4, 'q');
drop table if exists q;
# -------------


# -------------
# Q5
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

select distinct DIR2.last_name as direc1,DIR1.last_name as direc2
from movie as MOV,director as DIR1, director as DIR2,
		movie_has_director as MHD1,movie_has_director as MHD2
where DIR1.director_id=MHD1.director_id and 
		DIR2.director_id=MHD2.director_id
		and MHD1.movie_id = MOV.movie_id 
		and MHD2.movie_id = MOV.movie_id 
		and DIR1.director_id<DIR2.director_id
and MHD1.director_id<MHD2.director_id 
and (MOV.year between 2000 and 2006) and (

select COUNT(distinct GEN.genre_name)
from movie as MOV1 ,
	genre as GEN,movie_has_genre as MHG1,
	movie_has_director as MHD1,movie_has_director as MHD2
where  DIR1.director_id=MHD1.director_id and 
		DIR2.director_id=MHD2.director_id and
		MHD1.movie_id = MOV1.movie_id and
		MHD2.movie_id = MOV1.movie_id 
and DIR1.director_id<DIR2.director_id
and MHD1.director_id<MHD2.director_id and
GEN.genre_id=MHG1.genre_id and MHG1.movie_id=MOV1.movie_id

)>=6;


CALL ValidateQuery(5, 'q');
drop table if exists q;
# -------------


# -------------
# Q6
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

select distinct ACT.first_name as A,ACT.last_name as B,count(*) as cnt
from actor as ACT,director as DIR
where ACT.actor_id in (
						select ACT.actor_id
						from actor as ACT,role as ROL
						where ACT.actor_id=ROL.actor_id
						group by ROL.actor_id
						having COUNT(*)=3
						)
and DIR.director_id in (select MHD.director_id
							from movie_has_director as MHD,role as RL
 where ACT.actor_id=RL.actor_id and MHD.movie_id=RL.movie_id
)
group by ACT.actor_id;


CALL ValidateQuery(6, 'q');
drop table if exists q;
# -------------


# -------------
# Q7
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.


select distinct MOV.movie_id as II,GEN.genre_id as QQ,COUNT(*) as cnt
from movie as MOV,genre as GEN,
	movie_has_director as MHD,movie_has_genre as MHG,
	director as DIR
where MHG.movie_id=MOV.movie_id and MHG.genre_id=GEN.genre_id and
MOV.movie_id=MHD.movie_id and MHD.director_id=DIR.director_id and (GEN.genre_id) in
(					select distinct GEN.genre_id
					from movie as MOV,genre as GEN,movie_has_genre as MHG
					where MHG.movie_id=MOV.movie_id and MHG.genre_id=GEN.genre_id
					group by MOV.movie_id,MOV.title 
					having count(*)=1)
group by GEN.genre_id;


CALL ValidateQuery(7, 'q');
drop table if exists q;
# -------------


# -------------
# Q8
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

select ACT.actor_id 
from actor as ACT, role as ROl, genre as GEN, 
	movie as MOV, movie_has_genre as MHG
where ACT.actor_id=ROl.actor_id
and ROL.movie_id=MOV.movie_id
and MOV.movie_id=MHG.movie_id
and MHG.genre_id=GEN.genre_id
group by ACT.actor_id
having count(*)>20;

CALL ValidateQuery(8, 'q');
drop table if exists q;
# -------------


# -------------
# Q9
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

select * from actor a where a.actor_id = 933;

CALL ValidateQuery(9, 'q');
drop table if exists q;
# -------------


# -------------
# Q10
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

select * from actor a where a.actor_id = 933;

CALL ValidateQuery(10, 'q');
drop table if exists q;
# -------------

update fingerprints set fp = "16f08135cdf55d5116e839a4f4a4007d" where qnum = 6;

update fingerprints set fp = "0cbdfe3a95ecc5394304b2a74d22e8df" where qnum = 10;

update fingerprints set fp = "6f79e2b031bba7dd11bd9bc1c342d6a4" where qnum = 3;

update fingerprints set fp = "285dd7b03568bec0641bcae2d2566efd" where qnum = 7;
DROP PROCEDURE IF EXISTS RealValue;
DROP PROCEDURE IF EXISTS ValidateQuery;
DROP PROCEDURE IF EXISTS RunRealQueries;