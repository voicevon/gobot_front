import json


class StateMachine_Item():

    def __init__(self, current_state:str, command:str, post_state:str, service_function, repeat_serve=False) -> None:
        self.current_state = current_state
        self.command = command
        self.post_state = post_state
        self.service_function = service_function
        self.repeat_invoke_service_function = repeat_serve
        # self.print_debug = False

    def print_out(self):
        print("------------ StateMachine_Item() -------------", end='')
        print("from_state=", self.current_state, "  command=", self.command, "    post_state=", self.post_state)

    @classmethod
    def from_json(cls, json_string):
        json_dict = json.loads(json_string)
        return cls(** json_dict)

    def is_acceptable(self,  current_state: str) -> bool:
        if current_state in self.current_state:
            return True
        return False