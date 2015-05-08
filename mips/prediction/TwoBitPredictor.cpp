#include "prediction/TwoBitPredictor.h"

bool TwoBitPredictor::WouldBranch(void) const {
    return ( mState >= (unsigned)TwoBitPredictor::WEAKLY_TAKEN );
}

void TwoBitPredictor::Update(const bool branched) {
    if (branched && ( mState < (unsigned)TwoBitPredictor::STRONGLY_TAKEN ) ) {
        mState++;
    } else if (!branched && ( mState > (unsigned)TwoBitPredictor::STRONGLY_NOT_TAKEN ) ) {
        mState--;
    }
}
