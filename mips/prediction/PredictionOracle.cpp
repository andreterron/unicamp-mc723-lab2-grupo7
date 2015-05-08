#include "prediction/PredictionOracle.h"
#include "prediction/BranchPredictor.h"

PredictionOracle::PredictionOracle(void)
{}

PredictionOracle::~PredictionOracle(void)
{
    for ( PredictorMap::iterator it = mPredictors.begin(); it != mPredictors.end(); it++ ) {
        BranchPredictor* bp = it->second;
        if ( bp != NULL ) {
            delete bp;
        }
    }
}

bool PredictionOracle::AddPredictor(const std::string& id, BranchPredictor* predictor)
{
    try {
        BranchPredictor* bp = mPredictors.at(id);
        if ( bp != NULL ) {
            delete bp;
        }
    } catch (...) {
        // nop
    }
    mPredictors[id] = predictor;
}

bool PredictionOracle::RemovePredictor(const std::string& id)
{
    try {
        BranchPredictor* bp = mPredictors.at(id);
        mPredictors.erase(id);
        if ( bp != NULL ) {
            delete bp;
        }
    } catch (...) {
        // nop
    }
}

void PredictionOracle::AddInstruction(const void* instruction)
{
    for ( PredictorMap::iterator it = mPredictors.begin(); it != mPredictors.end(); it++ ) {
        it->second->AddInstruction(instruction);
    }
}

void PredictionOracle::Hit(const bool branched)
{
    for ( PredictorMap::iterator it = mPredictors.begin(); it != mPredictors.end(); it++ ) {
        it->second->Hit(branched);
    }
}
