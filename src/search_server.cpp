#include "search_server.h"
#include <algorithm>
#include <sstream>
#include <unordered_map>

SearchServer::SearchServer(const InvertedIndex& index)
        : idx(index) {}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries) const {
    std::vector<std::vector<RelativeIndex>> result;
    const auto& index = idx.GetIndex();

    for (const auto& query : queries) {
        std::vector<std::string> words = splitIntoWords(query);

        words.erase(std::remove_if(words.begin(), words.end(), [&](const std::string& word) {
            return stopwords.count(word);
        }), words.end());

        if (words.empty()) {
            result.emplace_back();
            continue;
        }

        std::unordered_map<std::string, size_t> maxWordFreq;
        for (const auto& word : words) {
            auto it = index.find(word);
            if (it == index.end()) continue;

            size_t maxCount = 0;
            for (const auto& entry : it->second) {
                if (entry.count > maxCount) maxCount = entry.count;
            }
            maxWordFreq[word] = maxCount;
        }

        std::unordered_map<size_t, float> docRelevance;
        for (const auto& word : words) {
            if (!maxWordFreq.count(word) || maxWordFreq[word] == 0) continue;

            auto it = index.find(word);
            for (const auto& entry : it->second) {
                float rel = static_cast<float>(entry.count) / maxWordFreq[word];
                docRelevance[entry.doc_id] += rel;
            }
        }

        std::vector<RelativeIndex> relevanceVec;
        for (auto& [docId, rel] : docRelevance) {
            rel /= words.size();
            relevanceVec.push_back({docId, rel});
        }

        std::sort(relevanceVec.begin(), relevanceVec.end(),
                  [](const RelativeIndex& a, const RelativeIndex& b) {
                      if (a.relevance == b.relevance) return a.doc_id < b.doc_id;
                      return a.relevance > b.relevance;
                  });

        if (relevanceVec.size() > 5) relevanceVec.resize(5);

        result.push_back(relevanceVec);
    }

    return result;
}

std::vector<std::string> SearchServer::splitIntoWords(const std::string& query) const {
    std::vector<std::string> words;
    std::istringstream iss(query);
    std::string word;
    while (iss >> word) {
        words.push_back(word);
    }
    return words;
}


