#include <iostream>
#include <curl/curl.h>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include <map>

using namespace rapidjson;
using namespace std;

//weather api {https://openweathermap.org/forecast5}

void displayAirPolution(const string& airPollution);


//curl 사용 시에 나오는 json을 callback 시켜주는 함수
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

map<string, string> weatherIcons = {
        {"Clear", "☀️"},
        {"Clouds", "☁️"},
        {"Rain", "🌧️"},
        {"Drizzle", "🌦️"},
        {"Thunderstorm", "⛈️"},
        {"Snow", "❄️"},
        {"Mist", "🌫️"},
        {"Smoke", "🌫️"},
        {"Haze", "🌫️"},
        {"Dust", "🌫️"},
        {"Fog", "🌫️"},
        {"Sand", "🌫️"},
        {"Ash", "🌫️"},
        {"Squall", "🌫️"},
        {"Tornado", "🌪️"},
};

// 날씨 상태에 해당하는 아이콘 반환
string getWeatherIcon(const string& weatherStatus) {
    if (weatherIcons.count(weatherStatus) > 0) {
        return weatherIcons[weatherStatus];
    }
    return "";
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

    string lat, lon;
    cout << "위도를 입력하세요: ";
    getline(cin, lat);
    cout << "경도를 입력하세요: ";
    getline(cin, lon);


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

    displayAirPolution(airPollution);

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

void displayAirPolution(const string& airPollution){
    // JSON 파싱
    Document data;
    data.Parse(airPollution.c_str());

    if(!data.HasParseError()) { // 파싱 오류 체크
        if(data.IsObject()) { // JSON 문서가 객체 형식인지 확인
            cout << "------------------------" << endl;
            if(data.HasMember("list") && data["list"].IsArray()){
                Value& airPollutionArray = data["list"];
                if (!airPollutionArray.Empty()) {
                    // 미세먼지 농도 추출
                    const Value& airQuality = airPollutionArray[0]["components"];
                    int pm25 = airQuality["pm2_5"].GetDouble();
                    int pm10 = airQuality["pm10"].GetDouble();

                    // 미세먼지 정보 출력
                    cout << "PM2.5: " << pm25 << " μg/m^3" << endl;
                    cout << "PM10: " << pm10 << " μg/m^3" << endl;
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
    int choice, break_point=0;

    //1. 위도 경도  2. 도시 이름 3. 우편 번호
    while(true){
        int curWeather=1;
        cout << "------------------------" << endl;
        cout << "어느 정보로 날씨를 검색 하시겠습니까?" << endl;
        cout << "1. 위도 경도  2. 도시 이름  3. 우편 번호 4. 일기 예보(도시 이름) 5. 미세먼지 확인 6. 끝내기" << endl;
        cout << "해당하는 숫자를 입력하세요: ";
        cin >> choice;
        getchar();

        switch (choice) {
            case 1:
                weatherData = getWDataByLatLon(apiKey);
                break;
            case 2:
                weatherData = getWDataByCity(apiKey);
                break;
            case 3:
                weatherData = getWDataByZipCode(apiKey);
                break;
            case 4:
                curWeather=0;
                getFDataByCity(apiKey);
                break;
            case 5:
                getAirQuality(apiKey);
                curWeather=0;
                break;
            case 6:
                break_point=1;
                break;
        }
        if(break_point){
            break;
        }else if(curWeather==1){
            displayWeatherInfo(weatherData);
        }
    }

    return 0;

}