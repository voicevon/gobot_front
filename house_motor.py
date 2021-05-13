import RPi.GPIO as GPIO
import time



class Stepper():
	def __init__(self):
		GPIO.setmode(GPIO.BCM)
		GPIO.cleanup()
		GPIO.setwarnings(False)
		self.__x_dir_pin = 23   # Pinout = 16
		self.__x_step_pin = 24  # Pinout = 18
		self.__y_dir_pin = 17   # Pinout = 11
		self.__y_step_pin = 27  # Pinout = 13

		GPIO.setup(self.__x_dir_pin, GPIO.OUT)
		GPIO.setup(self.__x_step_pin, GPIO.OUT)
		GPIO.setup(self.__y_dir_pin, GPIO.OUT)
		GPIO.setup(self.__y_step_pin, GPIO.OUT)

		self.__sleep_time = 0.2   # unit is second

	def move_stone(self, relative_x, relative_y):
		x_dir = GPIO.HIGH
		if relative_x < 0:
			x_dir = GPIO.LOW
			relative_x = - relative_x
		GPIO.output(self.__x_dir_pin, x_dir)

		y_dir = GPIO.HIGH
		if relative_y < 0:
			y_dir = GPIO.LOW
			relative_y = - relative_y
		GPIO.output(self.__y_dir_pin, y_dir)


		max_step = max(relative_x, relative_y)
		for i in range(0,max_step):
			if relative_x > 0:
				GPIO.output(self.__y_dir_pin, y_dir)
				relative_x -= 1

			if relative_y > 0:
				GPIO.output(self.__y_step_pin, y_dir)
				relative_y -= 1

			time.sleep(self.__sleep_time)
			GPIO.output(self.__x_step_pin, y_dir)
			GPIO.output(self.__y_step_pin, y_dir)
			time.sleep(self.__sleep_time)
			print('step into ', i)
		

if __name__ == '__main__':

	testor = Stepper()
	testor.move_stone(100,20)


