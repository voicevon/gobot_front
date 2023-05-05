import cv2


class SingleMarker:
    def __init__(self, id:int) -> None:
        self.id = id
        self.x1 = 1
        self.y1 = 2
        self.x2 = 3
        self.y2 = 4

    # def get_elements(self):
    #     return self.x1, self.y1, self.x2, self.y2

    def update_position(self, x1,y1,x2,y2):
        self.x1 = x1
        self.y1 = y1
        self.x2 = x2
        self.y2 = y2

    def draw_rectangle(self, image, color):
        cv2.rectangle(image, (self.x1, self.y1), (self.x2, self.y2), color, thickness=2)

    def get_json(self) :
        result = {}
        result["id"] = self.id
        result["postions"] = self.x1, self.y1, self.x2, self.y2
        return result
