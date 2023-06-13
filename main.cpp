#include <iostream>
#include <curl/curl.h>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include <map>

using namespace rapidjson;
using namespace std;

//weather api {https://openweathermap.org/forecast5}

string getWeatherIcon(const string& weatherStatus);
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);
const Value& geocoding(string cityName);
string getWDataByLatLon(const string& apiKey);
string getWDataByCity(const string& apiKey);
string getWDataByZipCode(const string& apiKey);
void getFDataByCity(const string& apiKey);
void getAirQuality(const string& apiKey);
double KelvinToCelsius(double kel);
void displayWeatherInfo(const string& weatherData);
void displayAirPollution(const string &airPollution, string cityName);
void displayWeatherForecast(const string& weatherData);

//curl 사용 시에 나오는 json을 callback 시켜주는 함수
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// 날씨 상태에 해당하는 아이콘 반환
string getWeatherIcon(const string& weatherStatus) {

    map<string, string> weatherIcons;
    weatherIcons.insert(make_pair("Clear", "☀️"));
    weatherIcons.insert(make_pair("Clouds", "☁️"));
    weatherIcons.insert(make_pair("Rain", "🌧️"));
    weatherIcons.insert(make_pair("Drizzle", "🌦️"));
    weatherIcons.insert(make_pair("Snow", "❄️"));
    weatherIcons.insert(make_pair("Thunderstorm", "⛈️"));
    weatherIcons.insert(make_pair("Mist", "🌫️"));
    weatherIcons.insert(make_pair("Smoke", "🌫️"));
    weatherIcons.insert(make_pair("Haze", "🌫️"));
    weatherIcons.insert(make_pair("Dust", "🌫️"));
    weatherIcons.insert(make_pair("Fog", "🌫️"));
    weatherIcons.insert(make_pair("Sand", "🌫️"));
    weatherIcons.insert(make_pair("Ash", "🌫️"));
    weatherIcons.insert(make_pair("Squall", "🌫️"));
    weatherIcons.insert(make_pair("Tornado", "🌪️"));


    if (weatherIcons.count(weatherStatus) > 0) {
        return weatherIcons[weatherStatus];
    }
    return "";
}

const Value& geocoding(string cityName){
    //Mozilla5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.103 Safari/537.36

    // Nominatim API URL 생성
    string apiUrl = "https://nominatim.openstreetmap.org/search?q=" + cityName + "&format=json&limit=1";

    // HTTP 요청 초기화
    CURL* curl = curl_easy_init();
    if (curl){
        // HTTP 요청 설정
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // User-Agent 설정
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.103 Safari/537.36");

        // HTTP 요청 수행
        string latlonjson;

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &latlonjson);
        CURLcode result = curl_easy_perform(curl);

        // HTTP 요청 결과 처리
        if (result == CURLE_OK){
            // JSON 데이터 파싱
            Document document;
            document.Parse(latlonjson.c_str());

            // 결과 확인
            if (!document.Empty()){
                if (document.IsArray()){
                    const Value& result = document[0];
                    if (result.HasMember("lat") && result.HasMember("lon")){
                        if (result.HasMember("licence") && result["licence"].IsString()) {
                            const std::string& licence = result["licence"].GetString();
                            cout << "------------------------" << endl;
                            cout << "Licence: " << licence << endl;
                        }
                        return result;
                    }
                    else{
                        cout << "도시의 위도와 경도를 찾을 수 없습니다." << endl;
                    }
                }
                else{
                    cout << "잘못된 JSON 형식 - 배열이 필요합니다." << endl;
                }
            }
            else{
                cout << "도시에 대한 검색 결과가 없습니다." << endl;
            }
        }
        else{
            cout << "HTTP 요청을 수행하지 못했습니다: " << curl_easy_strerror(result) << endl;
        }
        // CURL 자원 정리
        curl_easy_cleanup(curl);
    }
    else{
        cout << "CURL을 초기화하지 못했습니다." << endl;
    }
}


