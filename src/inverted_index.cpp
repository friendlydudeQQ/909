#include "index.h"
#include <sstream>
#include <thread>
#include <iostream>
#include <map>

InvertedIndex::InvertedIndex() = default;

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = input_docs;
    freq_dictionary.clear();

    std::vector<std::thread> threads;
    for (size_t i = 0; i < docs.size(); ++i) {
        threads.push_back(std::thread(&InvertedIndex::IndexDocument, this, i));
    }

    for (auto& th : threads) {
        th.join();
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
        freq_dictionary[word].push_back({doc_id, count});
        std::cout << "Indexed word: " << word << " for DocID: " << doc_id << " with count: " << count << std::endl;
    }
}


const std::map<std::string, std::vector<Entry>>& InvertedIndex::GetIndex() const {
    return freq_dictionary;
}



