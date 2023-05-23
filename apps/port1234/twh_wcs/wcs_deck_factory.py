from twh_wcs.von.wcs.deck.simple_deck import SimpleDeck
from twh_wcs.von.wcs.deck.deck import Wcs_DeckBase
from von.logger import Logger


g_decks = dict[str, dict[str,list[Wcs_DeckBase]]]()
'''
Explain:
```
      dict[str, dict[str, list[Wcs_DeckBase]]]()
           |          |           |
           |          |           |--- deck instance.
           |          |--- group_name
           |--- warehouse_id
```'''          


class DeckGroupFactory:
    def __init__(self) -> None:
        pass

    @classmethod
    def FindIdleDecks(cls, warehouse_id:str, group_name:str) -> list[Wcs_DeckBase]:
        idle_decks = list[Wcs_DeckBase]()
        for deck in g_decks[warehouse_id][group_name]:
            if deck.GetState() == 'idle':
                idle_decks.append(deck)
        return idle_decks
    

    @classmethod
    def CreateDeckGroup(cls, group_name:str, deck_count:int) -> dict[str, list[Wcs_DeckBase]]:
            decks = list[Wcs_DeckBase]()
            for i in range(deck_count):
                new_deck = SimpleDeck(i)
                decks.append(new_deck)
            group ={}
            group[group_name] = decks
            return group

    @classmethod
    def CreateDeckGroups(cls, warehouse_id:str) -> dict[str,list[Wcs_DeckBase]]:
        if warehouse_id == '221109':
            group_name = 'packer'
            deck_count = 12
            new_group =  cls.CreateDeckGroup(group_name, deck_count)
            g_decks[warehouse_id] = new_group
            return new_group
        else:
            Logger.Error('DeckGroupFactory::CreateDeckGroup()   Unknown warehouse_id')
            Logger.Print("warehouse_id", warehouse_id)
            
        return None # type: ignore


