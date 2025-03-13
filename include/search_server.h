#ifndef SEARCH_SERVER_H
#define SEARCH_SERVER_H

#include "index.h"
#include <vector>
#include <string>
#include <cmath>
#include <unordered_set>

struct RelativeIndex {
    size_t doc_id;
    float relevance;

    bool operator==(const RelativeIndex& other) const {
        const float tolerance = 1e-6;
        return (doc_id == other.doc_id && std::fabs(relevance - other.relevance) < tolerance);
    }
};

class SearchServer {
public:
    explicit SearchServer(const InvertedIndex& index);

    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries) const;

private:
    const InvertedIndex& idx;
    std::vector<std::string> splitIntoWords(const std::string& query) const;
    std::unordered_set<std::string> stopwords = {"is", "the", "of", "to"};
};
#endif // SEARCH_SERVER_H

