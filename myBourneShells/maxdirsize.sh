#! /bin/sh
#
# Usage: maxsize
#
echo -n "Please specify the directory names: "
read dirs ; set - $dirs ; ndirs=$#
max=0
while [ $ndirs -ne 0 ]
do
dir=$1 ; shift ; dirs=$*
if [ ! -d $dir ]
then
echo "$0 : There is no directory $dir"
else
duout=`du -s $dir` ; set - $duout ; size=$1
if [ $size -gt $max ]
then
max=$size ; maxdir=$dir
fi
fi
set - $dirs
ndirs=`expr $ndirs - 1`
done
echo "$maxdir $max"
