/* Конфигурация гексапода */

#ifndef CONFIG_H
#define GONFIG_H

#define COXA_LENGTH  80     // длина плеча в мллиметрах
#define FEMORA_LENGTH 80    // бедра плеча в мллиметрах
#define TIBIA_LENGTH 120    // длина глени в мллиметрах
#define TIBIA_OFFSET 40     // длина смещения плеча в мллиметрах относительно узла наклона
  
#define COXA_ANGLE_0 90     // угол среднего (нулевого) положения плеча
#define FEMORA_ANGLE_0 90   // угол среднего (нулевого) положения бедра
#define TIBIA_ANGLE_0 90    // угол среднего (нулевого) положения голени

#define COXA_ANGLE_INVERSE false   // признак инверсии направления вращения плечевого серва
#define FEMORA_ANGLE_INVERSE false  // признак инверсии направления вращения серва бедра
#define TIBIA_ANGLE_INVERSE true  // признак инверсии направления вращения серва голени

#define COXA_MIN_ANGLE -80    // минимальное отклонение плеча от среднего положения
#define COXA_MAX_ANGLE 80     // максимальное отклонение плеча от среднего положения

#define FEMORA_MIN_ANGLE -80  // минимальное отклонение бедра от среднего положения
#define FEMORA_MAX_ANGLE 80   // максимальное отклонение бедра от среднего положения

#define TIBIA_MIN_ANGLE -80   // минимальное отклонение голени от среднего положения
#define TIBIA_MAX_ANGLE 80    // максимальное отклонение голени от среднего положения
  
// распиновка сервоприводов (физическое подключение каждого сервопривода к сервошилду)
#define LEFT_FRONT_FOOT_COXA_PIN    0
#define LEFT_FRONT_FOOT_FEMORA_PIN  1
#define LEFT_FRONT_FOOT_TIBIA_PIN   2
#define LEFT_MIDLE_FOOT_COXA_PIN    3
#define LEFT_MIDLE_FOOT_FEMORA_PIN  4
#define LEFT_MIDLE_FOOT_TIBIA_PIN   5
#define LEFT_BACK_FOOT_COXA_PIN     6
#define LEFT_BACK_FOOT_FEMORA_PIN   7
#define LEFT_BACK_FOOT_TIBIA_PIN    8
#define RIGTH_FRONT_FOOT_COXA_PIN   9
#define RIGTH_FRONT_FOOT_FEMORA_PIN 10
#define RIGTH_FRONT_FOOT_TIBIA_PIN  11
#define RIGTH_MIDLE_FOOT_COXA_PIN   12
#define RIGTH_MIDLE_FOOT_FEMORA_PIN 13
#define RIGTH_MIDLE_FOOT_TIBIA_PIN  14
#define RIGTH_BACK_FOOT_COXA_PIN    15
#define RIGTH_BACK_FOOT_FEMORA_PIN  16
#define RIGTH_BACK_FOOT_TIBIA_PIN   17

// предельные максимальные значения углов наклона бедра и голени
// {FEMORA_MAX_ANGLE, TIBIA_MAX_ANGLE} не является ограничением, а служит признаком конца массива
#define COMPLEX_ANGLE_LIMITS_1 { {-80, -53}, {-75, -57}, {-70, -62}, {-65, -67}, {-60, -71}, {-55, -73}, {-50, -75}, {-45, -76}, {-40, -79}, {-30, -80}, {FEMORA_MAX_ANGLE, TIBIA_MAX_ANGLE} }

#define MIDLE_COXA_MIN_ANGLE -45    // минимальное отклонение среднего (MIDLE) плеча от среднего положения
#define MIDLE_COXA_MAX_ANGLE 45     // максимальное отклонение среднего (MIDLE) плеча от среднего положения

// предельные максимальные значения углов поворота смежных плеч (LEFT или RIGTH)
// LEFT: {FRONT, MIDLE, BACK}, ....
// RIGTH: {BACK, MIDLE, FRONT}, ....
// { .. , MIDLE_COXA_MAX_ANGLE, ..} не является ограничением, а служит признаком конца массива
#define COMPLEX_ANGLE_LIMITS_2 { {-30, -MIDLE_COXA_MIN_ANGLE, -45}, {-25, -40, -45}, {-20, -35, -45}, {-10, -30, -45}, {-5, -25, -45}, {5, -20, -40}, {15, -15, -35}, {20, -10, -30}, {25, -5, -25}, {30, 0, -20},\
                                  {35, 5, -15}, {40, 10, -10}, {40, 15, 0}, {45, 20, 10}, {45, 25, 15}, {45, 30, 20}, {45, 35, 20}, {45, 40, 30}, {45, MIDLE_COXA_MAX_ANGLE, 35} }
                                  
// разность значеий углов правого и левого передних (RIGTH_FRONT_FOOT_COXA_VALUE - LEFT_FRONT_FOOT_COXA_VALUE)
// или левого и правого задних углов (LEFT_FRONT_FOOT_COXA_VALUE - RIGTH_FRONT_FOOT_COXA_VALUE)
// не должна превышать COMPLEX_COXA_ANGLE_LIMIT
#define COMPLEX_COXA_ANGLE_LIMIT 145

#define LEFT_RIGTH_FRONT_DISTANCE 92
#define LEFT_RIGTH_MIDLE_DISTANCE 118
#define LEFT_RIGTH_BACK_DISTANCE LEFT_RIGTH_FRONT_DISTANCE
#define FRONT_BACK_DISTANCE 116

// положение лап относительно центра координат робота
#define LEFT_FRONT_FOOT_POSITION { FRONT_BACK_DISTANCE >> 1, LEFT_RIGTH_FRONT_DISTANCE >> 1, 0}
#define LEFT_MIDLE_FOOT_POSITION { 0, LEFT_RIGTH_MIDLE_DISTANCE >> 1, 0}
#define LEFT_BACK_FOOT_POSITION { -(FRONT_BACK_DISTANCE >> 1), LEFT_RIGTH_BACK_DISTANCE >> 1, 0}
#define RIGTH_FRONT_FOOT_POSITION { FRONT_BACK_DISTANCE >> 1, -(LEFT_RIGTH_FRONT_DISTANCE >> 1), 0}
#define RIGTH_MIDLE_FOOT_POSITION { 0, -(LEFT_RIGTH_MIDLE_DISTANCE >> 1), 0}
#define RIGTH_BACK_FOOT_POSITION { -(FRONT_BACK_DISTANCE >> 1), -(LEFT_RIGTH_BACK_DISTANCE >> 1), 0}

// углы поворота лапы относительно центра робота
#define LEFT_FRONT_FOOT_ROTATION {0,0,60}
#define LEFT_MIDLE_FOOT_ROTATION {0,0,90}
#define LEFT_BACK_FOOT_ROTATION {0,0,120}
#define RIGTH_FRONT_FOOT_ROTATION {0,0,-60}
#define RIGTH_MIDLE_FOOT_ROTATION {0,0,-90}
#define RIGTH_BACK_FOOT_ROTATION {0,0,-120}

#define MOTION_JOB_PERIOD 50
#define MOVE_STEP 80
#define ROTATE_STEP 20

#endif
