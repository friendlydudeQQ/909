# 🔍 Поисковый движок с инвертированным индексом

Проект реализует поисковую систему, которая:
1. Индексирует текстовые документы с использованием многопоточности
2. Обрабатывает поисковые запросы
3. Ранжирует результаты по релевантности
4. Сохраняет ответы в JSON-формате

## 📦 Зависимости
- Компилятор C++17+
- Библиотека [nlohmann/json](https://github.com/nlohmann/json) (уже должна быть в проекте)
- CMake (опционально)

## 🚀 Быстрый старт

### Сборка проекта
```bash
g++ -std=c++17 src/*.cpp -Iinclude -o search_engine
```
## 📂 Структура файлов
````
project/
├── config/
│   ├── config.json     # Конфигурация
│   ├── request.json    # Поисковые запросы
│   └── answers.json    # Результаты (генерируется)
├── src/
│   ├── main.cpp        # Основная логика
│   ├── converter_json.cpp # Работа с JSON
│   ├── index.cpp # Формула релевантности
│   ├── inverted_index.cpp # Инвертированный индекс
│   └── search_server.cpp # Логика движка
├── include/
│    ├── index.h
│    ├── converter_json.h
│    ├── inverted_index.h        
│    └── search_server.h   
├── resources/
│    ├── file1.txt
│    ├── file2.txt
│    └── file3.txt
└── tests/ # Тесты
     ├── test_index.cpp
     └── test_inverted_index.cpp    
````
## ⚙️ Конфигурация
### config.json
````
{
  "config": {
    "name": "SkillboxSearchEngine",
    "version": "0.1",
    "max_responses": 5
  },
  "files": [
    "../resources/file1.txt",
    "../resources/file2.txt",
    "../resources/file3.txt"
  ]
}
````
### request.json
````
{
  "requests": [
    "apple",
    "cloud"
  ]
}
````
## 🔧 Основные компоненты
### 1. InvertedIndex
 -  Многопоточная индексация документов

 -  Частотный анализ слов

 -  Хранение данных в формате: слово → {doc_id, count}

### 2. SearchServer
 -  Обработка поисковых запросов

 -  Расчет релевантности по формуле:
````
relevance = (word_count / max_word_freq) / total_words
````
 -  Ранжирование результатов
### 3. ConverterJSON
 -  Чтение/запись конфигурационных файлов

 -  Валидация входных данных

 -  Сериализация результатов

## 🧪 Тестирование
### Запуск тестов
````
# Сборка тестов
g++ -std=c++17 tests/*.cpp src/*.cpp -Iinclude -lgtest -pthread -o run_tests

# Запуск
./run_tests
````
### Основные тест-кейсы:
#### Инвертированный индекс
 - Индексация документов (TestBasic, TestBasic2)

 - Обработка отсутствующих слов (TestInvertedIndexMissingWord)
#### Поисковый сервер
 - Ранжирование результатов (TestSimple, TestTop5)

 - Расчет релевантности (TestRelevance)
## 📊 Пример работы
Входные документы:
````
file1.txt: "apple car mountain music cloud"
file2.txt: "music apple tree river"
file3.txt: "mountain forest cloud apple music"
````
Запрос:
````
{
  "requests": [
    "apple",
    "cloud"
  ]
}
````
Результат (answers.json):
````
{
    "answers": {
        "request1": {
            "relevance": [
                {
                    "docid": 1,
                    "rank": 1.0
                },
                {
                    "docid": 2,
                    "rank": 1.0
                },
                {
                    "docid": 3,
                    "rank": 1.0
                }
            ],
            "result": "true"
        },
        "request2": {
            "relevance": [
                {
                    "docid": 1,
                    "rank": 1.0
                },
                {
                    "docid": 3,
                    "rank": 1.0
                },
                {
                    "docid": 2,
                    "rank": 0.0
                }
            ],
            "result": "true"
        }
    }
}
````
## 🛠️ Особенности реализации
 - Потокобезопасная индексация документов

 - Поддержка стоп-слов (реализуется в SearchServer)

 - Гибкая настройка через JSON

 - Логирование процесса индексации
--------------------------------------------------------
