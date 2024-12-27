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
void sendtoElastic(const Train& train) {

        //заполняем наши данные в json-структурки
    Json::Value json;
    json["TrainDate"]=std::string(train.date);
    json["TrainDayOfTheWeek"]=train.dayoftheweek;
    json["TrainPlace"]=std::string(train.place);
    json["TimeToGO"] = std::string(train.timetogo);
    json["TrainFreeSeats"] = train.freeSeats;
    //херачим данные строку для отправки через http
        Json::StreamWriterBuilder writer;
        std::string jsonStr=Json::writeString(writer, json);
        std::string Url="http://localhost:9200/traininfo/_doc/";

        std::string response;
        if(!cURL(jsonStr,Url,response)) {
           std::cerr << "Ошибка при отправке данных" << std::endl;
       }
}

Train* searchTrain=nullptr;
int searchCount=0;

void search_mNkt(int availableSeats, const std::string& placeTOgo,int DayOfTheWeek, const std::string& timeEarlier) {
    Json::Value query; //Json::Value класс в библиотеки json для построение json-структуры
    query["query"]["bool"]["must"] = Json::arrayValue; //описываем какие данные мы хотим получить

    Json::Value searchSeats;
    searchSeats["range"]["TrainFreeSeats"]["gt"]=availableSeats;//greater than
    query["query"]["bool"]["must"].append(searchSeats);

    Json::Value searchPlace;
    searchPlace["match"]["TrainPlace"]["query"] = placeTOgo;
    //searchPlace["match"]["TrainPlace"]["fuzziness"] = "AUTO";
    query["query"]["bool"]["must"].append(searchPlace);

    Json::Value searchDayOfTheWeek;
    searchDayOfTheWeek["term"]["TrainDayOfTheWeek"]= DayOfTheWeek;
    query["query"]["bool"]["must"].append(searchDayOfTheWeek);

    Json::Value searchTime;
    searchTime["range"]["TimeToGO"]["lt"]=timeEarlier; //range чтобы показать что работает в даипозоле где searchTime lt(less than)
    query["query"]["bool"]["must"].append(searchTime);

    std::string Url = "http://localhost:9200/traininfo/_search";
        //данные в строку
    Json::StreamWriterBuilder writer;
    std::string jsonStr = Json::writeString(writer, query);

   // std::cout << jsonStr << std::endl;

    std::string response;
    if(!cURL(jsonStr,Url,response)) {
        std::cerr << "Ошибка при отправке данных" << std::endl;
    }

    Json::CharReaderBuilder reader;
    Json::Value root;
    std::istringstream responseStream(response);
    std::string errs;
   // std::cout << "Ответ сервера: " << response << std::endl;

    if (!parseFromStream(reader, responseStream, &root, &errs)) {
        std::cerr << "Ошибка при парсинге JSON ответа: " << errs << std::endl;
        return;
    }

    // Извлечение данных из ответа
    const Json::Value& hits = root["hits"]["hits"];
    searchCount = hits.size();
    searchTrain = (Train*)malloc(searchCount * sizeof(Train));

    for (int i = 0; i < searchCount; ++i) {
        const Json::Value& source = hits[i]["_source"];
        strncpy(searchTrain[i].date, source["TrainDate"].asCString(), sizeof(searchTrain[i].date));
        searchTrain[i].date[sizeof(searchTrain[i].date) - 1] = '\0';
        searchTrain[i].dayoftheweek = source["TrainDayOfTheWeek"].asInt();
        strncpy(searchTrain[i].place, source["TrainPlace"].asCString(), sizeof(searchTrain[i].place));
        searchTrain[i].place[sizeof(searchTrain[i].place) - 1] = '\0';
        strncpy(searchTrain[i].timetogo, source["TimeToGO"].asCString(), sizeof(searchTrain[i].timetogo));
        searchTrain[i].timetogo[sizeof(searchTrain[i].timetogo) - 1] = '\0';
        searchTrain[i].freeSeats = source["TrainFreeSeats"].asInt();
          }
    if (searchCount == 0) {
        std::cout << "Нет автобусов, соответствующих запросу." << std::endl;
    } else {
        for (int i = 0; i < searchCount; ++i) {
            std::cout << "Место назначения: " << searchTrain[i].place << ".\n";
            std::cout << "Время отправления: " << searchTrain[i].timetogo << ".\n";
            std::cout << "--------------------------------------";
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
    std::string url = "http://localhost:9200/traininfo/_delete_by_query";

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

void deleteFromElastic(const std::string& timestr) {
    Json::Value query;
    query["query"]["term"]["BusNumber"] = std::string(timestr);

    Json::StreamWriterBuilder writer;
    std::string jsonStr = writeString(writer, query);
    std::string response;
    std::string url = "http://localhost:9200/traininfo/_delete_by_query";

    if(!cURL(jsonStr,url,response)) {
        std::cerr << "Ошибка  при удалении из эластика. ";
    }else {
        std::cout <<"Данные автобуса удалены.";
    }
}

void updateInElastic(int index) {
    const Train& updatedTrain= train[index];
    Json::Value query;
    query["query"]["term"]["TimeToGO"] = std::string(updatedTrain.timetogo);

    // Обновлённые данные
    Json::Value script;
    script["source"] = R"(
        ctx._source.TrainDate = params.TrainDate;
        ctx._source.TrainDayOfTheWeek = params.TrainDayOfTheWeek;
        ctx._source.TrainPlace = params.TrainPlace;
        ctx._source.TimeToGO = params.TimeToGO;
        ctx._source.TrainFreeSeats = params.TrainFreeSeats;
    )";
    script["params"]["TrainDate"] = std::string(updatedTrain.date);
    script["params"]["TrainDayOfTheWeek"] = std::to_string(updatedTrain.dayoftheweek);
    script["params"]["TrainPlace"] = std::string(updatedTrain.place);
    script["params"]["TimeToGO"] = std::string(updatedTrain.timetogo);
    script["params"]["TrainFreeSeats"] = std::to_string(updatedTrain.freeSeats);

    query["script"] = script;

    Json::StreamWriterBuilder writer;
    std::string jsonStr = Json::writeString(writer, query);
    std::string response;
    std::string url = "http://localhost:9200/traininfo/_update_by_query";

    if(!cURL(jsonStr,url,response)) {
        std::cerr << "Ошибка  при удалении из эластика. ";
    }else {
        std::cout <<"Данные автобуса обновлены.";
    }
}