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

    struct PiperProcessedSentence {
        piper::PhonemeId* phonemesIds;
        size_t phonemesIdsLength;
    };

    struct PiperProcessedText {
        PiperProcessedSentence* sentences;
        size_t sentencesCount;
    };

    PIPERPHONEMIZE_EXPORT int init_piper(const char* dataPath);

    PIPERPHONEMIZE_EXPORT int process_text(const char* text, const char* voice);

    PIPERPHONEMIZE_EXPORT PiperProcessedText get_processed_text();

    PIPERPHONEMIZE_EXPORT void free_piper();
}

#endif // API_H_