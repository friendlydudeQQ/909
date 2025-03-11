#include <iostream>
#include <vector>
#include <string>
#include "index.h"
#include "converter_json.h"

int main() {
    try {
        std::vector<std::string> files = ConverterJSON::GetTextDocuments();
        InvertedIndex index;
        index.UpdateDocumentBase(files);

        std::vector<std::string> queries = ConverterJSON::GetRequests();
        int max_responses = ConverterJSON::GetResponsesLimit();

        std::vector<std::vector<std::pair<int, float>>> answers;

        for (const auto& query : queries) {
            std::vector<std::pair<int, float>> relevance_scores;

            for (size_t doc_id = 0; doc_id < files.size(); ++doc_id) {
                float relevance = CalculateRelevance(index.GetIndex(), {query}, doc_id);
                relevance_scores.emplace_back(static_cast<int>(doc_id + 1), relevance);
            }

            std::sort(relevance_scores.begin(), relevance_scores.end(), [](const auto& a, const auto& b) {
                return a.second > b.second;
            });

            if (relevance_scores.size() > max_responses) {
                relevance_scores.resize(max_responses);
            }

            answers.emplace_back(relevance_scores);
        }

        ConverterJSON::putAnswers(answers);

        std::cout << "Processing completed and answers saved." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}



