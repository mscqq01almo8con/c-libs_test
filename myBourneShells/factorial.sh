#!bin/sh
#
#Usage: factorial
#
echo -n "Give input number: "
read n
fact=1
until [ $n -eq 0 ]
do
fact=`expr $fact \* $n`
n=`expr $n - 1`
done
echo $fact
