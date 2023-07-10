#if defined tutorial_included
	#endinput
#endif
#define tutorial_included

#include <a_samp>

//Macros
#define Tutorial:: \
	Tutorial_

//Callbacks
forward TutorialChecker(playerid, tutorialid, localid);

#define MAX_TUTORIALS 10 //Change as per your need 
#define MAX_LOCALS 20
#define MAX_TUTORIAL_CHARS 128
#define MAX_CALLBACK_CHAR 30
#define MAX_TUTORIAL_TEXTDRAW 2
#define TIME_BETWEEN_LOCALS 10*1000 //10 segs

enum tutorialData {
	bool:tutorialExists,
	tutorialCallBack[MAX_CALLBACK_CHAR],
	PlayerText:tutorialTextdrawBox[MAX_LOCALS],
	PlayerText:tutorialTextdrawMessage[MAX_LOCALS],
	Float:tutorialCPosX[MAX_LOCALS],
	Float:tutorialCPosY[MAX_LOCALS],
	Float:tutorialCPosZ[MAX_LOCALS],
	Float:tutorialCLookX[MAX_LOCALS],
	Float:tutorialCLookY[MAX_LOCALS],
	Float:tutorialCLookZ[MAX_LOCALS]
} new TutorialData[MAX_TUTORIALS][tutorialData];
new bool:TutorialPassed[MAX_PLAYERS][MAX_LOCALS], Tutorial[MAX_PLAYERS], TutorialTimer[MAX_PLAYERS], TutorialMessage[MAX_TUTORIALS][MAX_LOCALS];

public OnPlayerDisconnect(playerid, reason)
{
	Tutorial::Stop(playerid);
	#if defined Tutorial_OnPlayerDisconnect
		return Tutorial_OnPlayerDisconnect(playerid, reason);
	#else
		return 1;
	#endif
}

#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect Tutorial_OnPlayerDisconnect
#if defined Tutorial_OnPlayerDisconnect
	forward Tutorial_OnPlayerDisconnect(playerid, reason);
#endif

stock Tutorial::Create(tutorialid)
{
	TutorialData[tutorialid][tutorialExists] = true; 
	return 1;
}

stock Tutorial::Delete(tutorialid) 
{
	TutorialData[tutorialid][tutorialExists] = false;
	for(new localid; localid < MAX_LOCALS; localid++){
		
		if(TutorialData[tutorialid][tutorialCPosX][localid] != 0.0)
		{
			TutorialData[tutorialid][tutorialCPosX][localid] = 0;
			TutorialData[tutorialid][tutorialCPosY][localid] = 0;
			TutorialData[tutorialid][tutorialCPosZ][localid] = 0;

			TutorialData[tutorialid][tutorialCLookX][localid] = 0;
			TutorialData[tutorialid][tutorialCLookY][localid] = 0;
			TutorialData[tutorialid][tutorialCLookZ][localid] = 0;
			PlayerTextDrawDestroy(playerid, TutorialData[tutorialid][tutorialTextdrawBox][localid]);
			PlayerTextDrawDestroy(playerid, TutorialData[tutorialid][tutorialTextdrawMessage][localid]);
		}
		
	}
	return 1;
}
stock Tutorial::AddLocal(tutorialid, localid, msg[MAX_TUTORIAL_CHARS], Float:cposx, Float:cposy, Float:cposz, Float:clookx, Float:clooky, Float:clookz)
{
	if(TutorialData[tutorialid][tutorialExists] == true)
	{
		format(TutorialMessage[tutorialid][localid], MAX_TUTORIAL_CHARS, msg);
		TutorialData[tutorialid][tutorialCPosX][localid] = cposx;
		TutorialData[tutorialid][tutorialCPosY][localid] = cposy;
		TutorialData[tutorialid][tutorialCPosZ][localid] = cposz;

		TutorialData[tutorialid][tutorialCLookX][localid] = clookx;
		TutorialData[tutorialid][tutorialCLookY][localid] = clooky;
		TutorialData[tutorialid][tutorialCLookZ][localid] = clookz;
	}
	return 1;
}

stock Tutorial::Init(playerid, const tutorialid, const callback[]) 
{
	if(TutorialData[tutorialid][tutorialExists] == true)
	{
		if(TutorialData[tutorialid][tutorialCPosX][1] != 0.0)
		{
			TogglePlayerSpectating(playerid, true);
			Tutorial[playerid] = tutorialid;
			format(TutorialData[tutorialid][tutorialCallBack], MAX_CALLBACK_CHAR, callback);
			TutorialTimer[playerid] = SetTimerEx("TutorialChecker", 1000, false, "iii", playerid, tutorialid, 0);
		}
	}
	return 1;
}

stock Tutorial::Stop(playerid) 
{
	if(TutorialPassed[playerid][1])
	{
		KillTimer(TutorialTimer[playerid]);
		TogglePlayerSpectating(playerid, false);
		for(new localid; localid < MAX_LOCALS; localid++){
			TutorialPassed[playerid][localid] = false;
			PlayerTextDrawHide(playerid, TutorialData[Tutorial[playerid]][tutorialTextdrawBox][localid]);
			PlayerTextDrawHide(playerid, TutorialData[Tutorial[playerid]][tutorialTextdrawMessage][localid]);
		}
		Tutorial[playerid] = 0;
	}
	return 1;
}


