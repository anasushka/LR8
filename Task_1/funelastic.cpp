#include "hehe.h"

//добавляем заголовочки чтобы сервер показать гениальному серверу что нашо строчка это на самом деле json шок
curl_slist* heheaders() {
    curl_slist* headers = nullptr;//мы хуячим указатель на указатель(curl_slist структура данных в libcurl которая содержит указатели на строки)
    headers = curl_slist_append(headers, "Content-Type: application/json"); //добавляем заголовок чтобы показать серверу что как бы строчка то наша это json
    return headers;
}
// Функция для обработки ответа от сервера (запись в строку)
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    ((std::string*)userp)->append((char*)contents, totalSize);
    return totalSize;
}
//жесткая настройка и отправка запросика
bool cURL(const std::string& jsonStr, const std::string& Url, std::string& response) {
    CURL* curl=curl_easy_init();
    if (!curl) {
        std::cerr << "Не получилось что-то по ссылочке найтись:'(" << std::endl;
        return false;
    }
    curl_easy_setopt(curl, CURLOPT_URL, Url.c_str());//настроиваем url для отправки данных
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr.c_str()); //указываем что мы отправляем
    curl_easy_setopt(curl, CURLOPT_POST, 1L);//указываем что наш запросик это post - отправить и соззранить данные. 1L - значени чтобы активировать post

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);  // Ответ будет записан в строку 'response'

    curl_slist* headers = heheaders();
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);//говорим что надо использовать список заголовков, а то что мы зря все это делали
    //мораль такова мы создали список заголовков, добавили туда хуйню  и говорим что вот списочек заголовков, используй и не пизди
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); // 10 секунд
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L); // Включить отладочный вывод
    //отправляем надеясь на чудо
    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return res == CURLE_OK;
}
//отправляем данные в эластик
void sendtoElastic(const busS& Bus) {

        //заполняем наши данные в json-структурки
    Json::Value json;
    json["BusNumber"]=Bus.number;
    json["BusNumberStr"]=std::to_string(Bus.number);
    json["BusType"]=std::string(Bus.type);
    json["BusPlace"]=std::string(Bus.place);
    json["TimeToGO"] = std::string(Bus.timetogo);
    json["TimeToCOME"] = std::string(Bus.timetocome);

    //херачим данные строку для отправки через http
        Json::StreamWriterBuilder writer;
        std::string jsonStr=Json::writeString(writer, json);
        std::string Url="http://localhost:9200/buses/_doc/";

        std::string response;
        if(!cURL(jsonStr,Url,response)) {
           std::cerr << "Ошибка при отправке данных" << std::endl;
       }
}

busS* searchBus=nullptr;
int searchCount=0;

void searchPlaceAndTime(const std::string& placeTOgo, const std::string& timeEarlier) {
    Json::Value query; //Json::Value класс в библиотеки json для построение json-структуры
    query["query"]["bool"]["must"] = Json::arrayValue; //описываем какие данные мы хотим получить

    Json::Value searchPlace;
    searchPlace["match"]["BusPlace"]["query"] = placeTOgo;
    // searchPlace["match"]["BusPlace"]["fuzziness"] = "AUTO";
    query["query"]["bool"]["must"].append(searchPlace);

    Json::Value searchTime;
    searchTime["range"]["TimeToCOME"]["lt"]=timeEarlier; //range чтобы показать что работает в даипозоле где searchTime lt(less than)
    query["query"]["bool"]["must"].append(searchTime);

    std::string Url = "http://localhost:9200/buses/_search";
        //данные в строку
    Json::StreamWriterBuilder writer;
    std::string jsonStr = Json::writeString(writer, query);
    std::string response;
    if(!cURL(jsonStr,Url,response)) {
        std::cerr << "Ошибка при отправке данных" << std::endl;
    }

    Json::CharReaderBuilder reader;
    Json::Value root;
    std::istringstream responseStream(response);
    std::string errs;

    if (!parseFromStream(reader, responseStream, &root, &errs)) {
        std::cerr << "Ошибка при парсинге JSON ответа: " << errs << std::endl;
        return;
    }

    // Извлечение данных из ответа
    const Json::Value& hits = root["hits"]["hits"];
    searchCount = hits.size();
    searchBus = (busS*)malloc(searchCount * sizeof(busS));

    for (int i = 0; i < searchCount; ++i) {
        const Json::Value& source = hits[i]["_source"];

        searchBus[i].number = source["BusNumber"].asInt();
        strncpy(searchBus[i].type, source["BusType"].asCString(), sizeof(searchBus[i].type) - 1);
        searchBus[i].type[sizeof(searchBus[i].type) - 1] = '\0';
        strncpy(searchBus[i].place, source["BusPlace"].asCString(), sizeof(searchBus[i].place));
        searchBus[i].place[sizeof(searchBus[i].place) - 1] = '\0';
        strncpy(searchBus[i].timetogo, source["TimeToGO"].asCString(), sizeof(searchBus[i].timetogo));
        searchBus[i].timetogo[sizeof(searchBus[i].timetogo) - 1] = '\0';
        strncpy(searchBus[i].timetocome, source["TimeToCOME"].asCString(), sizeof(searchBus[i].timetocome));
        searchBus[i].timetocome[sizeof(searchBus[i].timetocome) - 1] = '\0';
    }
}

