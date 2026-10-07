# CS-350-Emerging-Systems-Architecture

## Summarize the project and what problem it was solving.

I enhanced the morse code artifact, I realized I did not know much about morse code to begin with, so beyond the dict I already had, hooking it up to my raspberrypi again made me realize the code was not working quite as expected based on the debugging statements. 

## What did you do particularly well?

I changed up the structure of the project slightly, removed the judicious commenting that was mostly unnecessary fluff, and fixed bugs in the program such as checking for a condition in the wrong indentation resulting in wrong pausing.

## Where could you improve?
I could maybe improve on the user-experience, currently it waits for the entire message to send out before you can change it with the button, I could change it to interrupt the transmission instead. There is still a small bit of duplicated comments, I think re-iterating the requirements isn't bad but in terms of pure space, it could be a bit smaller. 

## What skills from this project will be particularly transferable to other projects and/or course work?

This type of troubleshooting took a while, but it did teach me a lot and that understanding will really help me in the future when I will inevitably run into more issues. Learning the layout for the breadboard could help me particularly in Systems Architecture 2, and in general I really think understanding technology at a lower level is really transferable. I can understand that a 5V display shouldn't write to a 3.3V RaspberryPI and should instead be tied to ground. I could look into the specifics of why each different resistor was used. 

## How did you make this project maintainable, readable, and adaptable?

I made the project more maintainable by reducing the amount you'd have to look to find answers, I improved the base structure and the algorithm to be correct, and readable. You could adapt this to other raspberrypi projects pretty easily for whatever purposes require morse code encoding. I left the comments about GPIO to make sure it is maintainable and understandable.
