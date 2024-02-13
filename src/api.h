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

    struct PiperSentencePhonemes {
        piper::PhonemeId* phonemesIds;
        size_t phonemesIdsLength;
    };

    struct PiperTextPhonemes {
        PiperSentencePhonemes* sentences;
        size_t sentencesCount;
    };

    PIPERPHONEMIZE_EXPORT int preprocess_text(const char* text, const char* voice, const char* dataPath);

    PIPERPHONEMIZE_EXPORT PiperTextPhonemes get_preprocessed_text();

    PIPERPHONEMIZE_EXPORT void free_piper();
}

#endif // API_H_