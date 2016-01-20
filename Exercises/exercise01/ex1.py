from threading import Thread



i = 0

def Thread_func1():
	
	for j in range(1000000):
		global i
		i+=1
	print(i)
		


print(i)
		
def Thread_func2():
	
	for k in range(1000000):
		global i
		i-=1
	print(i)


def main():
	
	Thread1 = Thread(target = Thread_func1())
	Thread1.start()
    
	Thread2 = Thread(target = Thread_func2())
	Thread2.start()
    
	Thread1.join()
	Thread2.join()
	
	
	

	
main()