busS* onesearchBus = nullptr;
int onesearchCount = 0;

void searchElastic(const std::string& searchInput) {
    bool isNumber=true;
    for(char c : searchInput) {
        if(!std::isdigit(c)) {
            isNumber=false;
        }
    }
    Json::Value query;
    query["query"]["bool"]["should"] = Json::arrayValue;

    if (searchInput.find('-') != std::string::npos) {
        std::stringstream ss(searchInput);
        int min, max;
        char dash;
        ss >> min >> dash >> max;
        Json::Value rangeQuery;
        rangeQuery["range"]["BusNumber"]["gte"] = min;
        rangeQuery["range"]["BusNumber"]["lte"] = max;
        query["query"]["bool"]["should"].append(rangeQuery);
    } else if(isNumber) { // Точное совпадение по номеру автобуса
        Json::Value searchNumber;
        searchNumber["term"]["BusNumber"]["value"] = searchInput;
        query["query"]["bool"]["should"].append(searchNumber);
    }

    // Поиск по номеру автобуса с помощью wildcard
    /*Json::Value searchNumberWildcard;
    searchNumberWildcard["wildcard"]["BusNumberStr"]["value"] = "*" + searchInput + "*";
    query["query"]["bool"]["should"].append(searchNumberWildcard);*/

    Json::Value searchType;
    searchType["match"]["BusType"]["query"] = searchInput;
  // searchType["match"]["BusType"]["fuzziness"] = "AUTO"; // Автоматическая настройка нечеткости
    query["query"]["bool"]["should"].append(searchType);

    /*
    Json::Value searchType;
    searchType["wildcard"]["BusType"]["value"] = "*" + searchInput + "*";
    query["query"]["bool"]["should"].append(searchType);

    Json::Value searchType;
    searchType["prefix"]["BusType"]["value"] = searchInput;
    query["query"]["bool"]["should"].append(searchType);

     */

   Json::Value searchPlace;
    searchPlace["match"]["BusPlace"]["query"] = searchInput;
   // searchPlace["match"]["BusPlace"]["fuzziness"] = "AUTO";
    query["query"]["bool"]["should"].append(searchPlace);
    /*
Json::Value searchPlace;
searchPlace["wildcard"]["BusPlace"]["value"] = "*" + searchInput + "*";
query["query"]["bool"]["should"].append(searchPlace);

    Json::Value searchPlace;
searchPlace["prefix"]["BusPlace"]["value"] = searchInput;
query["query"]["bool"]["should"].append(searchPlace);
    */

    bool validTime=isTimeFormat(searchInput);
    if (searchInput.find('-') != std::string::npos) {
        std::stringstream ss(searchInput);
        int min, max;
        char dash;
        ss >> min >> dash >> max;
        if(max<23) {
            std::string  minTime = std::to_string(min)+":00";
            std::string  maxTime = std::to_string(max)+":00";
            Json::Value rangeTimeToGO;
            rangeTimeToGO["range"]["TimeToGO"]["gte"] = minTime;
            rangeTimeToGO["range"]["TimeToGO"]["lte"] = maxTime;
            query["query"]["bool"]["should"].append(rangeTimeToGO);

            Json::Value rangeTimeToCOME;
            rangeTimeToCOME["range"]["TimeToCOME"]["gte"] = minTime;
            rangeTimeToCOME["range"]["TimeToCOME"]["lte"] = maxTime;
            query["query"]["bool"]["should"].append(rangeTimeToCOME);
        }
    }else if(validTime) {
        Json::Value searchTimeToGo;
        searchTimeToGo["match"]["TimeToGO"]["query"] = searchInput;
        query["query"]["bool"]["should"].append(searchTimeToGo);

        Json::Value searchTimeToCome;
        searchTimeToCome["match"]["TimeToCOME"]["query"] = searchInput;
        query["query"]["bool"]["should"].append(searchTimeToCome);
    }

            std::string url = "http://localhost:9200/buses/_search";

            // Преобразование JSON-запроса в строку
            Json::StreamWriterBuilder writer;
            std::string jsonStr = Json::writeString(writer, query);
            std::string response;

            //std::cout << jsonStr << std::endl;

            if (!cURL(jsonStr, url, response)) {
                std::cerr << "Ошибка при отправке данных" << std::endl;
                return;
            }
            //std::cout << response << std::endl;

            Json::CharReaderBuilder reader;
            Json::Value root;
            std::istringstream responseStream(response);
            std::string errs;

            if (!parseFromStream(reader, responseStream, &root, &errs)) {
                std::cerr << "Ошибка при парсинге JSON ответа: " << errs << std::endl;
                return;
            }

            // Извлечение данных из ответа
            const Json::Value& hits = root["hits"]["hits"];
            onesearchCount = hits.size();
            onesearchBus = (busS*)malloc(onesearchCount * sizeof(busS));


            for (int i = 0; i < onesearchCount; ++i) {
                const Json::Value& source = hits[i]["_source"];
                onesearchBus[i].number = (source["BusNumber"].asInt());
                strncpy(onesearchBus[i].type, source["BusType"].asCString(), sizeof(onesearchBus[i].type) - 1);
                onesearchBus[i].type[sizeof(onesearchBus[i].type) - 1] = '\0';
                strncpy(onesearchBus[i].place, source["BusPlace"].asCString(), sizeof(onesearchBus[i].place) - 1);
                onesearchBus[i].place[sizeof(onesearchBus[i].place) - 1] = '\0';
                strncpy(onesearchBus[i].timetogo, source["TimeToGO"].asCString(), sizeof(onesearchBus[i].timetogo) - 1);
                onesearchBus[i].timetogo[sizeof(onesearchBus[i].timetogo) - 1] = '\0';
                strncpy(onesearchBus[i].timetocome, source["TimeToCOME"].asCString(), sizeof(onesearchBus[i].timetocome) - 1);
                onesearchBus[i].timetocome[sizeof(onesearchBus[i].timetocome) - 1] = '\0';
            }

            if (onesearchCount == 0) {
                std::cout << "Нет автобусов, соответствующих запросу." << std::endl;
            } else {
                for (int i = 0; i < onesearchCount; ++i) {
                    std::cout << "Номер автобуса: " << onesearchBus[i].number << ".\n";
                    std::cout << "Тип: " << onesearchBus[i].type << ".\n";
                    std::cout << "Место назначения: " << onesearchBus[i].place << ".\n";
                    std::cout << "Время отправления: " << onesearchBus[i].timetogo << ".\n";
                    std::cout << "Время прибытия: " << onesearchBus[i].timetocome << ".\n";
                    std::cout << "--------------------------------------\n";
                }
    }
}

