#include "index.h"
#include <sstream>
#include <thread>
#include <iostream>
#include <algorithm>

InvertedIndex::InvertedIndex() = default;

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = input_docs;
    freq_dictionary.clear();

    std::vector<std::thread> threads;
    for (size_t i = 0; i < docs.size(); ++i) {
        threads.emplace_back(&InvertedIndex::IndexDocument, this, i);
    }

    for (auto& th : threads) {
        if (th.joinable()) {
            th.join();
        }
    }

    for (auto& [word, entries] : freq_dictionary) {
        std::sort(entries.begin(), entries.end(),
                  [](const Entry& a, const Entry& b) { return a.doc_id < b.doc_id; });
    }
}

void InvertedIndex::IndexDocument(size_t doc_id) {
    std::istringstream stream(docs[doc_id]);
    std::string word;
    std::map<std::string, size_t> word_count;

    while (stream >> word) {
        word_count[word]++;
    }

    for (const auto& [word, count] : word_count) {
        std::lock_guard<std::mutex> lock(dict_mutex);
        freq_dictionary[word].emplace_back(doc_id, count);
        std::cout << "Indexed word: " << word << " for DocID: " << doc_id
                  << " with count: " << count << std::endl;
    }
}

const std::map<std::string, std::vector<Entry>>& InvertedIndex::GetIndex() const {
    return freq_dictionary;
}



