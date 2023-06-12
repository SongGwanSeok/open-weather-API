#include <iostream>

#include <string>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

using namespace std;

string displayWeatherInfo(const string& weatherData){
    // JSON 파싱
    Document data;
    data.Parse(weatherData.c_str());

    if(!data.HasParseError()) { // 파싱 오류 체크
        if(data.IsObject()) { // JSON 문서가 객체 형식인지 확인
            if(data.HasMember("name") && data["name"].IsString()){
                string cityName = data["name"].GetString();
                cout << "도시명: " << cityName << endl;
            }
            if(data.HasMember("main") && data["main"].IsString()){
                const Value& mainObj = data["main"];
                if (mainObj.HasMember("temp") && mainObj["temp"].IsNumber()) {
                    double temperature = mainObj["temp"].GetDouble();
                    std::cout << "온도: " << temperature << "°C" << std::endl;
                }

                if (mainObj.HasMember("humidity") && mainObj["humidity"].IsNumber()) {
                    int humidity = mainObj["humidity"].GetInt();
                    std::cout << "습도: " << humidity << "%" << std::endl;
                }
            }
            if(data.HasMember("wind") && data["wind"].IsString()){
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
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}


//json data seoul
//{
//   "coord":{  경도와 위도 -> coord
//      "lon":126.9778,
//      "lat":37.5683
//   },
//   "weather":[  날씨 정보
//      {
//         "id":803,
//         "main":"Clouds",
//         "description":"broken clouds",
//         "icon":"04d"
//      }
//   ],
//   "base":"stations",
//   "main":{
//      "temp":299.81, // 온도
//      "feels_like":299.81, // 체감온도
//      "temp_min":295.84, // 최소 온도
//      "temp_max":299.81, // 최대 온도
//      "pressure":1010, // 대기압
//      "humidity":42, // 습도
//      "sea_level":1010, // 해수면
//      "grnd_level":1003 // 지표면
//   },
//   "visibility":10000, // 가시성
//   "wind":{ // 바람
//      "speed":6.97, // 풍속
//      "deg":261, // 풍향
//      "gust":10.29 //돌풍
//   },
//   "clouds":{ //구름
//      "all":74
//   },
//   "dt":1685948444,
//   "sys":{
//      "type":1,
//      "id":5509,
//      "country":"KR", // 도시 코드
//      "sunrise":1685909496, // 일출 시간
//      "sunset":1685962177 // 일몰 시간
//   },
//   "timezone":32400,
//   "id":1835848,
//   "name":"Seoul", // 도시 이름
//   "cod":200
//}