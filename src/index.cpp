#include "index.h"
#include "converter_json.h"
#include <vector>
#include <algorithm>
#include <stdexcept>

float CalculateRelevance(const std::map<std::string, std::vector<Entry>>& index,
                         const std::vector<std::string>& query_words,
                         size_t doc_id) {
    float relevance = 0.0f;
    for (const auto& word : query_words) {
        auto wordIt = index.find(word);
        if (wordIt == index.end()) continue;

        for (const auto& entry : wordIt->second) {
            if (entry.doc_id == doc_id) {
                relevance += entry.count;
                break;
            }
        }
    }
    return relevance;
}










