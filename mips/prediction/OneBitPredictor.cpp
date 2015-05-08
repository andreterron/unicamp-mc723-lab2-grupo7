#include "prediction/OneBitPredictor.h"

bool OneBitPredictor::WouldBranch(void) const {
    return ( mPrediction == TAKEN );
}

void OneBitPredictor::Update(const bool branched) {
    if (branched) {
        mPrediction = TAKEN;
    } else {
        mPrediction = NOT_TAKEN;
    }
}