//latitude, longitude(위도, 경도)로 weatherData 받아오는 함수
string getWDataByLatLon(const string& apiKey){
    string lat, lon;
    cout << "위도를 입력하세요: ";
    getline(cin, lat);
    cout << "경도를 입력하세요: ";
    getline(cin, lon);

    CURL *curl = curl_easy_init();
    string url = "https://api.openweathermap.org/data/2.5/weather?lat=" + lat + "&lon=" + lon + "&appid="+apiKey;
    string weatherData;

    if(curl){
        // URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &weatherData);

        // METHOD
        // DEFAULT : GET

        // EXECUTE
        CURLcode res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);

        if(res != CURLE_OK){
            cout << "날씨 정보를 불러오는 도중에 문제 발생:" << curl_easy_strerror(res) << endl;
        }
    }

    return weatherData;
}

// city 정보로 weatherData 받아오는 함수
string getWDataByCity(const string& apiKey){
    string city;
    cout << "도시 이름을 영어로 입력하세요: ";
    getline(cin, city);

    CURL *curl = curl_easy_init();
    string url = "https://api.openweathermap.org/data/2.5/weather?q="+city+"&appid="+apiKey;
    string weatherData;

    if(curl){
        // URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &weatherData);

        // METHOD
        // DEFAULT : GET

        // EXECUTE
        CURLcode res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);

        if(res != CURLE_OK){
            cout << "날씨 정보를 불러오는 도중에 문제 발생:" << curl_easy_strerror(res) << endl;
        }
    }


    return weatherData;
}

// zipcode(우편 번호) 정보로 weatherData 받아오는 함수
string getWDataByZipCode(const string& apiKey){
    string zip;
    cout << "우편 번호를 입력하세요: ";
    getline(cin, zip);

    CURL *curl = curl_easy_init();
    string url = "https://api.openweathermap.org/data/2.5/weather?zip="+zip+"&appid="+apiKey;
    string weatherData;

    if(curl){
        // URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &weatherData);

        // METHOD
        // DEFAULT : GET

        // EXECUTE
        CURLcode res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);

        if(res != CURLE_OK){
            cout << "날씨 정보를 불러오는 도중에 문제 발생:" << curl_easy_strerror(res) << endl;
        }
    }

    return weatherData;
}

//위도 경도 정보를 가지고 미세먼지 정보 확인
void getAirQuality(const string& apiKey) {
    CURL* curl;
    string airPollution;
    string city;
    cout << "도시 이름을 영어로 입력하세요: ";
    getline(cin, city);

    const Value &latLonList = geocoding(city);
    string lat = latLonList["lat"].GetString();
    string lon = latLonList["lon"].GetString();

    // OpenWeatherMap API 요청 URL (API 키를 포함해야 합니다)
    string url = "http://api.openweathermap.org/data/2.5/air_pollution?lat=" + lat + "&lon=" + lon + "&appid=" + apiKey;

    // cURL 초기화
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl){
        // URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &airPollution);

        // METHOD
        // DEFAULT : GET

        // EXECUTE
        CURLcode res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);

        if(res != CURLE_OK){
            cout << "날씨 정보를 불러오는 도중에 문제 발생:" << curl_easy_strerror(res) << endl;
        }
    }

    displayAirPollution(airPollution, city);

}

//Kelvin 온도를 Celsius 온도로 변환
double KelvinToCelsius(double kel){
    double cel;
    cel = kel - 273.15;

    return cel;
}

