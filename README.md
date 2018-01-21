# THEDC2016
2016年电设代码，模块比较齐全

## 代码分四层
- 策略层/PL
  - `User/Decision.*`
  - 战斗状态/轨迹策略等
- 执行层/EL
  - `Exec/*`
  - 音乐(MUSIC)
    - 顾名思义
  - 轨迹坐标(POINTS)
    - 设置/实现按照一个坐标点序列行走的功能
  - 战场信息(INF)
    - 各类统计信息
- 运算层/CL
  - `Calc/*`
  - 音调(TONE)
    - 通过调节PWM频率实现
  - 速度控制(SPEED)
    - 目标速度和当前ADC输入进行比对并控制PWM
  - 完整通信数据(COM)
    - 对UART数据进行解析
  - 角度信息(ANGLE)
    - 角速度积分及根据通信数据进行校准
  - 地图信息(MAP)
    - 记录红外结果，生成地图
- 驱动层/DL
  - `Driver/*`
  - PWM(PWM)
  - 速度反馈(ADC)
    - PCB上设计反电动势输出，通过ADC输入
  - UART(UART)
  - 角速度(IMU)
    - IIC+MPU6050
  - 红外检测(IR)
    - 场地黑白情况

## 层次关系
![依赖关系](https://github.com/wengzhe/THEDC2016/raw/master/依赖.png)