# WhatPulse Logitech Gaming Keyboard Widget
Welcome to the repository where we host the source code for the WhatPulse Logitech Gaming Keyboard Widget. This widget can be found packaged as an installer on http://whatpulse.org/downloads/ - this repository is for the people that want to look at the source, to either improve on it, or use it as an example on how to use the WhatPulse Client API.

## What does this do?

This widget displays WhatPulse statistics (unpulsed or your account totals) on your Logitech Keyboard LCD screen. Here are two pictures from test keyboards (the Logitech G19 and G510):

| G19        | G510     |
| ------------- |:-------------:| 
| ![G19](https://dl.dropboxusercontent.com/u/7766970/WhatPulse/WhatPulse-Logitech-Widget-G19-stats.jpg)     | ![G510](https://dl.dropboxusercontent.com/u/7766970/WhatPulse/WhatPulse-Logitech-Widget-G510-stats.jpg) |

Pretty cool, right? :-)

## Settings

You can change two things about this widget; 1) the time it takes to refresh the stats and 2) the Client API URL it gets the stats from.

There settings are stored in the file *WhatPulseLogitechWidget.ini*, which is stored in the local application directory (%LOCALAPPDATA%). Here's how it looks by default:

> [Settings]

> ClientAPIURL = http://localhost:3490/v1/unpulsed

> RefreshSeconds = 5


You can either edit this file directly and watch it take effect the next refresh, or use the *Menu* button on your keyboard, if you have one (the G510 doesn't), and see this window pop up:

![Settings](https://dl.dropboxusercontent.com/u/7766970/WhatPulse/WhatPulse-Logitech-Settings.png)


## Keyboards Tested

This widget is currently only tested on a Logitech G19 and G510. It should work on all Logitech Gaming keyboards, though. Please let us know if you have another type and whether it works, or doesn't work!

## Prerequisites

This project has been build using Microsoft Visual Studio 2013 and the project files are included to make it easier to compile this yourself. All the code needed to compile it is included and there are no required libraries.

You do need the *Logitech Gaming Software* to be able to run this widget though, as it connects to the keyboard through that software.

It is also required to enable the Client API inside the WhatPulse Client. You can find this in the Settings tab, on the last page. The Client API was introduced in version 2.7.

## Forks / Pull Requests

We encourage you to take this code and make something cool yourself, all we ask is you let us know what you've come up with so we can share it. If you'd like to improve this widget, also be our guest to submit a pull request and we'll make sure it'll get in, so we can make this widget better together.


