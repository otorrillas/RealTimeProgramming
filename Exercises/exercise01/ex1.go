
package main

import (
     "fmt"
    "runtime"
    "time"
 
)


var i int = 0


func Thread_func1() {
	
	
	for j := 0; j < 1000000; j++{
	
		i+=1
	}
    
    fmt.Println(i)
      
}

func Thread_func2() {
	
	
	for j := 0; j < 1000000; j++{
	
		i-=1
	}
    
    fmt.Println(i)
      
}

func main(){

	runtime.GOMAXPROCS(runtime.NumCPU())    
                                           
    go Thread_func1()                      

    time.Sleep(100*time.Millisecond)
    
    go Thread_func2()                      

    time.Sleep(100*time.Millisecond)
    
    
    fmt.Println("Hello from main!")



}
