


# withdraw #& deposite
'''
.      -------------------------------------idle----------------------------------------------<
.     |                                                                                       ^
.     |                    / (withdraw)                                                       ^
.     |                   /                                                                   ^
.     |          --------------------------------<                                            ^
.     |         |                                 ^                                           ^
.     |         |     / (ir_check_empty)          ^                                           ^
.     |         |    /                            ^                                           ^
.     +-----> pickingCell --------------> dropto_outbox --------------------------------------^
.     | \                          \                                    \                     ^
.     |  \ (withdraw)               \ (ir_check_blocked)                 \ (end_withdraw)     ^
.     |                                                                                       ^
.     |                                                                                       ^
.     |                                                                                       ^
.     |                                                     / (ir_check_block)                ^
.     |                                                    /                                  ^
.     |                                                   /            / (retry <=3)          ^
.     |                                                  /            /                       ^
.     |                                   droping_cell <---          /        /(white)        ^
.     |                                           |        ^    ------<-----------<           ^
.     |                                           |        ^   |       ^           ^          ^
.     |----> preparing_deposit ----->  picking_centerbox -----> retry ---> verify ----------> ^
.        \                        |        \                       \                     \    ^
.         \ (prepare_deposit)     |         \ (start_deposit)       \ (ir_check_empty)    \ (black)   
.                                 |                                                           ^
'                                 |-----------------------------------------------------------^     
'                                                            \
'                                                             \ (end_deposit)
'''                  


