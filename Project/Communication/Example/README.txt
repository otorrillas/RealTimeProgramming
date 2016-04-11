Communication module should be ready. Here's small instruction how to use it.

1. Run Master_recv first -> it listens to the lifts 
2. Run ./Lift (+ IP of Master); Lift will send Hello Message to Master_recv, Master will then save lift ID and IP;
	NOTE: Lift has to be run on another pc with different IP, on localhost it doesn't work!
3. Run Master_send it will constantly send messages to connected lift but only if previosly Master_recv stored its IP	
4. Main_Master is some kind of a backup and a try of spawning all Master processes at once but it behaves crazy sometimes, you can 	  look at it and maybe improve it or I will figure something out.


Example folder contains the all files necessary(some are doubled), but you have it also kept nicely in other directoris(Master, Lift, Communication). I did it to have quicker access to them, we earase them later.
  
