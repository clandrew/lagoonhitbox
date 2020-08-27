# Lagoon (SNES) Hitbox Modification ROM Patching Tool

## Usage

This is a command-line tool. The usage is

``` 
lhbm.exe [input file] [output file] [hitboxSizeMultiplierForward] [hitboxMultiplierSides]
```

For example,

``` 
lhbm.exe input.smc output.smc 3 2
```

[input file] and [output file] are paths to an (unarchived) ROM file. The tool allows both headered and non-headered ROMs.

[hitboxSizeMultiplierForward] multiplies the hitbox sizes in the direction NASIR is facing. For example, when NASIR is facing right, a multiplier of 3 will move the right-hand-side of the box to the right by a multiple of 3 compared to the default.

[hitboxMultiplierSides] multiplies the hitbox sizes perpendicular to the direction NASIR is facing. For example, when NASIR is facing right, a multiplier of 2 will move the top and bottom edges of the hit box by a multiple of 2 compared to the default.

Result example:

![Example image](http://secretplace.cml-a.com/images/hitbox1.png "Example image")
![Example image](http://secretplace.cml-a.com/images/hitbox2.png "Example image")

![Example image](http://secretplace.cml-a.com/images/hitbox3.png "Example image")
![Example image](http://secretplace.cml-a.com/images/hitbox4.png "Example image")

## Technical Document
Find a document with an explanation of the patcher and the code that it modifies [here](https://raw.githubusercontent.com/clandrew/lagoonhitbox/master/lagoon_patch_info.txt).

## Build
The code is organized and built as a Visual Studio 2019 solution written in C++. 

It has been tested on an x86-64-compatible Windows 10 environment.
