#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include <thread>

struct Entry {
    size_t doc_id, count;
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};

class InvertedIndex {
public:
    InvertedIndex();

    void UpdateDocumentBase(const std::vector<std::string>& input_docs);

    const std::map<std::string, std::vector<Entry>>& GetIndex() const;

private:
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;

    void IndexDocument(size_t doc_id);
};

#endif // INVERTED_INDEX_H


