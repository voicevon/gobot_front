from abc import ABC, abstractmethod
from twh_wcs.twh_robot_loop_porter import Twh_LoopPorter
from twh_wcs


class OrderItem_Scheduler(ABC):
    
    def __init__(self) -> None:
        self.state = 'idle'
        self.__got_start_command = False
        self.__linked_loop_porter :Twh_LoopPorter

    def StartPorting(self):
        self.__got_start_command = True

    @abstractmethod
    def _get_idle_porter() -> Twh_LoopPorter:
        idle_porter = Twh_LoopPorter('', 1,"" ,'')
        return idle_porter
    
    def SpinOnce(self):
        if self.state == 'idle':
            self.state = 'inside_loop_porter'
        if self.state == 'inside_loop_porter':
            if self.__got_start_command:
                loop_porter =  self._get_idle_porter()
                if loop_porter is not None:
                    loop_porter.MoveToGate()
                    self.state = 'inside_loop_porter_moving'
        if self.state == 'inside_loop_porter_moving':
            if self.__linked_loop_porter.GetState() == 'ready':
                self.state = 'loop_porter_gate'
        if self.state == 'loop_porter_gate':
            self.__linked_loop_porter.PickPlace()
            self.state = 'loop_porter_gate_picking'
        if self.state == 'loop_porter_gate_picking':
            if self.__linked_loop_porter.has_done():
                # Q: Need to wait tube conveyer be idle ?
                # A: Don't know now.
                self.state = 'loop_porter_done'

        if self.state == 'loop_porter_done':
            if self.__linked_tube_conveyer.GetState() == 'idle':
                self.__linked_loop_porter.SetStateTo('idle')
                self.__linked_tube_conveyer.SetVavle()
                self.state = 'tube_moving'
        if self.state == 'tube_moving':
                self.state = 'tube_outlet'
        if self.state == 'tube_outlet':
            if self.__linked_tube_conveyer().got_outlet():
                self.__linked_tube_conveyer().SetStateTo('idle')
                self.state = 'workstation'


        