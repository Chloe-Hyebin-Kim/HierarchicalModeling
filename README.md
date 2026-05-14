# HierarchicalModeling
https://stormy-airbus-32f.notion.site/Assignment-1-Hierarchical-Modeling-17f690c00943806dba77f04f65f77501



FK anim 구현.

## 실행 환경

* OS: Windows
* Language: C++
* IDE: Visual Studio
* freeglut :  https://www.transmissionzero.co.uk/software/freeglut-devel/
* glew :  http://glew.sourceforge.net/


## 문서 구조
```
HierarchicalModeling/
├── main.cpp // 프로그램 진입점, OpenGL window/callback 설정
├── HierarchicalModeling.h // HierarchicalModeling 클래스 선언
├── HierarchicalModeling.cpp  // HierarchicalModeling 클래스 구현
└── HierarchicalModeling.slnx // Visual Studio 솔루션 구성 파일
```


## 구현 개념

부모 좌표계의 변환 × 자식 좌표계의 변환 = 자식의 월드 좌표계 변환.<br>
Hierarchical modeling에서 각 관절의 local angle만 저장.<br>
실제 화면에 그릴 때는 부모 변환들을 전부 곱해서 global position/orientation을 계산.<br>

#### 로드리게스 회전행렬 비슷하게 -> 계산된 axis와 angle을 함수에 전달 ####


각 관절은 자신의 local transformation만 가지고 있으며, <br>
실제 화면에 그릴 때는 root node부터 현재 node까지의 변환을 순서대로 누적하여 global position과 orientation을 계산한다.

<img width="1327" height="1011" alt="image" src="https://github.com/user-attachments/assets/00cd0842-73b3-4602-8f37-e5f0b442994a" />



## 모델 설명
```
Base //바닥에 놓인 원기둥 받침대
 └── Neck Joint
      └── Neck Link  //Base 위에서 시작되는 세로 원기둥 링크
           └── Arm Joint
                └── Arm Link //Neck 끝 관절에서 회전하는 팔 형태의 원기둥 링크
                     └── Lamp Head Joint
                          └── Lamp Head //Arm 끝 관절에 연결된 절두원뿔 형태의 조명 갓
```



전체 변환은 부모에서 자식 방향으로 누적.
```
   T_head = T_base *T_neck_joint *T_neck_link *T_arm_joint *T_arm_link *T_head_joint *T_head_link
```



## 함수 설명
AlignYAxisToVector : local y축 방향으로 만들어진 primitive를 실제 link 방향으로 정렬.   <br>     
DrawCylinderY :항상 자신의 local y축 방향으로 원긷우 생성. <br>
DrawFloorGrid :3D 공간감을 확인하기 위한 바닥 격자. <br>





