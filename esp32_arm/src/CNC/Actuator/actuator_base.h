#pragma once

class ActuatorBase{
    public:
        // virtual void Init();
        virtual float GetCurrentPosition();
        virtual void SetPosition(float position);
        virtual void MoveTo(bool is_absolute_position, float position);

    protected:

    private:
    
};