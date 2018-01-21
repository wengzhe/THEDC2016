# THEDC2016
2016年电设代码，模块比较齐全

## 代码分四层
- 策略层/PL
  - 战斗状态/轨迹策略等
  - `User/Decision.*`
- 执行层/EL
  - 音乐(MUSIC)
  - 设置/实现轨迹坐标(POINTS)
  - 战场信息(INF)
  - `Exec/*`
- 运算层/CL
  - 音调(TONE)
  - 速度控制(SPEED)
  - 完整通信数据(COM)
  - 角度信息(ANGLE)
  - 地图信息(MAP)
  - `Calc/*`
- 驱动层/DL
  - PWM(PWM)
  - 速度(ADC)
  - UART(UART)
  - 角速度(IMU)
  - 黑白情况(IR)
  - `Calc/*`

## 层次关系
![依赖关系](https://github.com/wengzhe/THEDC2016/raw/master/依赖.png)