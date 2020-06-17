# yuv-player
The player using OpenGL shader for converting YUV to RGB

## 설치 및 빌드 방법
```bash
git clone https://github.com/moonyl/yuv-player.git
cd yuv-player
git submodule init
git submodule update
mkdir build
cd build
cmake ..
```

build 디렉토리에 yuv-player.sln 파일이 생성되어 있을 겁니다.
visual studio 에서 오픈하고, 빌드하시면 됩니다.

디버깅 시에 시작 프로젝트로 yuv-player 를 설정하는 것을 잊지 마세요.
