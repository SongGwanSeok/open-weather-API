#include <iostream>
#include <curl/curl.h>

using namespace std;

//weather api {https://openweathermap.org/forecast5}
//1. 위도 경도  2. 도시 이름 3. 우편 번호

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
    cout << "도시 정보를 입력하세요: ";
    getline(cin, city);

    CURL *curl = curl_easy_init();
    string url = "https://api.openweathermap.org/data/2.5/weather?q="+city+"&appid="+apiKey;
    string weatherData;

    if(curl){
        // URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

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

int main() {
    string apiKey = "b1e668504d0535c4e1cc989cdcb31fa2";
    string weatherData;
    int choice;

    //1. 위도 경도  2. 도시 이름 3. 우편 번호
    while(true){
        cout << "어느 정보로 날씨를 검색 하시겠습니까?" << endl;
        cout << "1. 위도 경도  2. 도시 이름  3. 우편 번호" << endl;
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
        }
        cout << weatherData << endl;
    }

    return 0;

}