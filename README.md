#실시간 운영체제 과제 Myshell
=================

## 1. 프로젝트 소개

아래 코드를 기반으로 shell 개발.

https://github.com/jongchank/syssw/blob/master/13_process_management/toy_shell.c

필수 구현 요소

1. exit 입력시 shell 종료

2. a; b; c 와 같이 입력시 a, b, c 세개의 프로그램 차례대로 실행

3. username@hostname$ 형식의 shell prompt. username과 hostname 얻는 방법은 아래 코드 참조

https://github.com/jongchank/syssw/blob/master/13_process_management/gethostusername.c

이 외에 구현하고자 하는 기능은 자유롭게 구현



## 2. 추가한 기능 

1. 사용자로부터 입력받을경우 입력받은 프로그램의 수와 그 순서를 출력해준다.

2. 각프로그램별 실행시간을 측정하고 총 실행시간을 합산하여 보여주는 함수를 추가하였습니다. (이미지 추가)


## <실행화면 예시>
<img src =./pic/os_3.jpg>
<img src =./pic/os_4.jpg>
