# !/usr/bin/env python
# -*- coding: utf-8 -*-


# cd ~/PhoenixGo
# bazel-bin/mcts/mcts_main --gtp --config_path=etc/mcts_1gpu.conf --logtostderr --v=0 --listen_port=50007


import socket
import logging


from gogame.chessboard import ChessboardLayout
from gogame.chessboard_cell import StoneColor
from config.config import Config

from von.terminal_font import TerminalFont

class GoGameAiClient(object):
    '''
    Connet to PhonixGo, or any other Go player.
    Via TCP socket.
    The player must be running in cloud, 
    '''
    def __init__(self):
        self.layout = ChessboardLayout('AI layout')
        self.__is_connected = False
        self.__socket_client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)      # 定义socket类型，网络通信，TCP


        self.__FC_BLACK = TerminalFont.Color.Fore.black
        logging.warning('Init AI is done......')

    def __to_ai(self, command:str) -> str:
        '''
        TODO:  rename to:  talk_to_ai()  ??
        '''
        command += '\n'
        self.__socket_client.sendall(command.encode())      # 把命令发送给对端
        data = self.__socket_client.recv(1024)     # 把接收的数据定义为变量'
        
        receiving = True
        while receiving:
            if(data.count(b'\n\n') > 0):
                receiving = False
            else:
                data = data + self.__socket_client.recv(1024)
        text = data.decode().replace("\n\n", "")
        # print(data,text)
        return text
    
    def list_commands(self):
        the_list = self.__to_ai('list_commands')
        print(the_list)

    def get_board(self):
        '''
        PhoenixGo doesn't support this command.
        See list_commands()
        '''
        ret = self.__to_ai('showboard')
        print (ret)

    def start_new_game(self):
        #   connect_to_server
        if self.__is_connected:
            logging.warn('GoGameAiCient: connect() is invoked when connected')
            return

        server = Config.Internet.ai.ip
        port = Config.Internet.ai.port
        # try:
        print('>>>>>>>>>>>>>>>>>>>>>>>>>> server: %s Port: %d'%(server, port))
        self.__socket_client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)      # 定义socket类型，网络通信，TCP
        self.__socket_client.connect((server, port))       # 要连接的IP与端口
        print('>>>>>>>>>>>>>>>>>>>>>>>>>> PhoenixGo is Connected >>>>>>>>>>>>>')
            # return True
        # except ConnectionRefusedError:
        #     return False
        # except:
        #     pass
        self.__is_connected = True

        # Connect to AI
        ret = self.__to_ai("clear_board")
        if(ret.count("=") > 0):
            print("AI棋盘: 清空成功\r\n")
            self.layout.clear()
            Config.current_game.lastest_move_cell_name = None
        else:
            logging.error('Start ai player error! ')

    def stop_game(self):
        xx = self.__to_ai('quit')
        print('=======================%s'%xx)
        self.__socket_client.close()   # 关闭连接`
        # self.__socket_client.shutdown()
        self.__is_connected = False
        print('GO AI_client is closed')

    def get_ai_move(self):
        # AI computing to move White
        ret = self.__to_ai('genmove w')
        cell_name = ret.replace("= ", "")
        self.layout.play(cell_name, self.__FC_BLACK)
        Config.current_game.lastest_move_cell_name = cell_name
        return cell_name

    def feed_user_move(self, cell_name:str):
        '''
        Tell AI where user has played a stone of Black.
        '''
        self.layout.play(cell_name, StoneColor.BLACK)
        command = "play b " + cell_name
        ret = self.__to_ai(command)
        # print(command, ret)

        # if ret.decode().count("=") > 0:
        if ret.count("=") > 0:
            Config.current_game.lastest_move_cell_name = cell_name
            return
        else:
            logging.warn('feed_user_move() ret=%s' %ret)

    def get_final_score(self):
        xx = self.__to_ai('final_score')
        print('final_score', xx)

if __name__ == "__main__":

    runner = GoGameAiClient()
    runner.start_new_game()
    runner.list_commands()
    runner.feed_user_move('Q4')
    cell_name = runner.get_ai_move()
    print("Computer played at  ", cell_name)
    runner.stop_game()



    # while True:
    #     # runner.get_ai_move()
    #     runner.get_board()


#  Config game rule
#       https://github.com/Tencent/PhoenixGo#configure-guide
#
#  A7. How make PhoenixGo think with longer/shorter time?
#       https://github.com/Tencent/PhoenixGo/blob/master/docs/FAQ.md#a7-how-make-phoenixgo-think-with-longershorter-time
#             File location: ~/PhoenixGo/etc/mcts_dist.conf  Not this one
#             File location: ~/PhoenixGo/etc/mcts_1gpu.conf   Can be seen from command line, When launch PhoenixGo

# https://www.gnu.org/software/gnugo/gnugo_19.html
# https://en.wikipedia.org/wiki/Handicap_(go)
