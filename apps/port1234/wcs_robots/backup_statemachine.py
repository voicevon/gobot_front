# import json


# class StateMachine_Item():

#     def __init__(self, current_state:str, command:str, post_state:str, service_function=None) -> None:
#         self.current_state = current_state
#         self.command = command
#         self.post_state = post_state
#         self.service_function = service_function
#         if service_function is None:
#             self.service_function = self.do_nothing

#     def print_out(self):
#         print("------------ StateMachine_Item() -------------", end='')
#         print("from_state=", self.current_state, "  command=", self.command, "    post_state=", self.post_state)

#     @classmethod
#     def from_json(cls, json_string):
#         json_dict = json.loads(json_string)
#         return cls(** json_dict)

#     def is_acceptable(self,  current_state: str) -> bool:
#         if current_state in self.current_state:
#             return True
#         return False

#     def do_nothing(self):
#         pass


# class StateMachine():
#     def __init__(self, machine_name = 'NoName', init_state = 'idle') -> None:
#         self.items = [StateMachine_Item('idle','idle','idle', self.do_nothing)]
#         self.current_state = init_state
#         self.name = machine_name

#     def AppendItem(self, item: StateMachine_Item):
#         self.items.append(item)

#     def stimulate(self, command: str, **args) -> bool:
#         '''
#         Returns
#         True:  Update to post state successed.
#         False: Update to post state failed.
#         '''
#         if not self.__is_acceptable(command):
#             return False

#         for item in self.items:
#             if item.current_state == self.current_state:
#                 if item.command == command:
#                     item.print_out()
#                     item.service_function(**args)
#                     self.current_state = item.post_state
#                     return True
#         print("Statemachine::stimulate() failed, my_name is:", self.name)
#         return False 


#     def __is_acceptable(self, command:str) ->bool:
#         for item in self.items:
#             # item.print_out()
#             if item.current_state == self.current_state:
#                 if item.command == command:
#                     return True
#         return False

#     def do_nothing(self):
#         pass

