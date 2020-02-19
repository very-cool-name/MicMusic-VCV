#include "EnvelopeGenerator.hpp"

#include "math.hpp"

#include <exception>

EnvelopeGenerator::EnvelopeGenerator(Time attack, Time decay, Level sustain, Time release)
: attack_(attack)
, decay_(decay)
, sustain_(sustain)
, release_(release)
, state_(EnvelopeStates::OFF)
, timeSinceAttack_(0)
, timeSinceRelease_(0)
, inState_(false) {
}

void EnvelopeGenerator::nextState(Level in) {
    auto inState = checkInState(in);
    if (inState == InState::ACTIVATED) {
        timeSinceAttack_ = 0;
        state_ = EnvelopeStates::ATTACK;
    } else if (inState == InState::DEACTIVATED) {
        timeSinceRelease_ = 0;
        state_ = EnvelopeStates::RELEASE;
    } else if (state_ == EnvelopeStates::ATTACK && timeSinceAttack_ > attack_) {
        state_ = EnvelopeStates::DECAY;
    } else if (state_ == EnvelopeStates::DECAY && timeSinceAttack_ > attack_ + decay_) {
        state_ = EnvelopeStates::SUSTAIN;
    }
}

float EnvelopeGenerator::nextSample(Time delta) {
    return rack::math::clamp(calcSample(delta), 0.f, 1.f);
}

float EnvelopeGenerator::calcSample(Time delta) {
    if (state_ == EnvelopeStates::OFF) {
        return 0;
    } else if (state_ == EnvelopeStates::ATTACK) {
        timeSinceAttack_ += delta;
        if (attack_ > 1e-6) {
            return timeSinceAttack_ / attack_;
        } else {
            return 1;
        }
    } else if (state_ == EnvelopeStates::DECAY) {
        timeSinceAttack_ += delta;
        if (decay_ > 1e-6) {
            return 1 - (timeSinceAttack_ - attack_) / decay_ * (1 - sustain_);
        } else {
            return sustain_;
        }
    } else if (state_ == EnvelopeStates::SUSTAIN) {
        return sustain_;
    } else if (state_ == EnvelopeStates::RELEASE) {
        timeSinceRelease_ += delta;
        if (release_ > 1e-6) {
            return (1 - timeSinceRelease_ / release_) * sustain_;
        } else {
            return 0;
        }
    } else {
        throw std::runtime_error("EnvelopeGenerator is in invalid state");
    }
}


EnvelopeGenerator::InState EnvelopeGenerator::checkInState(Level in) {
    if (!inState_ && in >= 1.f) {
        inState_ = true;
        return InState::ACTIVATED;
    } else if (inState_ && in <= 0.f) {
        inState_ = false;
        return InState::DEACTIVATED;
    } else {
        return InState::FLAT;
    }
}