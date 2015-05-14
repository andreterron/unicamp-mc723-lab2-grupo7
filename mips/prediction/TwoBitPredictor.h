#ifndef _TWOBITPREDICTOR_H_
#define _TWOBITPREDICTOR_H_

#include "prediction/BranchPredictor.h"

class TwoBitPredictor
    :   public BranchPredictor
{
private:
    enum PredictionState {
        STRONGLY_NOT_TAKEN  = 0,
        WEAKLY_NOT_TAKEN    = 1,
        WEAKLY_TAKEN        = 2,
        STRONGLY_TAKEN      = 3
    };

public:
    TwoBitPredictor(void) {}
    virtual ~TwoBitPredictor(void) {}

    virtual void AddInstruction(const Instruction* instruction) { return; }
    virtual bool WouldBranch(void) const;
    virtual void Update(const bool branched);

private:
    unsigned int        mState;
};

#endif //_TWOBITPREDICTOR_H_
