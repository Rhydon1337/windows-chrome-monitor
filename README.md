# windows-chrome-monitor
## TL;DR
Monitor Chrome to know when movie or TV show ends and turns off the computer automatically
Does it bother you that your pc is running all the night when you sleep after the movie/TV show you watched is over?
Does it bother you to get out of bed to shutdown the pc when the movie/TV show you watched is over?

## How it works
First, The program need to find chrome process which run the movie/TV show. It done by calculating 
the cpu usage percentage for each chrome process instance over 5 minutes and then pick the one that spend the most cpu usage.
Then, the program monitor to that process and when the process cpu usage is below the minimum for 5 minutes
we can understand that the movie is over and we are ready to shutdown the computer.

Formula:
Lets say wo took the first process and system overall usage examinations in time T1
And lets say wo took the second process and system overall usage examinations in time T2
Now we can calculate the process cpu usage percentage between T1 - T2 by this formula:

Percentage = (T2SystemOverallCpuTime - T1SystemOverallCpuTime) / (T2ProcessOverallCpuTime - T1ProcessOverallCpuTime) * 100

## Usage
Run WindowsChromeMonitor.exe as administrator

DONE!!!