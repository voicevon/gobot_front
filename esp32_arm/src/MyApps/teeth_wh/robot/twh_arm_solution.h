#pragma once

#include "CNC/arm_solution/arm_solution_base.h"
#include "twh_arm_solution_config.h"


class MiddleKinematic{
	public:
		float X;
		float Angle;
		// float Minimuim_X;
};

class Twh_ArmSolution: public ArmSolutionBase{
    public:
        void LinkConfig(Core_XY_XA_ab_config* config){this->_config=config;};
        void MK_to_Fk(MiddleKinematic* from_mk, FKPosition_XYZRPY* to_fk);

    protected:
        void IK(FKPosition_XYZRPY* from_fk,IKPosition_abgdekl* to_ik) override;
        void FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPY*  to_fk) override;

        Core_XY_XA_ab_config* _config;

    private:

};