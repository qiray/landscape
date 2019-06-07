#include <cstring>
#include <cstdlib>
#include "landscape.h"

//TODO: readme

#define PARSE_INT(desc, func) \
    if (strcmp(argv[i], desc) == 0) { \
        func(atoi(argv[++i])); \
        continue; \
    }

#define PARSE_FLOAT(desc, func) \
    if (strcmp(argv[i], desc) == 0) { \
        func(static_cast<float>(atof(argv[++i]))); \
        continue; \
    }

#define PARSE_STRING(desc, func) \
    if (strcmp(argv[i], desc) == 0) { \
        func(argv[++i]); \
        continue; \
    }

int main(int argc, char **argv) {
    LandscapeAlgorithm alg;
    for (int i = 0; i < argc; i++) {
        PARSE_STRING("--algorithm", alg.setType);
        PARSE_STRING("--output", alg.setOutFileName);
        PARSE_INT("--size", alg.setMapSize);
        PARSE_INT("--islands", alg.setNumberOfIslands);
        PARSE_INT("--islandSize", alg.setIslandSize);
        PARSE_INT("--seed", alg.setRandomSeed);
        PARSE_INT("--height", alg.setStartHeight);
        PARSE_INT("--gens", alg.setGenerations);
        PARSE_INT("--rivers_number", alg.setRiversCount);
        PARSE_FLOAT("--roughness", alg.setRoughness);
        PARSE_FLOAT("--out_height", alg.setOutHeight);
        PARSE_FLOAT("--amplitude", alg.setAmplitude);
        PARSE_FLOAT("--persistence", alg.setPersistence);
        PARSE_FLOAT("--frequency", alg.setFrequency);
        if (strcmp(argv[i], "--noise") == 0) {
            alg.setHillNoise(1);
            continue;
        }
    }
    alg.generateLandscape();
    alg.printLandscape();
    return 0;
}
