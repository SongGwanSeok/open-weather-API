# Open Weather API


<img width="1440" alt="weather main" src="https://github.com/SongGwanSeok/Weather/assets/105411445/750dbb65-32bc-4f23-bc15-fc422488ce77">


<br/>

## 프로그램 설명 

이 프로그램은 기상정보 open api인 open weather map api를 가져와 간단하게 사용할 수 있도록 만든 프로그램입니다.

3가지 기능을 구현했으며, [1. 현재 날씨 2. 일기 예보 3. 미세 먼지] 세가지 기능이 있습니다.

현재 날씨는 3가지 정보로 검색이 가능합니다. 위도와 경도, 도시 이름, 우편 번호로 현재 날씨를 검색합니다.

일기 예보와 미세 먼지는 도시 이름으로만 검색이 가능합니다.

<br/>

## 사용한 언어 / 라이브러리 / API
<div align=center>
  <img src="https://img.shields.io/badge/c++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white">
  <img src="https://img.shields.io/badge/curl-007396?style=for-the-badge&logo=curl&logoColor=white">
  <img src="https://img.shields.io/badge/rapidjson-3776AB?style=for-the-badge&logo=json&logoColor=white">
  <br/> 
  open geocoding API
  https://nominatim.org/
  
  oepn weather map API
  <https://openweathermap.org/>
  
  Licence: Data © OpenStreetMap contributors, ODbL 1.0. 
  <https://osm.org/copyright>
</div>

<br/>

## 프로그램 가이드

리눅스
```
1. 파일 다운
  $ wget [원하는 버전의 파일 다운로드 링크]

2. libcurl 설치
  $ apt-get install libcurl4-openssl-dev
  
3. 압축 해제
  tar.gz 파일
  $ tar -zxvf [다운 받은 tar 파일]
  
  zip 파일
  $ unzip [다운 받은 zip 파일]
  
4. 실행 파일 만들기 
  $ g++ -o weather main.cpp -lcurl
  
5. 실행
  $ ./weather
```

<br/>

## 기능 소개

1. 현재 날씨

    a. 위도, 경도 : 위도, 경도 정보를 입력하면 현재 날씨 정보를 반환받는다.
    
    <img width="523" alt="1-1" src="https://github.com/SongGwanSeok/Weather/assets/105411445/9f5f4d02-fb07-42bb-8283-d1a438ce6497">
    
    b. 도시 이름 : 도시 이름을 입력하면 현재 날씨 정보를 반환받는다.
    
    <img width="523" alt="1-2" src="https://github.com/SongGwanSeok/Weather/assets/105411445/fca097eb-0599-434c-ba20-2352dbf69fc8">

    c. 우편 번호 : 우편 번호를 입력하면 현재 날씨 정보를 반환받는다.
    
    <img width="523" alt="1-3" src="https://github.com/SongGwanSeok/Weather/assets/105411445/5c67e98e-db5b-4ab7-a5b8-882b5e8ab0fc">

2. 5일치 일기 예보 : 도시 이름을 입력하면 5일치 일기 예보를 반환받는다.

    <img width="523" alt="2-1" src="https://github.com/SongGwanSeok/Weather/assets/105411445/158205bb-097f-4a38-9778-ebfcf4f54179">
    
    <img width="523" alt="2-2" src="https://github.com/SongGwanSeok/Weather/assets/105411445/bc48bb8b-7f86-4aa8-9ff8-c067f171f76f">

3. 미세먼지 : 도시 이름을 입력하면 현재 미세먼지 정보를 반환받는다.

    <img width="523" alt="3" src="https://github.com/SongGwanSeok/Weather/assets/105411445/e55292ba-aaac-4ce0-aaed-cc5e4b8a5c09">

4. 종료 : 프로그램을 종료시킨다.


## 
    

