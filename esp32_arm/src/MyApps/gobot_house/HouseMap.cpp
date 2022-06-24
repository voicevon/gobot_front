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
    this->neck.Y = 0;
    // this->neck.X = 55.0;
    this->neck.X = 65.0;
    this->head.Y = 0;
    this->head.X = 148.93;

    this->rooms[0].X = -78.79;
    this->rooms[0].Y = 126.38;
    this->rooms[1].X = -98.18;
    this->rooms[1].Y = 95.34;
    this->rooms[2].X = -111.9;
    this->rooms[2].Y = 61.4;
    this->rooms[3].X = -119.51;
    this->rooms[3].Y = 25.59;

    this->rooms[4].X = this->rooms[3].X;
    this->rooms[4].Y = - this->rooms[3].Y;
    this->rooms[5].X = this->rooms[2].X;
    this->rooms[5].Y = - this->rooms[2].Y;
    this->rooms[6].X = this->rooms[1].X;
    this->rooms[6].Y = - this->rooms[1].Y;
    this->rooms[7].X = this->rooms[0].X;
    this->rooms[7].Y = - this->rooms[0].Y;

    // this->gates[0].X = -10.37;
    // this->gates[0].Y = 59.07;
    // this->gates[1].X = -21.39;
    // this->gates[1].Y = 56.12;
    // this->gates[2].X = -44.28;
    // this->gates[2].Y = 40.67;
    // this->gates[3].X = -57.42;
    // this->gates[3].Y = 17.95;


    this->gates[4].X = -3;
    this->gates[4].Y = -65;
    this->gates[5].X = -27;
    this->gates[5].Y = -59;
    this->gates[6].X = -49;
    this->gates[6].Y = -42;
    this->gates[7].X = -62;
    this->gates[7].Y = -19;

    this->gates[3].X = -62;
    this->gates[3].Y = 19;
    this->gates[0].X = this->gates[3].X;
    this->gates[0].Y = this->gates[3].Y;
    this->gates[1].X = this->gates[3].X;
    this->gates[1].Y = this->gates[3].Y;
    this->gates[2].X = this->gates[3].X;
    this->gates[2].Y = this->gates[3].Y;



    this->doors[4].X = -51;
    this->doors[4].Y = -105;
    this->doors[5].X = -67;
    this->doors[5].Y = -89;
    this->doors[6].X = -78;
    this->doors[6].Y = -51;
    this->doors[7].X = -85;
    this->doors[7].Y = -21;
    
    this->doors[0].X = this->doors[4].X;
    this->doors[0].Y = - this->doors[4].Y;
    this->doors[1].X = this->doors[5].X;
    this->doors[1].Y = - this->doors[5].Y;
    this->doors[2].X = this->doors[6].X;
    this->doors[2].Y = - this->doors[6].Y;
    this->doors[3].X = this->doors[7].X;
    this->doors[3].Y = - this->doors[7].Y;
}