void displayWeatherInfo(const string& weatherData){
    // JSON 파싱
    Document data;
    data.Parse(weatherData.c_str());

    if(!data.HasParseError()) { // 파싱 오류 체크
        if(data.IsObject()) { // JSON 문서가 객체 형식인지 확인
            cout << "------------------------" << endl;
            if(data.HasMember("name") && data["name"].IsString()){
                string cityName = data["name"].GetString();
                cout << "도시명: " << cityName << endl;
            }
            if(data.HasMember("main") && data["main"].IsObject()){
                Value& mainObj = data["main"];
                if (mainObj.HasMember("temp") && mainObj["temp"].IsNumber()) {
                    double temperature = mainObj["temp"].GetDouble();
                    cout << "온도: " << KelvinToCelsius(temperature) << "°C" << endl;
                }

                if (mainObj.HasMember("humidity") && mainObj["humidity"].IsNumber()) {
                    int humidity = mainObj["humidity"].GetInt();
                    cout << "습도: " << humidity << "%" << endl;
                }
            }
            if(data.HasMember("wind") && data["wind"].IsObject()){
                Value& windObj = data["wind"];
                if (windObj.HasMember("speed") && windObj["speed"].IsNumber()) {
                    double windSpeed = windObj["speed"].GetDouble();
                    cout << "풍속: " << windSpeed << " m/s" << endl;
                }
            }
            if(data.HasMember("weather") && data["weather"].IsArray()) {
                Value &weatherArray = data["weather"];
                if (!weatherArray.Empty()) {
                    const Value &firstWeatherObject = weatherArray[0];
                    if (firstWeatherObject.HasMember("main") && firstWeatherObject["main"].IsString()) {
                        string weatherStatus = firstWeatherObject["main"].GetString();
                        // 이제 mainValue 변수에 "main" 필드의 값이 저장됩니다.
                        string weatherIcon = getWeatherIcon(weatherStatus);
                        cout << "날씨 상태: " << weatherStatus << " " << weatherIcon << endl;
                    }
                }
            }
        }else {
            cout << "잘못된 JSON 형식" << endl;
        }
    }else {
        cout << "날씨 데이터를 분석하는 동안 오류가 발생했습니다." << endl;
    }
}

