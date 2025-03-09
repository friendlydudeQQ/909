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


void ProcessSearch() {
    try {
        ConverterJSON converter;

        std::vector<std::string> files = converter.GetTextDocuments();

        std::vector<std::string> queries = converter.GetRequests();

        int max_responses = converter.GetResponsesLimit();

        InvertedIndex invertedIndex;
        invertedIndex.UpdateDocumentBase(files);

        const auto& index = invertedIndex.GetIndex();

        std::vector<std::vector<std::pair<int, float>>> answers;

        for (const auto& query : queries) {
            std::vector<std::pair<int, float>> relevance_scores;

            std::vector<std::string> query_words;
            std::istringstream query_stream(query);
            std::string word;
            while (query_stream >> word) {
                query_words.push_back(word);
            }

            for (size_t i = 0; i < files.size(); ++i) {
                float relevance = CalculateRelevance(index, query_words, i);

                relevance_scores.push_back({static_cast<int>(i + 1), relevance});
            }

            std::sort(relevance_scores.begin(), relevance_scores.end(), [](const auto& a, const auto& b) {
                return a.second > b.second;
            });

            if (relevance_scores.size() > max_responses) {
                relevance_scores.resize(max_responses);
            }

            answers.push_back(relevance_scores);
        }

        converter.putAnswers(answers);

        std::cout << "Processing completed and answers saved." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}



