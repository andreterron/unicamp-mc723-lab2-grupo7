#ifndef _ONEBITPREDICTOR_H_
#define _ONEBITPREDICTOR_H_

#include "prediction/BranchPredictor.h"

class OneBitPredictor
    :   public BranchPredictor
{
private:
    enum PredictionState {
        NOT_TAKEN  = 0,
        TAKEN      = 1
    };

public:
    OneBitPredictor(void) {}
    virtual ~OneBitPredictor(void) {}

    virtual void AddInstruction(const Instruction* instruction) { return; }
    virtual bool WouldBranch(void) const;
    virtual void Update(const bool branched);

private:
    PredictionState     mPrediction;
};

#endif //_ONEBITPREDICTOR_H_
