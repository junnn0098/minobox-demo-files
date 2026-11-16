# minobox-demo-files

minoBox 시연용 **무해한** 파일입니다. 실제 악성 코드가 아닙니다.

## QuoteViewer_Setup.exe

"거래처 메일로 받은 견적서 뷰어 설치 파일" 역할을 하는 데모 프로그램입니다. 서명이 없어서 minoBox는 이 파일을 내 PC가 아니라
격리 공간(Windows Sandbox 또는 micro VM)에서만 실행합니다.

- **아무것도 설치하지 않습니다.** 5초 동안 `installing...`을 출력합니다.
- 자기가 실행된 **컴퓨터 이름과 사용자 이름**을 출력하고, 같은 내용의 `quote_check_report.txt`를 minoBox 격리 공간의
  Output 폴더(`C:\RunnerWorkspace\Sessions\...\output`)에 남깁니다. 그 폴더가 없으면 현재 폴더에 남깁니다.
- 네트워크 접속, 레지스트리 변경, 다른 파일 수정은 하지 않습니다.

## PcCheck.exe

"GitHub에서 찾은 PC 점검 도구" 역할을 하는 데모 프로그램입니다. 서명이 없어서 minoBox는 격리 공간에서만 실행합니다.

- 아무것도 설치할 필요가 없는 실행 파일 하나입니다(Windows 기본 구성 요소만 사용).
- 실행된 컴퓨터의 이름, 사용자, Windows 버전, CPU 코어 수, 메모리, 켜진 시간을 **읽어서 출력만** 합니다.
  파일 쓰기, 네트워크 접속, 설정 변경은 하지 않습니다.
- Windows Sandbox에서 실행되면 사용자가 `WDAGUtilityAccount`로 나옵니다.

## 소스와 빌드

소스는 [`src/QuoteViewer_Setup.c`](src/QuoteViewer_Setup.c), [`src/PcCheck.c`](src/PcCheck.c)이며, Release의 exe는 이 소스를 MSYS2 UCRT64 gcc로 빌드한 것입니다.

```
gcc -O2 -s -static -o QuoteViewer_Setup.exe src/QuoteViewer_Setup.c
gcc -O2 -s -static -o PcCheck.exe src/PcCheck.c
```

다운로드: [Releases](../../releases/latest)
