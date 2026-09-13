#include <cmath>
#include <cstdlib>
#include <map>
#include <vector>

#define MIN_SENSE_THRESHOLD 5
#define MIN_ACTIVATION_THRESHOLD 5

struct PhysicalBodyPart {

    bool exists() {}
};

struct Pattern {
    int x, y, z;

    Pattern() : x(0), y(0), z(0) {}
    Pattern(int x, int y, int z) : x(x), y(y), z(z) {}

    Pattern operator+(Pattern& other) {
        return Pattern(x+other.x, y+other.y, z+other.z);
    }

    Pattern operator-(Pattern& other) {
        return Pattern(x-other.x, y-other.y, z-other.z);
    }

    Pattern operator*(int scalar) {
        return Pattern(x*scalar, y*scalar, z*scalar);
    }

    int norm() {
        return std::sqrt(std::pow(x,2) + std::pow(y,2) + std::pow(z,2));
    }

    bool isTouch() {

        return x == 1;
    }

    bool isVision() {

        return x != 1;
    }

    PhysicalBodyPart physical;

};

struct SensoryPattern : Pattern {
};

struct ActivationPattern : Pattern {
};

struct ReactionPattern : Pattern {
};

struct TouchStimuli {
    SensoryPattern pattern;
};

struct VisualStimuli {
    SensoryPattern pattern;

};

std::map<TouchStimuli, PhysicalBodyPart*> bodyPartMap;

struct ProcessingNodeListener {

    void processReaction(int reactionLevel) {

    }

};

struct ProcessingNode {
    // is there just one target pattern and a threshold or is it a range?
    SensoryPattern targetPattern;

    int patternsProcessed;

    std::vector<ProcessingNode*> neighbors;

    int activationThreshold;

    ProcessingNodeListener *listener;

    void updateWeights(SensoryPattern& pattern) {
        // inversely update weights based on how many patterns we've seen.
        patternsProcessed++;
        double weight = 1.0 / patternsProcessed;
        targetPattern.x = targetPattern.x * (1 - weight) + pattern.x * weight;
        targetPattern.y = targetPattern.y * (1 - weight) + pattern.y * weight;
        targetPattern.z = targetPattern.z * (1 - weight) + pattern.z * weight;
    }

    bool canReactToThisFeeling(ActivationPattern& activation) {

        return activation.norm() > activationThreshold;
    }

    ActivationPattern react(ActivationPattern& activation) {
        
        // send upstream to processing unit for decision making. it will be monitoring activation levels and determining correct
        // action to take
        double weight = 1.0;
        int reactionLevel = activation.norm() * weight;
        listener->processReaction(reactionLevel);

        Pattern scaled = static_cast<Pattern>(activation) * reactionLevel;
        ActivationPattern result;
        result.x = scaled.x;
        result.y = scaled.y;
        result.z = scaled.z;
        return result;
    }

    void triggerFeeling(ActivationPattern activation) {
        // multiple feelings can occur at once, there must be a processor on top of them to aggregate/decipher meaning out of them

        if (canReactToThisFeeling(activation)) {
            activation = react(activation);
        }

        // forward to neighbors
        for (auto& n : neighbors) {
            n->triggerFeeling(activation);
        }


    }
};

struct Region {

    std::vector<ProcessingNode*> nodes;

    void process(SensoryPattern& sensory) {
        // convert to activation pattern (might not be necessary)
        ActivationPattern activation;
        activation.x = sensory.x;
        activation.y = sensory.y;
        activation.z = sensory.z;

        // todo traverse nodes instead of iterating
        for (auto n : nodes) {
            if ((n->targetPattern - sensory).norm() <= MIN_SENSE_THRESHOLD) {
                n->triggerFeeling(activation);
                n->updateWeights(sensory);
            }
        }
    }
};

struct TouchRegion : Region {};
struct VisionRegion : Region {};
struct DefaultRegion : Region {};

Region getBrainRegion(SensoryPattern& pattern) {

    if (pattern.isTouch()) {
        return TouchRegion();
    }
    if (pattern.isVision()) {
        return VisionRegion();
    }
    return DefaultRegion();
}

void onReceiveSensoryStimuli(TouchStimuli *ts) {
    SensoryPattern pattern = ts->pattern;

    // send to correct brain region
    Region region = getBrainRegion(pattern);

    region.process(pattern);
}

Pattern transformPattern(Pattern pattern) {
    // basically simulating traversing brain connections and meaning/concept shifting
    return Pattern(pattern.x + std::rand(), pattern.y + std::rand(), pattern.z + std::rand());
}

void processVisualPattern(SensoryPattern pattern, SensoryPattern target) {
    // basically go through long cognitive association chain that results in linkage
    // between body part you are looking at and pattern for that part.

    // todo convert to Intermediate Representation
    if ((pattern - target).norm() <= MIN_SENSE_THRESHOLD) {
        // found it
        if (target.physical.exists()) {
            //target.physical.pattern = pattern; // naive pattern conflation

        }

    } else {
        // transform pattern further
        transformPattern(pattern);
    }
}

// ------------- Tests -----------

void test_PhantomCheekPain() {

}

void test_Synthesia() {

}