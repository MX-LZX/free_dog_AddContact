const int QIYA_PIN_1 = A5;  // 气压传感器1（LF）连接到A0
const int QIYA_PIN_2 = A3;  // 气压传感器2（LR）连接到A1
const int QIYA_PIN_3 = A4;  // 气压传感器3（RF）连接到A2
const int QIYA_PIN_4 = A2;  // 气压传感器4（RR）连接到A3

// 设置参考电压为5V
const float VCC = 5000.0;  // 参考电压为5V（单位：mV）

// 校准参数：根据实际传感器测得的零点电压和满量程电压进行调整
const float Voltage_0 = 160;   // 零点电压值（mV），根据实际情况修改
const float Voltage_40 = 4000; // 满量程输出电压值（mV），根据实际情况修改

// 滑动平均滤波器的窗口大小
const int windowSize = 5;  // 使用较小窗口，保证更高实时性

// 存储每个传感器的历史值
int readings_1[windowSize];
int readings_2[windowSize];
int readings_3[windowSize];
int readings_4[windowSize];

// 当前读数索引
int index = 0;

void setup() 
{
  // 启动串口通信，波特率设置为115200
  Serial.begin(115200);

  // 设置气压传感器接口为输入
  pinMode(QIYA_PIN_1, INPUT);
  pinMode(QIYA_PIN_2, INPUT);
  pinMode(QIYA_PIN_3, INPUT);
  pinMode(QIYA_PIN_4, INPUT);

  // 初始化所有读数为0
  for (int i = 0; i < windowSize; i++) {
    readings_1[i] = 0;
    readings_2[i] = 0;
    readings_3[i] = 0;
    readings_4[i] = 0;
  }
}

void loop() 
{
  // 获取新的读取值
  int P_ADC_1 = analogRead(QIYA_PIN_1);
  int P_ADC_2 = analogRead(QIYA_PIN_2);
  int P_ADC_3 = analogRead(QIYA_PIN_3);
  int P_ADC_4 = analogRead(QIYA_PIN_4);

  // 更新滑动平均数组
  readings_1[index] = P_ADC_1;
  readings_2[index] = P_ADC_2;
  readings_3[index] = P_ADC_3;
  readings_4[index] = P_ADC_4;

  // 计算当前窗口内的平均值
  int sum_1 = 0, sum_2 = 0, sum_3 = 0, sum_4 = 0;
  for (int i = 0; i < windowSize; i++) {
    sum_1 += readings_1[i];
    sum_2 += readings_2[i];
    sum_3 += readings_3[i];
    sum_4 += readings_4[i];
  }

  // 计算平均电压
  float Pressure_V_1 = sum_1 / windowSize * VCC / 1024.0;
  float Pressure_V_2 = sum_2 / windowSize * VCC / 1024.0;
  float Pressure_V_3 = sum_3 / windowSize * VCC / 1024.0;
  float Pressure_V_4 = sum_4 / windowSize * VCC / 1024.0;

  // 根据电压值计算气压值，并映射到0到100的范围
  uint16_t pressure_1 = map(Pressure_V_1, Voltage_0, Voltage_40, 0, 100);  // 映射到0-100范围，并转换为uint16_t
  uint16_t pressure_2 = map(Pressure_V_2, Voltage_0, Voltage_40, 0, 100);
  uint16_t pressure_3 = map(Pressure_V_3, Voltage_0, Voltage_40, 0, 100);
  uint16_t pressure_4 = map(Pressure_V_4, Voltage_0, Voltage_40, 0, 100);

  // 获取当前时间戳（毫秒）
  unsigned long currentTime = millis();  // 获取运行时间，单位毫秒

  // 串口输出格式：RF,LF,RR,LR
  Serial.print(pressure_3 + 24);  // 输出RF传感器数据
  Serial.print(",");
  Serial.print(pressure_1 + 24);  // 输出LF传感器数据
  Serial.print(",");
  Serial.print(pressure_4 + 24);  // 输出RR传感器数据
  Serial.print(",");
  Serial.println(pressure_2 + 24);  // 输出LR传感器数据
  
  // 更新索引，形成滑动窗口
  index++;
  if (index >= windowSize) {
    index = 0;  // 如果超出窗口大小，回到0，形成循环
  }

  // 延时5ms，保证一定的采样速度
  delay(5);
}
