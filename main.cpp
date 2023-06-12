#include <iostream>
#include <curl/curl.h>

using namespace std;

//weather api {https://openweathermap.org/forecast5}
//1. 위도 경도  2. 도시 이름 3. 우편 번호

//받아온 json 데이터를 가지고 정보를 출력해준다.
void displayWeatherInfo(const string& weatherData){


}

string getWDataByCity(const string& apiKey, const string& city){
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
    string city;

    cout << "도시 정보를 입력하세요: ";
    getline(cin, city);

    string weatherData = getWDataByCity(apiKey, city);

    return 0;

}