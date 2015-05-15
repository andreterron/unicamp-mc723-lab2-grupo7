
#===========================
# Prepare dinero command
#===========================

N=$1
ARGS=""

INPUT="mips/trace.din"
if [ "$2" == "-i" ] || [ "$2" == "-v" ]; then
	INPUT="$3"
fi


for i in `seq 1 $N`;
do
	if [ "$i" == "1" ]; then

		# ARGUMENTS FOR L1 CACHE
		ARGS="$ARGS -l$i-dsize 1K -l$i-isize 1K -l$i-ibsize 16 -l$i-dbsize 8"

	elif [ "$i" == "2" ]; then

		# ARGUMENTS FOR L2 CACHE
		ARGS="$ARGS -l$i-dsize 2K -l$i-isize 2K -l$i-ibsize 32 -l$i-dbsize 32"

	elif [ "$i" == "3" ]; then

		# ARGUMENTS FOR L3 CACHE
		ARGS="$ARGS -l$i-dsize 8K -l$i-isize 8K -l$i-ibsize 64 -l$i-dbsize 64"

	fi
done


#===========================
# Execute dinero command
#===========================




RAW=`./d4-7/dineroIV $ARGS -informat d < $INPUT`
RES=` echo "$RAW" | grep 'Demand Misses'`
#echo "RES = $RES"
#RES="$( ./d4-7/dineroIV $ARGS -informat d < $INPUT | grep 'Demand Misses' )"
if [ "$2" == "-v" ]; then
	echo "$RAW"
	exit
fi

#===========================
# Extract misses
#===========================

TYPE=0
LEVEL=1
TYPE_NAME=""

ACCESS_COST_D[1]=1   # L1 data
ACCESS_COST_I[1]=2   # L1 instruction
ACCESS_COST_D[2]=6  # L2 data
ACCESS_COST_I[2]=8  # L2 instruction
ACCESS_COST_D[3]=18 # L3 data
ACCESS_COST_I[3]=24 # L3 instruction
ACCESS_COST_D[4]=100 # MEM data
ACCESS_COST_I[4]=120 # MEM instruction
COST=0
LEVEL_COST=0

#echo "log2"

for i in `seq 1 $N`;
do
	j=`expr $i + 1`
	MISS_COST_D[$i]=${ACCESS_COST_D[$j]}
	MISS_COST_I[$i]=${ACCESS_COST_I[$j]}
done

MISS_COST_D[$N]=${ACCESS_COST_D[4]}
MISS_COST_I[$N]=${ACCESS_COST_I[4]}

#echo "log3"

while read -r line; do
	#echo "LINE = $line"
	MISS=`expr "$line" : 'Demand Misses\s\+\([0-9]\+\)'`

	#echo "log4, $MISS, $LEVEL, ${MISS_COST_D[$LEVEL]}"
	if [ "$TYPE" == "0" ]; then
		TYPE_NAME="data        "
		LEVEL_COST=` expr $MISS * ${MISS_COST_D[$LEVEL]}`
	elif [ "$TYPE" == "1" ]; then
		TYPE_NAME="instructions"
		LEVEL_COST=` expr $MISS \* ${MISS_COST_I[$LEVEL]}`
	fi
	#echo "log5"
	COST=` expr $COST + $LEVEL_COST`
	echo "Cache L$LEVEL for $TYPE_NAME missed $MISS times, costing $LEVEL_COST cycles"
	LEVEL=`expr $LEVEL + $TYPE`
	TYPE=`expr \( $TYPE + 1 \) % 2`
	#[[ "${RES}" =~ ([0-9]+) ]] && echo ${BASH_REMATCH[1]}
	#echo "$line"
done <<< "$RES"

echo "Total miss cost = $COST cycles"


#echo "----------------------=-=-=-=-=-=---------------------"
#echo "${RES}" #| grep "Demand Misses"
#[[ "${RES}" =~ "Demand Misses" ]] && echo matched
