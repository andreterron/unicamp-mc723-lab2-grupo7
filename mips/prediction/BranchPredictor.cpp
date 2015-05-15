#include "prediction/BranchPredictor.h"

#include <iomanip>

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
    output  << "BC: "   << std::setw(10) << mBranchCount << std::endl
            << "SP: "   << std::setw(10) << mHitCount    
            << " ("     << std::setfill(' ') 
                        << std::setw(6) 
                        << std::setprecision(2)
                        << 100.0f * mHitCount / (1.0f * mBranchCount)
            << "%)"     << std::endl;
}

void BranchPredictor::IncreaseBranchCount(void)
{
    mBranchCount++;
}

void BranchPredictor::IncreaseHitCount(void)
{
    mHitCount++;
}
