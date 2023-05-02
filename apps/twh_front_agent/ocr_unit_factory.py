from ocr_unit import OcrUnit
from von.logger import Logger



class OcrUnitFactory:
    
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