# include "HouseMap.h"
#include <math.h>

/*

	House                Y+
						 ^
			 r0          |                                                                     \ beta  
		  r1     d0      |                                                                    \  /
		r2      d1       |                                                                     \/      
	  r3       d3  d2    |                                                                   /  alpha
	  -----------------(0,0)------Neck----------Head    --> X+                               --------------------------------->  Alpha,Beta (0,0)
	  r4      d4  d5     |
	   r5       d6       |
		 r6     d7       |
		  r7             |

*/

void HouseMap::Init(){
    this->neck.y = 0;
    // this->neck.x = 55.0;
    this->neck.x = 65.0;
    this->head.y = 0;
    this->head.x = 148.93;

    this->rooms[0].x = -78.79;
    this->rooms[0].y = 126.38;
    this->rooms[1].x = -98.18;
    this->rooms[1].y = 95.34;
    this->rooms[2].x = -111.9;
    this->rooms[2].y = 61.4;
    this->rooms[3].x = -119.51;
    this->rooms[3].y = 25.59;

    this->rooms[4].x = this->rooms[3].x;
    this->rooms[4].y = - this->rooms[3].y;
    this->rooms[5].x = this->rooms[2].x;
    this->rooms[5].y = - this->rooms[2].y;
    this->rooms[6].x = this->rooms[1].x;
    this->rooms[6].y = - this->rooms[1].y;
    this->rooms[7].x = this->rooms[0].x;
    this->rooms[7].y = - this->rooms[0].y;

    // this->gates[0].x = -10.37;
    // this->gates[0].y = 59.07;
    // this->gates[1].x = -21.39;
    // this->gates[1].y = 56.12;
    // this->gates[2].x = -44.28;
    // this->gates[2].y = 40.67;
    // this->gates[3].x = -57.42;
    // this->gates[3].y = 17.95;


    this->gates[4].x = -3;
    this->gates[4].y = -65;
    this->gates[5].x = -27;
    this->gates[5].y = -59;
    this->gates[6].x = -49;
    this->gates[6].y = -42;
    this->gates[7].x = -62;
    this->gates[7].y = -19;

    this->gates[3].x = -62;
    this->gates[3].y = 19;
    this->gates[0].x = this->gates[3].x;
    this->gates[0].y = this->gates[3].y;
    this->gates[1].x = this->gates[3].x;
    this->gates[1].y = this->gates[3].y;
    this->gates[2].x = this->gates[3].x;
    this->gates[2].y = this->gates[3].y;



    this->doors[4].x = -51;
    this->doors[4].y = -105;
    this->doors[5].x = -67;
    this->doors[5].y = -89;
    this->doors[6].x = -78;
    this->doors[6].y = -51;
    this->doors[7].x = -85;
    this->doors[7].y = -21;
    
    this->doors[0].x = this->doors[4].x;
    this->doors[0].y = - this->doors[4].y;
    this->doors[1].x = this->doors[5].x;
    this->doors[1].y = - this->doors[5].y;
    this->doors[2].x = this->doors[6].x;
    this->doors[2].y = - this->doors[6].y;
    this->doors[3].x = this->doors[7].x;
    this->doors[3].y = - this->doors[7].y;
}

