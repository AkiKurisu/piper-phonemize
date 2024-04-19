#include <fstream>
#include <iostream>
#include "api.h"

static bool isPiperInit = false;
static PiperProcessedText* result = nullptr;

void free_results();

int init_piper(const char* dataPath)
{
    int code = espeak_Initialize(AUDIO_OUTPUT_SYNCHRONOUS, 0, dataPath, 0);
    if (code < 0)
        return code;

    isPiperInit = true;
    return 0;
}

int process_text(const char *text, const char *voice)
{
    if (!isPiperInit)
        return -1;

    // reset results
    free_results();
    try{
        result = new PiperProcessedText();

        piper::eSpeakPhonemeConfig config;
        config.voice = voice;

        std::vector<std::vector<piper::Phoneme>> phonemesBySentence;
        piper::phonemize_eSpeak(text, config, phonemesBySentence);

        // save results
        size_t sentencesCount = phonemesBySentence.size();
        result->sentencesCount = sentencesCount;
        result->sentences = new PiperProcessedSentence[sentencesCount];

        piper::PhonemeIdConfig idConfig;
        for (size_t i = 0; i < sentencesCount; i++)
        {
            std::vector<piper::Phoneme> phonemesSentence = phonemesBySentence[i];
            std::vector<piper::PhonemeId> phonemeIds;
            std::map<piper::Phoneme, std::size_t> missingPhonemes;

            phonemes_to_ids(phonemesSentence, idConfig, phonemeIds, missingPhonemes);
            
            // copy results
            size_t phonemesIdsLength = phonemeIds.size();
            PiperProcessedSentence& resultSentence = result->sentences[i];

            resultSentence.phonemesIdsLength = phonemesIdsLength;
            resultSentence.phonemesIds = new piper::PhonemeId[phonemesIdsLength];
            std::copy(phonemeIds.begin(), phonemeIds.end(), resultSentence.phonemesIds);
        }

        return 0;
    }
    catch(...)
    {
        return -1;
    }
}

PiperProcessedText get_processed_text()
{
    return *result;
}

void free_results()
{
    if (result == nullptr)
        return;

    for (int i = 0; i < result->sentencesCount; i++)
    {
        delete[] result->sentences[i].phonemesIds;
    }
    delete[] result->sentences;
    delete result;
    result = nullptr;
}

void free_piper()
{
    espeak_Terminate();
    free_results();
    isPiperInit = false;
}