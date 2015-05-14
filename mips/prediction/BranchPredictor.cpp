#include "prediction/BranchPredictor.h"

BranchPredictor::BranchPredictor(void)
    :   mHitCount(0)
    ,   mBranchCount(0)
{}
BranchPredictor::~BranchPredictor(void)
{}

void BranchPredictor::Hit(const bool branched)
{
    IncreaseBranchCount();
    if ( branched && WouldBranch() ) {
        IncreaseHitCount();
    }
    Update(branched);
}

void BranchPredictor::ExportResults(const std::string& filename) const
{
    std::fstream file(filename);

    AppendResults(file);
}

void BranchPredictor::AppendResults(std::ostream& output) const
{
    output  << "Branch count: "             << mBranchCount << std::endl
            << "Successful predictions: "   << mHitCount    << std::endl;
}

void BranchPredictor::IncreaseBranchCount(void)
{
    mBranchCount++;
}

void BranchPredictor::IncreaseHitCount(void)
{
    mHitCount++;
}
