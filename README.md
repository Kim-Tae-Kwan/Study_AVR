# Study AVR
#### AVR atmega128의 타이머/카운터, PWM, 인터럽트, UART, ADC 등을 사용하여 LED, 스텝모터, LCD, Cds, 부저 등을 제어 해보았습니다.
#### UART 통신으로 터미널과 통신하며, 키보드 입력으로 메뉴에 맞는 하드웨어를 제어 할 수 있습니다.

> 1. L : LED가 순차적으로 점등하고 점멸합니다
> 2. C  : ADC로 Cds 데이터를 읽어 터미널 창에 출력하게 됩니다
> 3. 0-7 : 도~시까지 각 숫자에 따라 음계를 부저로 출력합니다
> 4. S : R(CCW), F(CW) 입력으로 스텝 모터를 정방향, 역방향으로 제어할 수 있습니다
> 5. Push button : 외부 인터럽트로써, 동작하고 있는 하드웨어를 정지시킬 수 있습니다


## 시연영상
![모형 앞](/readmeFile/Study_AVR_QRCode.png)

## 교육 키트 & 터미널
![모형 앞](/readmeFile/Study_AVR_Hardware.png)
![모형 앞](/readmeFile/Study_AVR_Terminal.png)

