#ifndef _PREDICTIONORACLE_H_
#define _PREDICTIONORACLE_H_

#include <string>

#include "prediction/Instruction.h"

class BranchPredictor;

class PredictionOracle {
public:
    PredictionOracle(void);
    virtual ~PredictionOracle(void);

    bool AddPredictor(const std::string& id, BranchPredictor* predictor);
    bool RemovePredictor(const std::string& id);

    void AddInstruction(const Instruction* instruction);
    void Hit(const bool branched);
    
    void ExportAll(const std::string& filename) const;
    void ExportEach(const std::string& path) const;

private:
    unsigned int        mBufferSize;
    unsigned int        mNumPredictors;
    std::string*        mPredictorIds;
    BranchPredictor**   mPredictors;
};

#endif //_PREDICTIONORACLE_H_
