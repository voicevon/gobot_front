#include "line_segment.h"


void LineSegment::Calculate_distance_time(FKPosition_XYZRPW* start_position){
    //TODO:: distance in 3D space.
    float dx = this->TargetPosition.X - start_position->X;  
    float dy = this->TargetPosition.Y - start_position->Y;
    this->Distance_mm = sqrtf(dx * dx + dy * dy);
    this->Required_time = Distance_mm / Speed_mm_per_second;
    Logger::Debug("LineSegment::Calculate_distance_time()");
    // Logger::Print("start_position->X",start_position->X);
    // Logger::Print("TargetPosition.X",TargetPosition.X);
    // Logger::Print("start_position->Y",start_position->Y);
    // Logger::Print("TargetPosition.Y",TargetPosition.Y);
    Logger::Print("Distance_mm", Distance_mm);
    Logger::Print("Required_time",Required_time);
}

void LineSegment::DeepCopyTo(LineSegment* the_copy){
    // Logger::Debug("LineSegment::DeepCopyTo()");
    the_copy->TargetPosition.X = this->TargetPosition.X;
    the_copy->TargetPosition.Y = this->TargetPosition.Y;
    the_copy->TargetPosition.Z = this->TargetPosition.Z;
    the_copy->TargetPosition.Roll = this->TargetPosition.Roll;
    the_copy->TargetPosition.Pitch = this->TargetPosition.Pitch;
    the_copy->TargetPosition.Yaw = this->TargetPosition.Yaw;
    // Logger::Print("LineSegment::DeepCopyTo() point", 99);
}

void LineSegment::DeepCopyFromFkPosition(FKPosition_XYZRPW* from){
    this->TargetPosition.X = from->X;
    this->TargetPosition.Y = from->Y;
    this->TargetPosition.Z = from->Z;
    this->TargetPosition.Roll = from->Roll;
    this->TargetPosition.Pitch = from->Pitch;
    this->TargetPosition.Yaw = from->Yaw;
}

void LineSegment::PrintOUt(const char* title){
    Logger::Debug(title);
    Logger::Print("TargetPosition.X",this->TargetPosition.X);
    Logger::Print("TargetPosition.Y",this->TargetPosition.Y);
    Logger::Print("TargetPosition.Z",this->TargetPosition.Z);
    Logger::Print("TargetPosition.Roll",this->TargetPosition.Roll);
    Logger::Print("TargetPosition.Pitch",this->TargetPosition.Pitch);
    Logger::Print("TargetPosition.Yaw",this->TargetPosition.Yaw);
    Logger::Print("Distance_mm", this->Distance_mm);
    Logger::Print("Acceleration", this->Acceleration);
    Logger::Print("Required_time", this->Required_time);
    Logger::Print("Speed_mm_per_second", this->Speed_mm_per_second);

}