bool clearElastic() {
CURL *curl=curl_easy_init();
    if (!curl) {
        std::cerr << "Ошибка инициализации";
          return false;
    }
    // URL для удаления документов с помощью _delete_by_query
    std::string url = "http://localhost:9200/buses/_delete_by_query";

    // Тело запроса для удаления всех документов
    std::string query = R"({"query": {"match_all": {}}})";

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, query.c_str());

    // Заголовок для указания типа данных JSON
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Ошибка очистки Elasticsearch: " << curl_easy_strerror(res) << "\n";
    }

    curl_easy_cleanup(curl);
    return res == CURLE_OK;

}

void deleteFromElastic(int number) {
    Json::Value query;
    query["query"]["term"]["BusNumber"] = number;

    Json::StreamWriterBuilder writer;
    std::string jsonStr = writeString(writer, query);
    std::string response;
    std::string url = "http://localhost:9200/buses/_delete_by_query";

    if(!cURL(jsonStr,url,response)) {
        std::cerr << "Ошибка  при удалении из эластика. ";
    }else {
        std::cout <<"Данные автобуса с номером "<<number << " удалены.";
    }
}

void updateInElastic(int index) {
    const busS& updatedBus= Bus[index];
    Json::Value query;
    query["query"]["term"]["BusNumber"] = updatedBus.number;

    // Обновлённые данные
    Json::Value script;
    script["source"] = R"(
        ctx._source.BusType = params.BusType;
        ctx._source.BusPlace = params.BusPlace;
        ctx._source.TimeToGO = params.TimeToGO;
        ctx._source.TimeToCOME = params.TimeToCOME;
    )";
    script["params"]["BusType"] = std::string(updatedBus.type);
    script["params"]["BusPlace"] = std::string(updatedBus.place);
    script["params"]["TimeToGO"] = std::string(updatedBus.timetogo);
    script["params"]["TimeToCOME"] = std::string(updatedBus.timetocome);

    query["script"] = script;

    Json::StreamWriterBuilder writer;
    std::string jsonStr = Json::writeString(writer, query);
    std::string response;
    std::string url = "http://localhost:9200/buses/_update_by_query";

    if(!cURL(jsonStr,url,response)) {
        std::cerr << "Ошибка  при удалении из эластика. ";
    }else {
        std::cout <<"Данные автобуса с номером "<<updatedBus.number << " обновлены.";
    }
}