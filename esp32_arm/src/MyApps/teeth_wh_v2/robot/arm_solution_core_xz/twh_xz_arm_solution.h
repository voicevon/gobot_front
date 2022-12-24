#pragma once

#include "CNC/arm_solution/arm_solution_base.h"
#include "twh_xz_arm_solution_config.h"


class MiddleKinematic{
	public:
		float Arm_shaft_At_Y;
		float Arm_Angle;
};

class Twh2_XzArmSolution: public ArmSolutionBase{
    public:
        void MK_to_FK(FKPosition_XYZRPW* from_mk, FKPosition_XYZRPW* to_fk) override;
        void MK_to_Ik(FKPosition_XYZRPW* from_mk, IKPosition_abgdekl* to_ik) override;
        void real_MK_to_FK(MiddleKinematic* mk, FKPosition_XYZRPW* fk);
        void real_MK_to_IK(MiddleKinematic* mk, IKPosition_abgdekl* ik);
        void real_FK_to_MK(FKPosition_XYZRPW* fk, MiddleKinematic* mk);
        void real_IK_to_MK(IKPosition_abgdekl* ik, MiddleKinematic* mk);

        void Test_FK_MK_FK(FKPosition_XYZRPW* input);
        void Test_MK_FK_MK(MiddleKinematic* input);
        void Test_IK_MK_IK(IKPosition_abgdekl* input);
        void Test_MK_IK_MK(MiddleKinematic* input);

        void FK_to_IK(FKPosition_XYZRPW* from_fk,IKPosition_abgdekl* to_ik) override;
        void IK_to_FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPW*  to_fk) override;

    private:
        Twh2_XzArmSolution_Config __config;
};

