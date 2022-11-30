#pragma once

#include "CNC/arm_solution/arm_solution_base.h"
#include "twh_arm_solution_config.h"


class MiddleKinematic{
	public:
		float Arm_shaft_At_Y;
		float Arm_Angle;
		// float Minimuim_X;
};

class Twh_ArmSolution: public ArmSolutionBase{
    public:
        void LinkConfig(Twh_ArmSolution_Config* config){this->_config=config;};
        void MK_to_FK(FKPosition_XYZRPY* from_mk, FKPosition_XYZRPY* to_fk) override;
        void MK_to_Ik(FKPosition_XYZRPY* from_mk, IKPosition_abgdekl* to_ik) override;
        void real_MK_to_Fk(MiddleKinematic* mk, FKPosition_XYZRPY* fk);
        void real_MK_to_Ik(MiddleKinematic* mk, IKPosition_abgdekl* ik);

    protected:
        void IK(FKPosition_XYZRPY* from_fk,IKPosition_abgdekl* to_ik) override;
        void FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPY*  to_fk) override;

    private:
        Twh_ArmSolution_Config* _config;


};