public TutorialChecker(playerid, tutorialid, localid)
{
	new last_localid = localid;
	localid++;

	if(localid >= 20){
		Tutorial::Stop(playerid); CallLocalFunction(TutorialData[tutorialid][tutorialCallBack], "%d", playerid); return 1;}
	
	if(TutorialPassed[playerid][localid] == true){
		TutorialTimer[playerid] = SetTimerEx("TutorialChecker", 100, false, "iii", playerid, tutorialid, localid); return 1;}
	
	if(TutorialData[tutorialid][tutorialCPosX][localid] == 0.0){
		TutorialTimer[playerid] = SetTimerEx("TutorialChecker", 100, false, "iii", playerid, tutorialid, localid); return 1;}
	
	PlayerTextDrawHide(playerid, TutorialData[tutorialid][tutorialTextdrawBox][last_localid]);
	PlayerTextDrawHide(playerid, TutorialData[tutorialid][tutorialTextdrawMessage][last_localid]);

	TutorialData[tutorialid][tutorialTextdrawBox][localid] = CreatePlayerTextDraw(playerid, 72.000000, 110.000000, "_");
	PlayerTextDrawFont(playerid, TutorialData[tutorialid][tutorialTextdrawBox][localid], 1);
	PlayerTextDrawLetterSize(playerid, TutorialData[tutorialid][tutorialTextdrawBox][localid], 0.600000, 9.900001);
	PlayerTextDrawTextSize(playerid, TutorialData[tutorialid][tutorialTextdrawBox][localid], 298.500000, 132.500000);
	PlayerTextDrawSetOutline(playerid, TutorialData[tutorialid][tutorialTextdrawBox][localid], 1);
	PlayerTextDrawSetShadow(playerid, TutorialData[tutorialid][tutorialTextdrawBox][localid], 0);
	PlayerTextDrawAlignment(playerid, TutorialData[tutorialid][tutorialTextdrawBox][localid], 2);
	PlayerTextDrawColor(playerid, TutorialData[tutorialid][tutorialTextdrawBox][localid], -1);
	PlayerTextDrawBackgroundColor(playerid, TutorialData[tutorialid][tutorialTextdrawBox][localid], 255);
	PlayerTextDrawBoxColor(playerid, TutorialData[tutorialid][tutorialTextdrawBox][localid], 135);
	PlayerTextDrawUseBox(playerid, TutorialData[tutorialid][tutorialTextdrawBox][localid], 1);
	PlayerTextDrawSetProportional(playerid, TutorialData[tutorialid][tutorialTextdrawBox][localid], 1);
	PlayerTextDrawSetSelectable(playerid, TutorialData[tutorialid][tutorialTextdrawBox][localid], 0);

	TutorialData[tutorialid][tutorialTextdrawMessage][localid] = CreatePlayerTextDraw(playerid, 9.000000, 112.000000, TutorialMessage[tutorialid][localid]);
	PlayerTextDrawFont(playerid, TutorialData[tutorialid][tutorialTextdrawMessage][localid], 1);
	PlayerTextDrawLetterSize(playerid, TutorialData[tutorialid][tutorialTextdrawMessage][localid], 0.216666, 1.300000);
	PlayerTextDrawTextSize(playerid, TutorialData[tutorialid][tutorialTextdrawMessage][localid], 133.000000, 17.000000);
	PlayerTextDrawSetOutline(playerid, TutorialData[tutorialid][tutorialTextdrawMessage][localid], 1);
	PlayerTextDrawSetShadow(playerid, TutorialData[tutorialid][tutorialTextdrawMessage][localid], 0);
	PlayerTextDrawAlignment(playerid, TutorialData[tutorialid][tutorialTextdrawMessage][localid], 1);
	PlayerTextDrawColor(playerid, TutorialData[tutorialid][tutorialTextdrawMessage][localid], -1);
	PlayerTextDrawBackgroundColor(playerid, TutorialData[tutorialid][tutorialTextdrawMessage][localid], 255);
	PlayerTextDrawBoxColor(playerid, TutorialData[tutorialid][tutorialTextdrawMessage][localid], 50);
	PlayerTextDrawUseBox(playerid, TutorialData[tutorialid][tutorialTextdrawMessage][localid], 0);
	PlayerTextDrawSetProportional(playerid, TutorialData[tutorialid][tutorialTextdrawMessage][localid], 1);
	PlayerTextDrawSetSelectable(playerid, TutorialData[tutorialid][tutorialTextdrawMessage][localid], 0);

	PlayerTextDrawShow(playerid, TutorialData[tutorialid][tutorialTextdrawBox][localid]);
	PlayerTextDrawShow(playerid, TutorialData[tutorialid][tutorialTextdrawMessage][localid]);
	SetPlayerCameraPos(playerid, TutorialData[tutorialid][tutorialCPosX][localid], TutorialData[tutorialid][tutorialCPosY][localid], TutorialData[tutorialid][tutorialCPosZ][localid]);
	SetPlayerCameraLookAt(playerid, TutorialData[tutorialid][tutorialCLookX][localid], TutorialData[tutorialid][tutorialCLookY][localid], TutorialData[tutorialid][tutorialCLookZ][localid]);
	TutorialTimer[playerid] = SetTimerEx("TutorialChecker", TIME_BETWEEN_LOCALS, false, "iii", playerid, tutorialid, localid);
	TutorialPassed[playerid][localid] = true;
	return 1;
}
