from pika import spec

EXCHANGE = 'customers'
EXCHANGEACK = 'logs' #keep this seperate for now

class ChannelContainer(object):

    def __init__(self, channel, exchange, append, queue_list, callback):
        self.channel = channel
        self.exchange = exchange
        self.append_str = append
        self.queue_list = queue_list
        self.goodQueues = 0
        self.channelReady = False
        self.try_to_continue = callback

    def exchange_declare(self):
        self.channel.exchange_declare(exchange=self.exchange,
        type='direct', \
        callback=self.on_exchange_declare)


    # Step #4
    def on_exchange_declare(self, frame):
        """
        Called when our exchange has been created
        """
        for queue_name in self.queue_list:
            full_queue_name = queue_name + self.append_str
            self.channel.queue_declare(queue=full_queue_name, durable=True,            \
                            exclusive=False, auto_delete=False, \
                            callback=self.on_queue_declared)

        # Step #5
        def on_queue_declared(self, frame):
            """
            Bind the queues to the channel
            """
            if type(frame.method) == spec.Queue.DeclareOk:
                # Get the queue name
                queue_name = frame.method.queue
                print(self.append_str, "binding", queue_name)
                self.channel.queue_bind(exchange=self.exchange,
                queue=queue_name,
                routing_key=queue_name,
                callback=self.on_queue_bound)

        # Step #6
        def on_queue_bound(self, frame):
            """
            Called when RabbitMQ has told us our Queue has been declared,
            frame is the response from RabbitMQ
            """
            print("bound queue")
            self.goodQueues += 1
            if self.goodQueues == len(self.queue_list):
                self.channelReady = True
                #tell owner to try to continue
                self.try_to_continue()

class TxChannel(ChannelContainer):
    def __init__(self, channel, exchange, append, queue_list, callback):
        ChannelContainer.__init__(self, channel, exchange, append,
        queue_list, callback)

    def acknowledge(self, queue_name, body):
        """
        Send the action and object to the acknowledgement queue.
        """
        full_queue_name = '%s--ack' % queue_name
        print ("sending ack", full_queue_name)
        self.channel.basic_publish(exchange=self.exchange,
                    routing_key=full_queue_name,
                    body=body)

class RxChannel(ChannelContainer):
    def __init__(self, channel, exchange, append, queue_list, callback):
        ChannelContainer.__init__(self, channel, exchange, append,
        queue_list, callback)

    # Step #6
    def consume(self, callback):
        """
        Called when RabbitMQ has told us our Queue has been declared,
        frame is the response from RabbitMQ
        """
        #consume on all queues now
        for queue_name in self.queue_list:
            self.channel.basic_consume(callback, queue=queue_name)

# Here is the consumer code. I tried to take out everything irrelevant:

import pika
from pika.adapters import SelectConnection
from ChannelContainer import RxChannel, TxChannel

EXCHANGE = 'customers'
EXCHANGEACK = 'logs' #keep this seperate for now

class RabbitConsumer(object):

    def __init__(self):
        #initialize some variables
        self.queue_list = []
        self.goodQueues = 0
        self.channel_rx = None
        self.channel_tx = None
        self.connection = None
        self.parse_config()
        #get the queue_list
        self.gather_queue_names()
        """
        Functions to parse data and load config
        """
    def parse_config(self):
        """
        Parse the variables in the config file into the instance.
        """
        self.rabbitmq_host = 'localhost'
    def gather_queue_names(self):
        """
        Gather the names of the queues.
        """
        #These are just for testing
        self.queue_list = ['1a', '2b', '3c-4d-5e-6f-7g']

    """
    Functions to connect to RabbitMQ
    """
    # Step #2
    def on_connected(self, connection):
        """
        Called when we are fully connected to RabbitMQ
        """
        # Open a channel for tx and rx
        self.connection.channel(self.on_channel_open)
        self.connection.channel(self.on_channel_open)


    # Step #3
    def on_channel_open(self, new_channel):
        """
        Called when our channel has opened
        """
        if self.channel_rx is None:
            #rx channel
            self.channel_rx = RxChannel(new_channel, EXCHANGE,
            '', self.queue_list, self
            .start_listening)
            self.channel_rx.exchange_declare()
        else:
            self.channel_tx = TxChannel(new_channel, EXCHANGEACK,
            '--ack', self.queue_list, self
            .start_listening)
            self.channel_tx.exchange_declare()

    # Step #7
    def start_listening(self):
        """
        Called when all queues for a channel are declared
        """
        print ("test_allset")
        if self.channel_rx.channelReady and self.channel_tx.channelReady:
            #both channels are ready, try to continue
            self.channel_rx.consume(self.handle_consume)

    def handle_consume(self, channel, method, properties, body):
        print ("received message")
        queue_name = method.routing_key
        # Insert some long and complicated code goes here
        result = 'I finished!'
        #have the tx channel send an ack
        self.channel_tx.acknowledge(queue_name, result)

    # Step #1
    def start_consumer(self):
        # Step #1: Connect to RabbitMQ
        self.connection = SelectConnection(pika.ConnectionParameters(\
        host=self.rabbitmq_host), self.on_connected)
        try:
            # Loop so we can communicate with RabbitMQ
            self.connection.ioloop.start()
        except KeyboardInterrupt:
            # Gracefully close the connection
            self.connection.close()
            # Loop until we're fully closed, will stop on its own
            self.connection.ioloop.start()


if __name__ == '__main__':
    consumer = RabbitConsumer()
    consumer.start_consumer()
