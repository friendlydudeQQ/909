#ifndef CONVERTER_JSON_H
#define CONVERTER_JSON_H

#include <nlohmann/json.hpp>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class ConverterJSON {
public:
    static nlohmann::json ReadJsonFile(const fs::path& file_path);
    static void WriteJsonFile(const fs::path& file_path, const nlohmann::json& json_data);

    static std::vector<std::string> GetTextDocuments();
    static int GetResponsesLimit();
    static std::vector<std::string> GetRequests();
    static void putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers);

private:
    static nlohmann::json config_json;
    static nlohmann::json requests_json;
    static nlohmann::json answers_json;
};

#endif // CONVERTER_JSON_H

