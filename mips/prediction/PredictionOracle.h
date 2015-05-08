#ifndef _PREDICTIONORACLE_H_
#define _PREDICTIONORACLE_H_

#include <string>
#include <unordered_map>

class BranchPredictor;

typedef std::unordered_map<std::string, BranchPredictor*> PredictorMap;

class PredictionOracle {
public:
    PredictionOracle(void);
    virtual ~PredictionOracle(void);

    bool AddPredictor(const std::string& id, BranchPredictor* predictor);
    bool RemovePredictor(const std::string& id);

    void AddInstruction(const void* instruction);
    void Hit(const bool branched);

private:
    PredictorMap        mPredictors;
};

#endif //_PREDICTIONORACLE_H_
