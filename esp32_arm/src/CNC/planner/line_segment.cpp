#include "line_segment.h"


void LineSegment::Calculate_distance_time(FKPosition_XYZRPY* start_position){
    this->vector_distance = 123;
    this->required_time = vector_distance / Speed;
}

void LineSegment::DeepCopyTo(LineSegment* the_copy){
    // Logger::Debug("LineSegment::DeepCopyTo()");
    the_copy->TargetPosition.X = this->TargetPosition.X;
    the_copy->TargetPosition.Y = this->TargetPosition.Y;
    the_copy->TargetPosition.Z = this->TargetPosition.Z;
    the_copy->TargetPosition.Roll = this->TargetPosition.Roll;
    the_copy->TargetPosition.Pitch = this->TargetPosition.Pitch;
    the_copy->TargetPosition.Yaw = this->TargetPosition.Yaw;
    Logger::Print("LineSegment::DeepCopyTo() point", 99);
}

void LineSegment::DeepCopyFromFkPosition(FKPosition_XYZRPY* from){
    this->TargetPosition.X = from->X;
    this->TargetPosition.Y = from->Y;
    this->TargetPosition.Z = from->Z;
    this->TargetPosition.Roll = from->Roll;
    this->TargetPosition.Pitch = from->Pitch;
    this->TargetPosition.Yaw = from->Yaw;
}

void LineSegment::PrintOUt(){
    Logger::Debug("LineSegment::PrintOUt()");
    Logger::Print("TargetPosition.X",this->TargetPosition.X);
    Logger::Print("TargetPosition.Y",this->TargetPosition.Y);
    Logger::Print("TargetPosition.Z",this->TargetPosition.Z);
    Logger::Print("TargetPosition.Roll",this->TargetPosition.Roll);
    Logger::Print("TargetPosition.Pitch",this->TargetPosition.Pitch);
    Logger::Print("TargetPosition.Yaw",this->TargetPosition.Yaw);
}
