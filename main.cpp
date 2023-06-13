#include <iostream>
#include <curl/curl.h>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;
using namespace std;

//weather api {https://openweathermap.org/forecast5}
//1. 위도 경도  2. 도시 이름 3. 우편 번호

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
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
            if(data.HasMember("name") && data["name"].IsString()){
                string cityName = data["name"].GetString();
                cout << "도시명: " << cityName << endl;
            }
            if(data.HasMember("main") && data["main"].IsObject()){
                const Value& mainObj = data["main"];
                if (mainObj.HasMember("temp") && mainObj["temp"].IsNumber()) {
                    double temperature = mainObj["temp"].GetDouble();
                    std::cout << "온도: " << KelvinToCelsius(temperature) << "°C" << std::endl;
                }

                if (mainObj.HasMember("humidity") && mainObj["humidity"].IsNumber()) {
                    int humidity = mainObj["humidity"].GetInt();
                    std::cout << "습도: " << humidity << "%" << std::endl;
                }
            }
            if(data.HasMember("wind") && data["wind"].IsObject()){
                const Value& windObj = data["wind"];
                if (windObj.HasMember("speed") && windObj["speed"].IsNumber()) {
                    double windSpeed = windObj["speed"].GetDouble();
                    std::cout << "풍속: " << windSpeed << " m/s" << std::endl;
                }
            }
            else {
                std::cout << "잘못된 JSON 형식" << std::endl;
            }
        }
        else {
            std::cout << "날씨 데이터를 분석하는 동안 오류가 발생했습니다." << std::endl;
        }
    }
    return weatherData;
}

int main() {
    string apiKey = "b1e668504d0535c4e1cc989cdcb31fa2";
    string weatherData;
    int choice, break_point=0;

    //1. 위도 경도  2. 도시 이름 3. 우편 번호
    while(true){
        cout << "어느 정보로 날씨를 검색 하시겠습니까?" << endl;
        cout << "1. 위도 경도  2. 도시 이름  3. 우편 번호 4. 끝내기" << endl;
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
                break_point=1;
                break;
        }
        if(break_point){
            break;
        }else{
            displayWeatherInfo(weatherData);
        }
    }

    return 0;

}