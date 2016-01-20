
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
    
      
}

func Thread_func2() {
	
	
	for j := 0; j < 1000000; j++{
	
		i-=1
	}
    
      
}

func main(){

	runtime.GOMAXPROCS(runtime.NumCPU())    
                                           
    go Thread_func1()                          
    go Thread_func2()                      

    time.Sleep(200*time.Millisecond)
    
    
    fmt.Println(i)



}
