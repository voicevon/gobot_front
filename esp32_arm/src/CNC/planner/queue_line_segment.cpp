#include "queue_line_segment.h"

void Queue_LineSegment::DeepCopyCurrentFkPositionTo(FKPosition_XYZRPY* position){
    LineSegment* head_line = this->GetHeadLineSegment();
    position->X = head_line->TargetPosition.X;
    position->Y = head_line->TargetPosition.Y;
    position->Z = head_line->TargetPosition.Z;
    position->Roll = head_line->TargetPosition.Roll;
    position->Pitch = head_line->TargetPosition.Pitch;
    position->Yaw = head_line->TargetPosition.Yaw;
}
