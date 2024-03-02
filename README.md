# sc4-exemplar-load-logging

A DLL Plugin for SimCity 4 that adds logging to the game's exemplar loading process.   

This is a experimental plugin, you should not use it in a region or city you care about.
The logging will also slow down your game.

The plugin can be downloaded from the Releases tab: https://github.com/0xC0000054/sc4-exemplar-load-logging/releases

## Technical Details

The plugin replaces the exemplar loading class that the game's resource manager uses with a proxy/wrapper class that
forwards the request to the game's original exemplar loading class and logs the returned info.

## System Requirements

* SimCity 4 version 641
* Windows 10 or later

The plugin may work on Windows 7 or later with the [Microsoft Visual C++ 2022 x86 Redistribute](https://aka.ms/vs/17/release/vc_redist.x86.exe) installed, but I do not have the ability to test that.

## Installation

1. Close SimCity 4.
2. Copy `SC4ExemplarLoadLogging.dll` into the Plugins folder in the SimCity 4 installation directory.
3. Start SimCity 4.

## Troubleshooting

The plugin should write a `SC4ExemplarLoad.log` file in the same folder as the plugin.    
The log contains status information for the most recent run of the plugin.

# License

This project is licensed under the terms of the MIT License.    
See [LICENSE.txt](LICENSE.txt) for more information.

## 3rd party code

[gzcom-dll](https://github.com/nsgomez/gzcom-dll/tree/master) Located in the vendor folder, MIT License.    
[EABase](https://github.com/electronicarts/EABase) Located in the vendor folder, BSD 3-Clause License.    
[EASTL](https://github.com/electronicarts/EASTL) Located in the vendor folder, BSD 3-Clause License.    
[Windows Implementation Library](https://github.com/microsoft/wil) - MIT License    

# Source Code

## Prerequisites

* Visual Studio 2022

## Building the plugin

* Open the solution in the `src` folder
* Update the post build events to copy the build output to you SimCity 4 application plugins folder.
* Build the solution

## Debugging the plugin

Visual Studio can be configured to launch SimCity 4 on the Debugging page of the project properties.
I configured the debugger to launch the game in full screen with the following command line:    
`-intro:off -CPUcount:1 -f -CustomResolution:enabled -r2560x1440x32`

You will need to have the debugger open on a second screen, and you may need to adjust the resolution for your primary screen.
