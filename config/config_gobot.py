

class StateMachine:
    '''
    '   User will always use black stone, and place stone first. 
    ' 
    '                    --<------<------<-- 
    '                  /                     \                          
    '                 /<--- Withdraw_black <--\                    
    '                /                         \               
    ' begin --> user_playing            computer_playing  
    '                \                         / 
    '                 \--> Withdraw_white --->/ 
    '                  \                     /  
    '                    -->------>---    --->--- 
    '''                     
    
    class state:
        begin = 1
        computer_playing = 2
        withdraw_white = 3
        comparing_layout = 4
        user_playing = 5
        withdraw_black = 6
        game_over = 7