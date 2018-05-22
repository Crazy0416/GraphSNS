# SNSAnalysisApp

## 설명

DBLP 논문 사이트에서 논문을 낸 사람과 협업자들, 논문을 낸 날짜, 컨퍼런스 등을 데이터로 하는 그래프를 시각화하여 논문을 작성한 자들과의 연관성을 보여주는 프로그램입니다.

직접 DBLP에서 크롤링하진 않고 txt 파일에 데이터가 담겨져 있고 데이터를 가지고 다양한 자료구조 혹은 알고리즘을 활용해 다양한 데이터를 시각화하는 프로그램입니다.

Qt 프레임워크에 c++을 사용하여 제작하였습니다.

## Dependency

- [QT 5.10.1](https://www.qt.io/download) open source
- msvc14.1 (Windows10 Visual Studio 2017 환경)
- [Boost v 1.67.0](https://sourceforge.net/projects/boost/files/boost/1.67.0/)
- x64 (64bit)



## 시연영상

(유튜브 영상)[]



## 구현한 기능

- boost library를 활용한 랜덤한 위치의 그래프. (제공받은 코드 활용)
- Qt GUI를 활용해 그래프를 시각화
- 그래프 뮤텍스
- QTimer를 활용해 10초마다 txt 파일 업데이트 확인 및 그래프 업데이트
- 특정한 author 찾기 및 노드 이미지 강조
- QGraphicsItem 마우스 이벤트로 특정 노드의 정보 표시
- 힙트리를 이용해 연결점이 많은 노드 Top k 표시
- Bfs를 이용한 author 간의 shortest path 표시
- 년도, 컨퍼런스를 필터링하여 특정 노드들만 표시
- 구독한 author가 txt 파일에 추가되면 업데이트 시 구독자에게 알람.

