#ifndef _BRANCHPREDICTOR_H_
#define _BRANCHPREDICTOR_H_

#include "prediction/Instruction.h"

class BranchPredictor
{
public:
    BranchPredictor(void);
    virtual ~BranchPredictor(void);

    virtual void AddInstruction(const Instruction* instruction)  = 0;
    virtual bool WouldBranch(void) const = 0;

    void Hit(const bool branched);

    void ExportResults(const std::string& filename) const;
    void AppendResults(std::ostream& output) const;

protected:
    virtual void Update(const bool branched) = 0;
    void IncreaseBranchCount(void);
    void IncreaseHitCount(void);

private:
    unsigned int    mHitCount;
    unsigned int    mBranchCount; 
};

#endif //_BRANCHPREDICTOR_H_
