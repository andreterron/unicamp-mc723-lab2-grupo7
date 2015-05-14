#ifndef _STATICPREDICTOR_H_
#define _STATICPREDICTOR_H_

#include "prediction/BranchPredictor.h"

template<bool _Prediction>
class StaticPredictor
    :   public BranchPredictor
{
public: 
    StaticPredictor(void) {}
    virtual ~StaticPredictor(void) {}

    virtual void AddInstruction(const Instruction* instruction) { return; }
    virtual bool WouldBranch(void) const { return _Prediction; }
    virtual void Update(const bool branched) { return; }    
};

typedef StaticPredictor<false> StaticPredictorBranch;
typedef StaticPredictor<true>  StaticPredictorNoBranch;

#endif //_STATICPREDICTOR_H_
