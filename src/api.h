#ifndef API_H_
#define API_H_

#include <iostream>
#include <optional>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>

#include <espeak-ng/speak_lib.h>
#include "phoneme_ids.hpp"
#include "phonemize.hpp"

extern "C" {

    typedef struct Sentence {
        piper::PhonemeId* phonemesIds;
        size_t phonemesIdsLength;
    };

    typedef struct Results {
        Sentence* sentences;
        size_t sentencesCount;
    };

    PIPERPHONEMIZE_EXPORT int text_to_espeak_phonemes_ids(const char* text, const char* voice, const char* dataPath);

    PIPERPHONEMIZE_EXPORT Results get_results();
}

#endif // API_H_