#include "converter_json.h"
#include <stdexcept>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

nlohmann::json ConverterJSON::config_json;
nlohmann::json ConverterJSON::requests_json;
nlohmann::json ConverterJSON::answers_json;

nlohmann::json ConverterJSON::ReadJsonFile(const fs::path& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + file_path.string());
    }
    nlohmann::json json_data;
    file >> json_data;
    return json_data;
}

void ConverterJSON::WriteJsonFile(const fs::path& file_path, const nlohmann::json& json_data) {
    std::ofstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + file_path.string());
    }
    file << json_data.dump(4);
}

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    fs::path config_file = "../config/config.json";
    config_json = ReadJsonFile(config_file);

    if (config_json.find("files") == config_json.end()) {
        throw std::runtime_error("config file is empty");
    }

    std::vector<std::string> files;
    for (const auto& file : config_json["files"]) {
        fs::path file_path = file.get<std::string>();
        std::ifstream file_stream(file_path);
        if (file_stream.is_open()) {
            std::stringstream buffer;
            buffer << file_stream.rdbuf();
            files.emplace_back(buffer.str());
        } else {
            throw std::runtime_error("Failed to open file: " + file_path.string());
        }
    }
    return files;
}


int ConverterJSON::GetResponsesLimit() {
    fs::path config_file = "../config/config.json";
    config_json = ReadJsonFile(config_file);

    if (config_json.find("config") == config_json.end() || config_json["config"].find("max_responses") == config_json["config"].end()) {
        throw std::runtime_error("max_responses not found in config.json");
    }

    return config_json["config"]["max_responses"];
}

std::vector<std::string> ConverterJSON::GetRequests() {
    fs::path requests_file = "../config/request.json";
    requests_json = ReadJsonFile(requests_file);

    if (requests_json.find("requests") == requests_json.end()) {
        throw std::runtime_error("requests file is empty");
    }

    std::vector<std::string> requests;
    for (const auto& request : requests_json["requests"]) {
        requests.emplace_back(request.get<std::string>());
    }
    return requests;
}

void ConverterJSON::putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers) {
    fs::path output_file = "../config/answers.json";
    answers_json["answers"] = nlohmann::json::object();

    int request_id = 1;
    for (const auto& answer : answers) {
        std::string request_key = "request" + std::to_string(request_id++);
        nlohmann::json request_data;

        if (!answer.empty()) {
            request_data["result"] = "true";
            nlohmann::json relevance;
            for (const auto& pair : answer) {
                nlohmann::json doc_info;
                doc_info["docid"] = pair.first;
                doc_info["rank"] = pair.second;
                relevance.push_back(doc_info);
            }
            request_data["relevance"] = relevance;
        } else {
            request_data["result"] = "false";
        }

        answers_json["answers"][request_key] = request_data;
    }

    WriteJsonFile(output_file, answers_json);
}



