#ifndef INDEX_H
#define INDEX_H

#include <map>
#include <vector>
#include <string>

struct Entry {
    size_t doc_id, count;
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};

class InvertedIndex {
public:
    InvertedIndex();

    void UpdateDocumentBase(std::vector<std::string> input_docs);

    const std::map<std::string, std::vector<Entry>>& GetIndex() const;

private:
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;

    void IndexDocument(size_t doc_id);
};

float CalculateRelevance(const std::map<std::string, std::vector<Entry>>& index,
                         const std::vector<std::string>& query_words,
                         size_t doc_id);

#endif // INDEX_H




