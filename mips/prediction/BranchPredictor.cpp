#include "prediction/BranchPredictor.h"

void BranchPredictor::Hit(const bool branched)
{
    IncreaseBranchCount();
    if ( branched && WouldBranch() ) {
        IncreaseHitCount();
    }
    Update(branched);
}

void BranchPredictor::IncreaseBranchCount(void)
{
    mBranchCount++;
}

void BranchPredictor::IncreaseHitCount(void)
{
    mHitCount++;
}
