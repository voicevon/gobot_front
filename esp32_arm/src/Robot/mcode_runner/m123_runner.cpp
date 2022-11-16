#include "m123_runner.h"
#include "Robot/mcode_runner/mcode_os.h"
// #include "mcode_runners.h"


// EEF action will be differ for each robot

void Final_M123_RunnerBase::LinkToRunnerDispacher(){
    M123_Runner_EefAct* m123_runner = (M123_Runner_EefAct*) McodeOS::Instance().GetRunner(123);
    m123_runner->LinkFinalRunner(this);

}
