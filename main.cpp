#include <iostream>
#include <curl/curl.h>

using namespace std;

//weather api {https://openweathermap.org/forecast5}
//1. 위도 경도  2. 도시 이름 3. 우편 번호

int main() {
    CURL *curl = curl_easy_init();
    string url = "https://api.openweathermap.org/data/2.5/weather?q=Seoul&appid=b1e668504d0535c4e1cc989cdcb31fa2";
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


    cout << weatherData << endl;
    return 0;
}
