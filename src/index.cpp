#include "index.h"
#include "converter_json.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <stdexcept>

float CalculateRelevance(const std::map<std::string, std::vector<Entry>>& index, const std::vector<std::string>& query_words, size_t doc_id) {
    float relevance_score = 0.0;

    for (const auto& word : query_words) {
        auto it = index.find(word);
        if (it != index.end()) {
            for (const auto& entry : it->second) {
                if (entry.doc_id == doc_id) {
                    relevance_score += entry.count;
                    break;
                }
            }
        }
    }

    std::cout << "DocID: " << doc_id << " Relevance: " << relevance_score << std::endl;

    return relevance_score;
}






