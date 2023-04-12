#include "line_segments_queue.h"

void gs_LineSegment_Queue::DeepCopyCurrentFkPositionTo(FKPosition_XYZRPW* position){
    LineSegment* head_line = this->GetHeadObject();
    position->X = head_line->TargetPosition.X;
    position->Y = head_line->TargetPosition.Y;
    position->Z = head_line->TargetPosition.Z;
    position->Roll = head_line->TargetPosition.Roll;
    position->Pitch = head_line->TargetPosition.Pitch;
    position->Yaw = head_line->TargetPosition.Yaw;
}
