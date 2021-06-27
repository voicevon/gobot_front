#! /usr/bin/env python
# -*- coding: utf-8 -*-


# sudo chmod 666 /dev/ttyUSB0
from config import config as  app_config
import logging
import sys
sys.path.append("/home/pi/pylib")
from terminal_font import TerminalFont


class HumanLevel_RobotArm:
    '''
    This is an abstract robot.
    It is a  thing that Understood coordinators, 
    and can tranlate from chessboard co-ord to robot arm co-ord
        1. Chessboard coordinator: like 'Q4', 'T19', 
            Some position is out of the chessboard, like 'Trash','HouseEnd' 
        2. Robot arm coordinator: (35,126) (258,129)

    '''
    def __init__(self):
        '''
        '''
        pass

        # self.__robot_eye = RobotEye()
        # self.__robot_eye.start_show('origin')
        self._FC_YELLOW = TerminalFont.Color.Fore.yellow
        self._BG_GREEN = TerminalFont.Color.Background.green
        self._FC_RESET = TerminalFont.Color.Control.reset 
        self.__at_picked_up = False

    def goto_here(self, target_pose):
        '''
        Both hard_robot and soft_robot will goto the position of pose_name.
        the pose_name must be avaliable in pose_diction 
        '''
        if self.__log_level > 1:
            print('[Info:][Human_level_robot.goto_here()] Robot is moving to destination...')
        # self.__hard_robot.set_joints_angle_in_degree(target_pose.IK.to_diction())

        self.__set_hard_robot_folllowing(False)
        IK_dict = target_pose.IK.to_diction()
        self.__hard_robot.set_joints_angle_in_degree(IK_dict)
        if app_config.robot_arm.enable_moveit:
            self.__soft_robot.goto_the_pose_uint_mm(target_pose.FK)

        self.current_pose.name = target_pose.name
        self.current_pose.FK.from_diction(target_pose.FK.to_diction())
        self.current_pose.IK.from_diction(IK_dict)

    def __eef_pick_up(self):
        self.__hard_robot.eef_pick_up()

    def __eef_place_down(self):
        self.__hard_robot.eef_place_down()

    def eef_pickup_placedown(self):
        if self.__at_picked_up:
            self.__eef_place_down()
        else:
            self.__eef_pick_up()
        self.__at_picked_up = not self.__at_picked_up

    def eef_sleep(self):
        self.__hard_robot.eef_sleep()

    def action_pickup_chess_from_a_cell(self, cell_name='k10'):
        print ('[Info]: action_pickup_chess_from_a_cell  %s' %cell_name)
        # pose = self.get_target_pose_by_name('VIEW')
        # self.goto_here(pose)
        #pose = self.get_target_pose_by_name(cell_name.lower())
        #self.goto_here(pose)
        #pose = self.get_target_pose_by_name(cell_name.upper())
        #self.goto_here(pose)
        #self.__eef_pick_up()
        #pose = self.get_target_pose_by_name(cell_name.lower())
        #self.goto_here(pose)

    def action_pickup_chess_from_warehouse(self):
        logging.info('[Info]: Action_pickup_chess_from_warehouse')
        
        #pose_name_list_a = ['warehouse','WAREHOUSE']
        #for pose_name in pose_name_list_a:
        #    pose = self.get_target_pose_by_name(pose_name)
        #    self.goto_here(pose)
        #self.__eef_pick_up()
        # lift up gripper
        #pose = self.get_target_pose_by_name('warehouse')
        #self.goto_here(pose)

    def action_place_chess_to_trash_bin(self, park_to_view_point=True):
        logging.info('[Info]: Action_place_chess_to_trash_bin')

        #pose = self.get_target_pose_by_name('TRASH')
        #self.goto_here(pose)
        #self.__eef_place_down() 
        # lift up gripper
        #pose = self.get_target_pose_by_name('trash')    # this is a delaying for eef_place_down()  
        #self.goto_here(pose)
        #self.eef_sleep()

        #if park_to_view_point:
        #    pose = self.get_target_pose_by_name('VIEW')
        #    self.goto_here(pose)

    def action_place_chess_to_a_cell(self, cell_name='k10', auto_park=True):
        logging.info('[Info]: action_place_chess_to_a_cell %s' %cell_name)
        # pose = self.get_target_pose_by_name('VIEW')
        # self.goto_here(pose)
        #pose = self.get_target_pose_by_name(cell_name.lower())
        #self.goto_here(pose)
        #pose = self.get_target_pose_by_name(cell_name.upper())
        #self.goto_here(pose)
        #self.__eef_place_down()
        #pose = self.get_target_pose_by_name(cell_name.lower())
        #self.goto_here(pose)
        #self.eef_sleep()
        #if auto_park:
        #    pose = self.get_target_pose_by_name('VIEW')
        #    self.goto_here(pose)
            


if __name__ == '__main__':
    my_robot = HumanLevel_RobotArm(app_config.robot_arm.name)
    # my_robot.bridge_hard_robot_connect_to_marlin()
    # my_robot.bridge_hard_robot_home_all_joints()
    # my_robot.bridge_soft_robot_connect_to_moveit()
