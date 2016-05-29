#!bin/sh
#
#Usage: revstrs [string1 [string2 ...]]
#
for str
do
	strlen=`expr length "$str"`
	chind=$strlen
	while test $chind -gt 0
	do
		echo -n "`expr substr\"$str\"$chind 1`"
		chind=`expr $chind - 1`
	done
	echo -n " --> "
	echo -n "$strlen"
	echo " character(s)."
done
