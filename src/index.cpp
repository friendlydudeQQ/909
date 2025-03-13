#include "index.h"
#include "converter_json.h"
#include <vector>
#include <algorithm>
#include <stdexcept>

float CalculateRelevance(const std::map<std::string, std::vector<Entry>>& index,
                         const std::vector<std::string>& query_words,
                         size_t doc_id) {
    float totalRelevance = 0.0f;
    size_t validWords = 0;

    for (const auto& word : query_words) {
        auto wordIt = index.find(word);
        if (wordIt == index.end()) continue;

        size_t maxCount = 0;
        for (const auto& entry : wordIt->second) {
            if (entry.count > maxCount) maxCount = entry.count;
        }
        if (maxCount == 0) continue;

        size_t docCount = 0;
        for (const auto& entry : wordIt->second) {
            if (entry.doc_id == doc_id) {
                docCount = entry.count;
                break;
            }
        }

        totalRelevance += static_cast<float>(docCount) / maxCount;
        validWords++;
    }

    if (validWords == 0) return 0.0f;
    return totalRelevance / validWords;
}










