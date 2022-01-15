
#include "gobot_house.h"
#include "Arduino.h"

void GobotHouse::Setup(RobotAction* pAction, int segments){
	this->__house_action = pAction;
	__segments = segments;
	__map.setup();
}


// Head is a position name, The 5 bar arm will pick up stone from there.
void GobotHouse::MoveStone_FromRoomToHead(uint8_t room_id){
  __Move_fromNeck_toDoor(room_id,true);
  __Move_fromRoom_toDoor(room_id,false);
  __Enable_eefCoil(true);
  __Move_fromRoom_toDoor(room_id, true);
  __Move_fromNeck_toDoor(0, false);
  __Move_fromHead_toNeck(false);
  __Enable_eefCoil(false);
  __Move_fromHead_toNeck(true);
}

void GobotHouse::MoveStone_FromHeadToRoom(uint8_t room_id){
  __Move_fromNeck_toDoor(0, false);  // 0 is useless.
  __Move_fromHead_toNeck(false);
  __Enable_eefCoil(true);
  __Move_fromHead_toNeck(true);
  __Move_fromNeck_toDoor(room_id, true);
  __Move_fromRoom_toDoor(room_id, false);
  __Enable_eefCoil(false);
  __Move_fromRoom_toDoor(room_id, true);
}

void GobotHouse::__Enable_eefCoil(bool enable){

}

void GobotHouse::__Move_fromHead_toNeck(bool forwarding){
  float x1 = this->__map.head.x;
  float x2 = this->__map.neck.x;
  if(!forwarding){
	//from neck to head
	x1 = this->__map.neck.x;
	x2 = this->__map.head.x;
  }
  float distance = x2 - x1;
  float dx = distance / this->__segments ;
  for(int segment= 0; segment < this->__segments; segment++){
	float x = x1 + dx * segment;
	// MoveTo(x, 0);
	String strMoveToZero="G1A0";
	this->_commandQueue->AppendGcodeCommand(strMoveToZero);
  }
}

void GobotHouse::__Move_fromRoom_toDoor(uint8_t room_id, bool forwarding){
	//from room to door, now is at room.
	float x1 = __map.rooms[room_id].x;
	float y1 = __map.rooms[room_id].y;
	float x2 = __map.doors[room_id].x;
	float y2 = __map.doors[room_id].y;

	if (!forwarding){
		//from door to room, now is at door.
		x1 = __map.doors[room_id].x;
		y1 = __map.doors[room_id].y;
		x2 = __map.rooms[room_id].x;
		y2 = __map.rooms[room_id].y;
	}

	// float distance = __map.distance_room_to_door[room_id];
	float dx = (x2 - x1) / __segments;
	float dy = (y2 - y1) / __segments;
	for(int segment= 0; segment < __segments; segment++){
		float x = x1 + dx * segment;
		float y = y1 + dy * segment;
		// MoveTo(x,y);
		String strGcode="G1A";
		strGcode.concat(x);
		strGcode.concat("B");
		strGcode.concat(y);
		this->_commandQueue->AppendGcodeCommand(strGcode);
	}
}

// This is almost a  rotation, because beta should be no changing.
void GobotHouse::__Move_fromNeck_toDoor(uint8_t room_id, bool forwarding){
	float x = __map.doors[room_id].x;
	float y = __map.doors[room_id].y;
	if (!forwarding){
		x = __map.neck.x;
		y = __map.neck.y;    
	}
	String strGcode="G1A";
	strGcode.concat(x);
	strGcode.concat("B");
	strGcode.concat(y);
	this->_commandQueue->AppendGcodeCommand(strGcode);
}