


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
.     |                                                    / (ir_check_block)                 ^
.     |                                                   /                                   ^
.     |                                   droping_cell <---                                   ^
.     |                                           |        ^    / (ir_check_empty<=3)         ^
.     |                                           |        ^   /                              ^
.     |--------------> preparing_deposit ----->  picking_centerbox -------------------------> ^
.        \                        |        \                          \                       ^
.         \ (prepare_deposit)     |         \ (start_deposit)          \ (ir_check_empty>3)   ^
.                                 |                                                           ^
'                                 |-----------------------------------------------------------^     
'                                                            \
'                                                             \ (end_deposit)
'''                  


