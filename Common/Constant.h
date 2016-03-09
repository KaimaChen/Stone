#ifndef CONSTANT_H
#define CONSTANT_H

#include "float.h"

const int WIDTH               = 600; 
const int HEIGHT              = 600;
const float G                 = -9.8;
const float PI                = 3.1415926536f;
const int LINE_NUMS_IN_CIRCLE = 60; //圆含有的边数
const int FRAME_TIME          = 16; //一帧的毫秒数
const float PRECITION         = 0.0001; //精度
const float FRAME_TIME_SECOND = FRAME_TIME / 1000.0f;
const int MAX_POLYGON_VERTEX  = 16;

//Menu ID
const int CLEAR = 1;    //clear screen
const int CREATE = 2;   //create rigidbody
const int RAY_CAST = 3; //ray casting
const int PICK = 4; 	//pick rigidbody
const int ROD = 5; 		//make rod between rigidbodies
const int CABLE = 6; 	//make cable between rigidbodies

#endif // CONSTANT_H
