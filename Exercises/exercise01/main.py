from threading import Thread



i = 0

def Thread_func1():
	global i
	for j in range(1000000):
		i = i + 1

		

		
def Thread_func2():
	global i
	for k in range(1000000):
		i-=1


def main():
	
	Thread1 = Thread(target = Thread_func1(), args = (),)
	Thread2 = Thread(target = Thread_func2(), args = (),)
	Thread1.start()
	Thread2.start()
    
	Thread1.join()
	Thread2.join()
	
	print(i)
	

	
main()
