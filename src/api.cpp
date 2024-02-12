#include "api.h"

struct PhonemizeStateSentences
{
    std::vector<piper::Phoneme> phonemes;
    std::vector<piper::PhonemeId> phonemeIds;
    std::map<piper::Phoneme, std::size_t> missingPhonemes;
};

struct PhonemizeState
{
    std::vector<PhonemizeStateSentences> sentences;
} state;

bool eSpeakInitialized = false;

void reset_state()
{
    state.sentences.clear();
}

int text_to_espeak_phonemes_ids(const char *text, const char *voice, const char *dataPath)
{
    reset_state();
    
    if (!eSpeakInitialized)
    {
        int result =
            espeak_Initialize(AUDIO_OUTPUT_SYNCHRONOUS, 0, dataPath, 0);
        if (result < 0)
        {
            return result;
        }

        eSpeakInitialized = true;
    }

    piper::eSpeakPhonemeConfig config;
    config.voice = voice;

    std::vector<std::vector<piper::Phoneme>> phonemesBySentence;
    piper::phonemize_eSpeak(text, config, phonemesBySentence);

    piper::PhonemeIdConfig idConfig;
    for(std::vector<piper::Phoneme> phonemesSentence : phonemesBySentence)
    {
        std::vector<piper::PhonemeId> phonemeIds;
        std::map<piper::Phoneme, std::size_t> missingPhonemes;

        phonemes_to_ids(phonemesSentence, idConfig, phonemeIds, missingPhonemes);

        state.sentences.push_back({phonemesSentence, phonemeIds, missingPhonemes});
    }

    return 0;
}

Results get_results()
{
    Results results;
    results.sentencesCount = state.sentences.size();

    results.sentences = new Sentence[results.sentencesCount];
    for (int i = 0; i < results.sentencesCount; i++)
    {
        PhonemizeStateSentences stateSentence = state.sentences[i];
        results.sentences[i].phonemesIdsLength = stateSentence.phonemeIds.size();
        results.sentences[i].phonemesIds = stateSentence.phonemeIds.data();
    }

    return results;
}