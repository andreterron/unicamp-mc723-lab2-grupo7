CATEGORIES=$(ls MipsMibench)

for cat in $CATEGORIES
do
    TESTS=$(ls MipsMibench/$cat)
    for test in $TESTS
    do
        ./runalgo.sh $cat $test small
        ./runalgo.sh $cat $test large
    done
done
