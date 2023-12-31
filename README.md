# a_tutorial

[![sampctl](https://img.shields.io/badge/sampctl-a_tutorial-2f2f2f.svg?style=for-the-badge)](https://github.com/Gomaink/a_tutorial)


This library allows you to create tutorials dynamically and quickly.


![](https://i.imgur.com/YnAgCK8.png)

## Installation

Simply install to your project:

```bash
sampctl package install Gomaink/a_tutorial
```

Include in your code and begin using the library:

```pawn
#include <a_tutorial>
```

## Functions 

```pawn
Tutorial::Create(tutorialid)
Tutorial::Delete(tutorialid)
Tutorial::AddLocal(tutorialid, localid, msg[MAX_TUTORIAL_CHARS], Float:cposx, Float:cposy, Float:cposz, Float:clookx, Float:clooky, Float:clookz)
Tutorial::Init(playerid, const tutorialid, const callback[])
Tutorial::Stop(playerid) 
```

## Usage

```pawn
#include "a_tutorial.inc"

 //Tutorial ID
#define TUTORIAL_REGISTER (1)

main(){}
public OnGameModeInit()
{   
    //Creating a new tutorial
    Tutorial::Create(TUTORIAL_REGISTER);
    //Adding new locations and text for each tutorial location
    Tutorial::AddLocal(TUTORIAL_REGISTER, 1, "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque pe", 366.1500, 1762.8300, 158.9100, 365.3100, 1763.3800, 158.0600);
    Tutorial::AddLocal(TUTORIAL_REGISTER, 2, "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque pe", 331.2362, 1549.4685, 120.5536, 330.5472, 1548.7384, 119.9235);
    return 1;
}

public OnPlayerConnect(playerid)
{
    //Starting the tutorial
    Tutorial::Init(playerid, TUTORIAL_REGISTER, "OnRegisterTutorialEnd");
    return 1;
}

forward OnRegisterTutorialEnd(playerid);
public OnRegisterTutorialEnd(playerid)
{
    SetCameraBehindPlayer(playerid);
    SetPlayerPos(playerid, 846.3976, -1384.1038, -0.5015);
    SetPlayerFacingAngle(playerid, 318.4364);
    return 1;
}
```

## Testing

<!--
Depending on whether your package is tested via in-game "demo tests" or
y_testing unit-tests, you should indicate to readers what to expect below here.
-->

To test, simply run the package:

```bash
sampctl package run
```
