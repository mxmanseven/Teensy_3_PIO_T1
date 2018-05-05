#include "SpeedInterval.h"
#include "Arduino.h"
#include <string.h>

/// Test the retrival of GetMilesAndMinutesBySpeed().
void SiTest()
{
  SpeedInterval si;
  
  int speed = 6;
  float miles = 0;
  int minutes = 0;

  Serial.begin(9600);
  
  for(int s = 0; s < 99; s++)
  {
    si.GetMilesAndMinutesBySpeed(s, miles, minutes);
    Serial.print("speed " + String(s) + " miles " + String(miles) + " minutes " + String(minutes) + "\r\n");
  }
}

int SpeedInterval::GetMilesAndMinutesBySpeed(
  int speed, 
  float& miles,
  int& minutes)
{
  miles = 0.0;
  minutes = 0;
  
  if (speed > 99 || speed < 0) return -1;
  
  miles = SpeedInterval::MileTehthssAndMinutesBySpeed[speed][0] / 10.0;
  minutes = SpeedInterval::MileTehthssAndMinutesBySpeed[speed][1];

  return 0;
}


// first index is speed
// first value at second index is tenth miles between possiables at this speed
// second value is minutes between possiables at this speed
uint8_t SpeedInterval::MileTehthssAndMinutesBySpeed[][2] =
{ // tenth mile, minutes
  {0,0},  //0
  {1,6},  //1
  {1,3},  //2
  {1,2},  //3
  {2,3},  //4
  {5,6},  //5
  {1,1},  //6
  {21,18},  //7
  {4,3},  //8
  {3,2},  //9
  {5,3},  //10
  {11,6}, //11
  {2,1},  //12
  {13,6}, //13
  {21,9}, //14
  {5,2},  //15
  {8,3},  //16
  {51,18},  //17
  {3,1},  //18
  {57,18},  //19
  {10,3}, //20
  {7,2},  //21
  {11,3}, //22
  {69,18},  //23
  {4,1},  //24
  {25,6}, //25
  {13,3}, //26
  {9,2},  //27
  {42,9}, //28
  {87,18},  //29
  {5,1},  //30
  {31,6}, //31
  {16,3}, //32
  {11,2}, //33
  {51,9}, //34
  {35,6}, //35
  {6,1},  //36
  {37,6}, //37
  {57,9}, //38
  {13,2}, //39
  {20,3}, //40
  {0,0},  //41
  {7,1},  //42
  {0,0},  //43
  {22,3}, //44
  {15,2}, //45
  {69,9}, //46
  {47,6}, //47
  {8,1},  //48
  {49,6}, //49
  {25,3}, //50
  {17,2}, //51
  {26,3}, //52
  {0,0},  //53
  {9,1},  //54
  {55,6}, //55
  {84,9}, //56
  {19,2}, //57
  {87,9}, //58
  {59,6}, //59
  {10,1}, //60
  {0,0},  //61
  {31,3}, //62
  {21,2}, //63
  {32,3}, //64
  {65,6}, //65
  {11,1}, //66
  {67,6}, //67
  {0,0},  //68
  {23,2}, //69
  {35,3}, //70
  {0,0},  //71
  {12,1}, //72
  {0,0},  //73
  {37,3}, //74
  {25,2}, //75
  {0,0},  //76
  {77,6}, //77
  {13,1}, //78
  {79,6}, //79
  {40,3}, //80
  {27,2}, //81
  {0,0},  //82
  {83,6}, //83
  {14,1}, //84
  {85,6}, //85
  {0,0},  //86
  {29,2}, //87
  {44,3}, //88
  {89,6}, //89
  {15,1}, //90
  {0,0},  //91
  {0,0},  //92
  {31,2}, //93
  {47,3}, //94
  {95,6}, //95
  {16,1}, //96
  {0,0},  //97
  {49,3}, //98
  {33,2} //99
};



