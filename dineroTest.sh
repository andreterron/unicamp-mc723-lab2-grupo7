
#===========================
# Prepare dinero command
#===========================

PROC=$1
ARGS=""

INPUT="mips/trace.din"
if [ "$2" == "-i" ] || [ "$2" == "-v" ]; then
	INPUT="$3"
fi

if [ "$PROC" == "-C1" ]; then
	N=1
	i=$N
	ARGS="$ARGS -l1-dsize 32K -l1-isize 32K -l1-ibsize 64 -l1-dbsize 64"
	ARGS="$ARGS -l1-dassoc 4 -l1-iassoc 4"
	ARGS="$ARGS -l1-drepl l -l1-irepl l"
	
elif [ "$PROC" == "-C2" ]; then
	N=1
	ARGS="$ARGS -l1-dsize 64K -l1-isize 64K -l1-ibsize 64 -l1-dbsize 64"
	ARGS="$ARGS -l1-dassoc 4 -l1-iassoc 4"
	ARGS="$ARGS -l1-drepl l -l1-irepl l"
	
elif [ "$PROC" == "-ARM" ]; then
	N=2
	ARGS="$ARGS -l1-dsize 32K -l1-isize 32K -l1-ibsize 64 -l1-dbsize 64"
	ARGS="$ARGS -l1-dassoc 2 -l1-iassoc 2"
	ARGS="$ARGS -l1-drepl l -l1-irepl l"
	
	ARGS="$ARGS -l2-dsize 1M -l2-isize 1M -l2-ibsize 64 -l2-dbsize 64"
	ARGS="$ARGS -l2-dassoc 16 -l2-iassoc 16"
	ARGS="$ARGS -l2-drepl l -l2-irepl l"

elif [ "$PROC" == "-INTEL" ]; then
	N=3
	ARGS="$ARGS -l1-dsize 32K -l1-isize 32K -l1-ibsize 64 -l1-dbsize 64"
	ARGS="$ARGS -l1-dassoc 8 -l1-iassoc 8"
	ARGS="$ARGS -l1-drepl l -l1-irepl l"
	
	ARGS="$ARGS -l2-dsize 256K -l2-isize 256K -l2-ibsize 64 -l2-dbsize 64"
	ARGS="$ARGS -l2-dassoc 8 -l2-iassoc 8"
	ARGS="$ARGS -l2-drepl l -l2-irepl l"
	
	ARGS="$ARGS -l3-dsize 8M -l3-isize 8M -l3-ibsize 64 -l3-dbsize 64"

fi


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
if [ -z PROCESSOR ]; then
	ACCESS_COST_D[1]=1   # L1 data
	ACCESS_COST_I[1]=2   # L1 instruction
	ACCESS_COST_D[2]=6  # L2 data
	ACCESS_COST_I[2]=8  # L2 instruction
	ACCESS_COST_D[3]=18 # L3 data
	ACCESS_COST_I[3]=24 # L3 instruction
	ACCESS_COST_D[4]=100 # MEM data
	ACCESS_COST_I[4]=120 # MEM instruction
else
	ACCESS_COST_D[1]=1   # L1 data
	ACCESS_COST_I[1]=2   # L1 instruction
	ACCESS_COST_D[2]=6  # L2 data
	ACCESS_COST_I[2]=8  # L2 instruction
	ACCESS_COST_D[3]=18 # L3 data
	ACCESS_COST_I[3]=24 # L3 instruction
	ACCESS_COST_D[4]=100 # MEM data
	ACCESS_COST_I[4]=120 # MEM instruction
fi	
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
	#MISS=`expr "$line" : 'Demand Misses *\([0-9]*\)'`
	MISS=`echo "$line" | awk '{print $3}'`

	if [ "$TYPE" == "0" ]; then
		TYPE_NAME="data        "
		LEVEL_COST=` expr $MISS \* ${MISS_COST_D[$LEVEL]}`
	elif [ "$TYPE" == "1" ]; then
		TYPE_NAME="instructions"
		LEVEL_COST=` expr $MISS \* ${MISS_COST_I[$LEVEL]}`
	fi
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
