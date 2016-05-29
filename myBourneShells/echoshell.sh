echo $0
echo $1
echo $*
echo $#
read a
echo $a
if [ $# -eq 0 ]
then
	echo "1 arg"
elif [ $# -ge 2 ]
then
	echo "toomany"
else
	ls -Rl $1 | grep '^d' | colrm 1 43
fi