// 5일 동안의 일기 예보 정보 출력 (15시 기준)
void displayWeatherForecast(const string& weatherData) {
    // JSON 파싱
    Document data;
    data.Parse(weatherData.c_str());

    if (!data.HasParseError()) { // 파싱 오류 체크
        if (data.IsObject()) { // JSON 문서가 객체 형식인지 확인
            if (data.HasMember("list") && data["list"].IsArray()) {
                Value& forecastArray = data["list"];
                if (!forecastArray.Empty()) {
                    cout << "------------------------" << endl;
                    cout << "5일 동안의 15시 기준 일기 예보:" << endl;
                    for (SizeType i = 0; i < forecastArray.Size(); i++) {
                        const Value& forecastObj = forecastArray[i];
                        if (forecastObj.HasMember("dt_txt") && forecastObj["dt_txt"].IsString()) {
                            string forecastDateTime = forecastObj["dt_txt"].GetString();
                            // 시간이 15시일 때만 출력
                            if (forecastDateTime.substr(11, 2) == "15") {
                                cout << "------------------------" << endl;
                                cout << "일시: " << forecastDateTime << endl;
                                if (forecastObj.HasMember("main") && forecastObj["main"].IsObject()) {
                                    const Value& mainObj = forecastObj["main"];
                                    if (mainObj.HasMember("temp") && mainObj["temp"].IsNumber()) {
                                        double temperature = mainObj["temp"].GetDouble();
                                        cout << "온도: " << KelvinToCelsius(temperature) << "°C" << endl;
                                    }
                                    if (mainObj.HasMember("humidity") && mainObj["humidity"].IsNumber()) {
                                        int humidity = mainObj["humidity"].GetInt();
                                        cout << "습도: " << humidity << "%" << endl;
                                    }
                                }
                                if (forecastObj.HasMember("weather") && forecastObj["weather"].IsArray()) {
                                    const Value& weatherArray = forecastObj["weather"];
                                    if (!weatherArray.Empty()) {
                                        const Value& firstWeatherObject = weatherArray[0];
                                        if (firstWeatherObject.HasMember("main") && firstWeatherObject["main"].IsString()) {
                                            string weatherStatus = firstWeatherObject["main"].GetString();
                                            // 이제 mainValue 변수에 "main" 필드의 값이 저장됩니다.
                                            string weatherIcon = getWeatherIcon(weatherStatus);
                                            cout << "날씨 상태: " << weatherStatus << " " << weatherIcon << endl;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else {
            cout << "잘못된 JSON 형식" << endl;
        }
    }
    else {
        cout << "날씨 데이터를 분석하는 동안 오류가 발생했습니다." << endl;
    }
}

void displayAirPollution(const string &airPollution, string cityName){
    // JSON 파싱
    Document data;
    data.Parse(airPollution.c_str());

    if(!data.HasParseError()) { // 파싱 오류 체크
        if(data.IsObject()) { // JSON 문서가 객체 형식인지 확인
            cout << "------------------------" << endl;
            cout << "도시 이름: " << cityName << endl;
            if(data.HasMember("list") && data["list"].IsArray()){
                Value& airPollutionArray = data["list"];
                if (!airPollutionArray.Empty()) {
                    // 미세먼지 농도 추출
                    const Value& airQuality = airPollutionArray[0]["components"];
                    int pm25 = airQuality["pm2_5"].GetDouble();
                    int pm10 = airQuality["pm10"].GetDouble();

                    // 미세먼지 정보 출력
                    cout << "미세먼지 : " << pm10 << " μg/m^3" << endl;
                    cout << "초미세먼지 : " << pm25 << " μg/m^3" << endl;
                }
            }
        }
        else {
            cout << "잘못된 JSON 형식" << endl;
        }
    }
    else {
        std::cout << "미세먼지 데이터를 분석하는 동안 오류가 발생했습니다." << std::endl;
    }

}


// city(도시 이름) 정보로 forecastData 받아오는 함수
void getFDataByCity(const string& apiKey){
    string city;
    cout << "도시 이름을 영어로 입력하세요: ";
    getline(cin, city);

    CURL *curl = curl_easy_init();
    string url = "https://api.openweathermap.org/data/2.5/forecast?q="+city+"&appid="+apiKey;
    string forecastData;

    if(curl){
        // URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &forecastData);
        // METHOD
        // DEFAULT : GET

        // EXECUTE
        CURLcode res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);

        if(res != CURLE_OK){
            cout << "날씨 정보를 불러오는 도중에 문제 발생:" << curl_easy_strerror(res) << endl;
        }
    }

    displayWeatherForecast(forecastData);
}

int main() {
    string apiKey = "b1e668504d0535c4e1cc989cdcb31fa2";
    string weatherData;
    int choice, method, break_point=0;

    //1. 위도 경도  2. 도시 이름 3. 우편 번호
    while(true){
        int curWeather=1;
        cout << "------------------------" << endl;
        cout << "어느 정보를 검색하시겠습니까?" << endl;
        cout << "1. 현재 날씨 2. 일기 예보 3. 미세 먼지 4. 종료" << endl;
        cout << "해당하는 숫자를 입력하세요: ";
        cin >> choice;
        getchar();

        switch (choice) {
            case 1:
                cout << "------------------------" << endl;
                cout << "검색 방식을 선택합니다." << endl;
                cout << "1. 위도 경도  2. 도시 이름  3. 우편 번호" << endl;
                cout << "해당하는 숫자를 입력하세요 : ";
                cin >> method;
                getchar();
                // 현재 날씨
                switch (method) {
                    case 1:
                        weatherData = getWDataByLatLon(apiKey);
                        break;
                    case 2:
                        weatherData = getWDataByCity(apiKey);
                        break;
                    case 3:
                        weatherData = getWDataByZipCode(apiKey);
                        break;
                }
                displayWeatherInfo(weatherData);
                break;
            case 2:
                //일기 예보
                getFDataByCity(apiKey);
                break;
            case 3:
                //미세 먼지
                getAirQuality(apiKey);
                break;
            case 4:
                break_point=1;
                break;
        }
        if(break_point) break;
    }

    return 0;

}