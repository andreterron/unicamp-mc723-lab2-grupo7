#ifndef _BRANCHPREDICTOR_H_
#define _BRANCHPREDICTOR_H_

class BranchPredictor
{
public:
    BranchPredictor(void) {}
    virtual ~BranchPredictor(void) {}

    virtual void AddInstruction(const void* instruction)  = 0;
    virtual bool WouldBranch(void) const = 0;

    void Hit(const bool branched);

protected:
    virtual void Update(const bool branched) = 0;
    void IncreaseBranchCount(void);
    void IncreaseHitCount(void);

private:
    unsigned int    mHitCount;
    unsigned int    mBranchCount; 
};

#endif //_BRANCHPREDICTOR_H_
