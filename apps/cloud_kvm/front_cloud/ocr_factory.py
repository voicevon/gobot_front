from ocr_unit import OcrUnit
from ocr_window import OcrWindow

from von.logger import Logger



class OcrFactory:
    
    @classmethod
    def CreateOcrWindow(cls, window_name:str) -> OcrWindow:
        if window_name == 'ubuntu_performance':
            new_windows = OcrWindow("ocr/ubuntu_performance/screen_image", "ocr/ubuntu_performance/config")
            return new_windows
        else:
            Logger.Error("OcrFactory::CreateOcrWindow()")
            Logger.Print('requested window_name', window_name)
            return None # type: ignore

    

    @classmethod
    def CreateOcrUnit(cls, unit_name:str) -> OcrUnit:
        if unit_name == 'title':
            unit = OcrUnit()
            unit.name = unit_name
            unit.height = 660
            unit.width = 450
            unit.left_offset = -20
            unit.top_offset = 50
            return unit
        else:
            Logger.Error('CreateOcrUnit()')
            Logger.Print('request unit_name', unit_name)
            return None # type: ignore