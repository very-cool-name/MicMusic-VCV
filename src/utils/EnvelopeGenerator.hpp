#include <cstdint>

enum class EnvelopeStates {
    OFF,
    ATTACK,
    DECAY,
    SUSTAIN,
    RELEASE
};

class EnvelopeGenerator {
public:
    using Time = float;
    using Level = float;

    EnvelopeGenerator(Time attack, Time decay, Level sustain, Time release);

    void nextState(Level in);
    float nextSample(Time delta);
    
    Time& attack() { return attack_; };
    Time& decay() { return decay_; };
    Level& sustain() { return sustain_; };
    Time& release() { return release_; };

private:
    enum class InState : uint8_t {
        FLAT,
        ACTIVATED,
        DEACTIVATED
    };

    InState checkInState(Level in);
    float calcSample(Time delta);

    Time attack_;
    Time decay_;
    Level sustain_;
    Time release_;
    EnvelopeStates state_;
    Time timeSinceAttack_;
    Time timeSinceRelease_;
    bool inState_;
};
