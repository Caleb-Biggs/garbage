import sys
import threading
import queue
import time
import pygame


class Vis_Item:
	def __init__(self, file, func, time, ptr):
		self.file = file
		self.func = func
		self.time = time
		self.ptr = ptr


def main():
	q = queue.Queue()
	threading.Thread(target=reader, args=(q,)).start()
	threading.Thread(target=visual, args=(q,100,)).start()


def reader(q):
	for line in sys.stdin:
		file = line[0]
		func = line[2]
		tv_sec = int(line[4:24])
		tv_nsec = int(line[25:45])
		time = tv_sec+(tv_nsec/1e9)
		ptr = 0 if line[46:66] == "               (nil)" else int(line[46:66], 16)
		q.put(Vis_Item(file, func, time, ptr))
		# print(line)
	q.put("END")


def visual(q, slowness):
	pygame.init()
	screen = pygame.display.set_mode((800, 600))


	prev_time = None
	while True:
		p = q.get()
		if p == "END": break

		if prev_time != None:
			time.sleep(slowness*(p.time-prev_time))
		prev_time = p.time

		# pygame.draw.circle(screen, (255,0,0), (0,0), 10)

		match (p.file, p.func):
			case ('G', 'S'):
				pass
			case ('G', 'E'):
				pass
			case ('G', 'A'):
				pass
			case ('G', 'C'):
				pass
			case ('G', 'T'):
				pass

		print(p.file, p.func, p.time, p.ptr)


if __name__ == '__main__': main()