#! /bin/sh
#
# Usage: once
#
cur_date=`date +%d%m%y`
last_date=""

#True if FILE exists and has a size greater than zero.
if [ -s $HOME/.date ]
then
	last_date=`cat $HOME/.date`
fi
	echo "$cur_date" > $HOME/.date
if [ "$last_date" != "$cur_date" ]
then
	echo "Hello!! How are you today?"
fi
