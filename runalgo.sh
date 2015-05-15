echo "Category: $1, Algo: $2, Size: $3"

export BASE_PATH="$(pwd)"
export SIMULATOR="$BASE_PATH/mips/mips.x --load="
export OUTPUT_DIR="$BASE_PATH/output/$1/$2/$3"
export MC723_OUTPUT_BRANCHES="$OUTPUT_DIR/prediction"

echo "mkdir -p $MC723_OUTPUT_BRANCHES"
mkdir -p $MC723_OUTPUT_BRANCHES

cd $OUTPUT_DIR
echo "Starting execution: $(date)" | tee -a $OUTPUT_DIR/debug.out

cd $BASE_PATH/MipsMibench/$1/$2
./runme_$3.sh 2>&1 | tee -a $OUTPUT_DIR/debug.out
echo "FINISHED $(date)" | tee -a $OUTPUT_DIR/debug.out
