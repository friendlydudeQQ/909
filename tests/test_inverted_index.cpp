#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include "index.h"

struct SearchResult {
    int doc_id;
    float rank;

    bool operator==(const SearchResult& other) const {
        return doc_id == other.doc_id && rank == other.rank;
    }
};

bool float_equal(float a, float b, float epsilon = 1e-6) {
    return std::fabs(a - b) < epsilon;
}

TEST(TestCaseSearchServer, TestSimple) {
    const std::vector<std::string> docs = {
            "apple car mountain music cloud",
            "music apple tree river",
            "mountain forest cloud apple music"
    };
    const std::vector<std::string> request = {"apple music", "forest river"};
    std::vector<std::vector<SearchResult>> expected = {
            {
                    {2, 1},
                    {0, 0.7},
                    {1, 0.3}
            },
            {
                    {2, 1},
                    {1, 1}
            }
    };

    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);

    std::vector<std::vector<SearchResult>> result;
    const auto& index = idx.GetIndex();

    for (const auto& query : request) {
        if (index.find(query) != index.end()) {
            std::vector<SearchResult> search_results;
            for (const auto& entry : index.at(query)) {
                search_results.emplace_back(static_cast<int>(entry.doc_id), static_cast<float>(entry.count));
            }
            result.emplace_back(search_results);
        } else {
            result.emplace_back();
        }
    }

    for (auto& res : result) {
        std::sort(res.begin(), res.end(), [](const SearchResult& a, const SearchResult& b) {
            return a.rank > b.rank;
        });
    }

    for (auto& res : expected) {
        std::sort(res.begin(), res.end(), [](SearchResult& a, SearchResult& b) {
            return a.rank > b.rank;
        });
    }

    ASSERT_EQ(result.size(), expected.size());
    for (size_t i = 0; i < result.size(); ++i) {
        ASSERT_EQ(result[i].size(), expected[i].size());
        for (size_t j = 0; j < result[i].size(); ++j) {
            ASSERT_EQ(result[i][j].doc_id, expected[i][j].doc_id);
            ASSERT_TRUE(float_equal(result[i][j].rank, expected[i][j].rank));
        }
    }
}

TEST(TestCaseSearchServer, TestTop5) {
    const std::vector<std::string> docs = {
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisboa is the capital of portugal",
            "bern is the capital of switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway",
            "stockholm is the capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital of estonia",
            "warsaw is the capital of poland"
    };
    const std::vector<std::string> request = {"moscow is the capital of russia"};
    std::vector<std::vector<SearchResult>> expected = {
            {
                    {7, 1},
                    {14, 1},
                    {0, 0.666666687},
                    {1, 0.666666687},
                    {2, 0.666666687}
            }
    };

    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);

    std::vector<std::vector<SearchResult>> result;
    const auto& index = idx.GetIndex();

    for (const auto& query : request) {
        if (index.find(query) != index.end()) {
            std::vector<SearchResult> search_results;
            for (const auto& entry : index.at(query)) {
                search_results.push_back({static_cast<int>(entry.doc_id), static_cast<float>(entry.count)});

            }
            result.emplace_back(search_results);
        } else {
            result.emplace_back();
        }
    }

    for (auto& res : result) {
        std::sort(res.begin(), res.end(), [](const SearchResult& a, const SearchResult& b) {
            return a.rank > b.rank;
        });
    }

    for (auto& res : expected) {
        std::sort(res.begin(), res.end(), [](SearchResult& a, SearchResult& b) {
            return a.rank > b.rank;
        });
    }
    ASSERT_EQ(result.size(), expected.size());
    for (size_t i = 0; i < result.size(); ++i) {
        ASSERT_EQ(result[i].size(), expected[i].size());
        for (size_t j = 0; j < result[i].size(); ++j) {
            ASSERT_EQ(result[i][j].doc_id, expected[i][j].doc_id);
            ASSERT_TRUE(float_equal(result[i][j].rank, expected[i][j].rank));
        }
    }
}

TEST(TestCaseSearchServer, TestEmptyQuery) {
    const std::vector<std::string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
    };
    const std::vector<std::string> request = {"sugar"};
    const std::vector<std::vector<SearchResult>> expected = { };

    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);

    std::vector<std::vector<SearchResult>> result;
    const auto& index = idx.GetIndex();

    for (const auto& query : request) {
        if (index.find(query) != index.end()) {
            std::vector<SearchResult> search_results;
            for (const auto& entry : index.at(query)) {
                search_results.push_back({static_cast<int>(entry.doc_id), static_cast<float>(entry.count)});
            }
            result.emplace_back(search_results);
        } else {
            result.emplace_back();
        }
    }

    ASSERT_EQ(result, expected);
}

