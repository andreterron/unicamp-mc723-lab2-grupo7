#include "prediction/PredictionOracle.h"
#include "prediction/BranchPredictor.h"

#define SAFE_DELETE(ptr) \
    do { \
        if ( ptr != NULL ) { \
            delete ptr; \
            ptr = NULL; \
        } \
    } while (0)

static const unsigned int BUFFER_DELTA = 10;

PredictionOracle::PredictionOracle(void)
    :   mNumPredictors(0)
    ,   mBufferSize(BUFFER_DELTA)
{
    mPredictorIds = new std::string[mBufferSize];
    mPredictors = new BranchPredictor*[mBufferSize];
}

PredictionOracle::~PredictionOracle(void)
{
    for ( unsigned int i = 0; i < mNumPredictors; i++ ) {
        BranchPredictor* bp = mPredictors[i];
        SAFE_DELETE(bp);
    }
    SAFE_DELETE(mPredictorIds);
    SAFE_DELETE(mPredictors);
}

bool PredictionOracle::AddPredictor(const std::string& id, BranchPredictor* predictor)
{
    int pos = mBufferSize + 1;
    for ( unsigned int i = 0; i < mNumPredictors; i++ ) {
        if ( id.compare(mPredictorIds[i]) == 0 ) {
            return false;
        }
        if ( (mPredictors[i] == NULL ) && ( pos > mBufferSize ) ) {
            pos = i;
        }
    }
    if ( pos > mBufferSize ) {
        pos = mNumPredictors++;
        if ( mNumPredictors > mBufferSize ) {
            unsigned int bs_new = mBufferSize + BUFFER_DELTA;

            std::string* s_tmp = new std::string[bs_new];
            if ( s_tmp == NULL ) {
                return false; 
            }

            BranchPredictor** bp_tmp = new BranchPredictor*[bs_new];
            if ( bp_tmp == NULL ) {
                return false;
            }
            
            memcpy(s_tmp, mPredictorIds, mBufferSize * sizeof(std::string));
            memcpy(bp_tmp, mPredictors, mBufferSize * sizeof(BranchPredictor*));
            
            {
                std::string* tmp = s_tmp;
                s_tmp = mPredictorIds;
                mPredictorIds = tmp;
                SAFE_DELETE(tmp);
            }
            
            {
                BranchPredictor** tmp = bp_tmp;
                bp_tmp = mPredictors;
                mPredictors = tmp;
                SAFE_DELETE(tmp);
            }
            mBufferSize = bs_new;
        }
    }
    mPredictorIds[pos] = id;
    mPredictors[pos] = predictor;

    return true;
}

bool PredictionOracle::RemovePredictor(const std::string& id)
{
    for ( unsigned int i = 0; i < mNumPredictors; i++ ) {
        BranchPredictor* bp = mPredictors[i];
        std::string&     l_id = mPredictorIds[i];
        if ( id.compare(l_id) == 0 ) {
            SAFE_DELETE(bp);
            l_id.clear();
            return true;
        }
    }
    return false;
}

void PredictionOracle::AddInstruction(const Instruction* instruction)
{
    for ( unsigned int i = 0; i < mNumPredictors; i++ ) {
        BranchPredictor* bp = mPredictors[i];
        if ( bp != NULL ) {
            bp->AddInstruction(instruction);
        }
    }
}

void PredictionOracle::Hit(const bool branched)
{
    for ( unsigned int i = 0; i < mNumPredictors; i++ ) {
        BranchPredictor* bp = mPredictors[i];
        if ( bp != NULL ) {
            bp->Hit(branched);
        }
    }
}

void PredictionOracle::ExportAll(const std::string& filename) const
{
    std::fstream file(filename, std::fstream::out | std::fstream::trunc);

    for ( unsigned int i = 0; i < mNumPredictors; i++ ) {
        BranchPredictor* bp = mPredictors[i];
        std::string&     id = mPredictorIds[i];
        if ( bp != NULL && !id.empty() ) {
            file << id << std::endl;
            bp->AppendResults(file);
        }
    }
}

void PredictionOracle::ExportEach(const std::string& path) const
{
    for ( unsigned int i = 0; i < mNumPredictors; i++ ) {
        BranchPredictor* bp = mPredictors[i];
        std::string&     id = mPredictorIds[i];
        if ( bp != NULL && !id.empty() ) {
            char filename[2048];
            snprintf(filename, 2047, "%s/%s", path.c_str(), id.c_str());

            std::fstream file(filename, std::fstream::out | std::fstream::trunc);
            bp->AppendResults(file);
        }
    }
}
