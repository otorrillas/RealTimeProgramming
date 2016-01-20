from threading import Thread, Lock



i = 0
lock = Lock()

def Thread_func1():
	global i
	for j in range(1000000):
		lock.acquire()
		try:
		    i = i + 1
		finally:
		    lock.release()
		

		

		
def Thread_func2():
	global i
	for k in range(1000000):
		lock.acquire()
		try:
		    i = i - 1
		finally:
		    lock.release()


def main():
	
	Thread1 = Thread(target = Thread_func1(), args = (),)
	Thread2 = Thread(target = Thread_func2(), args = (),)
	Thread1.start()
	Thread2.start()
    
	Thread1.join()
	Thread2.join()
	
	print(i)
	

	
main()
