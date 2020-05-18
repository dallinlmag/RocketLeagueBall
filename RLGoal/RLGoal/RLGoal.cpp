#include "pch.h"
#include "RLGoal.h"
#pragma comment(lib, "ws2_32.lib")
#include "..\HTTPRequest\include\HTTPRequest.hpp"


BAKKESMOD_PLUGIN(RLGoal, "RLBall", plugin_version, PLUGINTYPE_SPECTATOR)

//From CinderBlock
int playerData[2][3][3]; //2 teams, 3 players, 3 data points: spectator shortcut, goals, assists
string teamThatScored = "";
string playerWhoScored = "";
string playerWhoAssisted = "";
bool playerGotHatTrick = false;


void RLGoal::onUnload()
{
}

void RLGoal::startWatching() {
}

void RLGoal::onLoad()
{

    gameWrapper->HookEvent("Function TAGame.Team_TA.EventScoreUpdated", bind(&RLGoal::goalScored, this));
    gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.EventMatchEnded", bind(&RLGoal::gameEnded, this));

    //hook the goal scored event and call a function to get assists and goals
    //gameWrapper->HookEvent("Function TAGame.Team_TA.EventScoreUpdated", bind(&RLGoal::goalScored, this));
}

void test()
{
    ///hello
}
// sendHTTP is meant to run on another thread to handle http calls to not slow, freeze, or crash the game
void RLGoal::sendHTTP(std::string url)
{
    try
    {
        
        http::Request request(url);

        // send a get request
        const http::Response response = request.send("GET");
        cvarManager->log(std::string(response.body.begin(), response.body.end())); // print the result
    }
    catch (const std::exception& e)
    {
        cvarManager->log("Request failed, error: " + std::string(e.what()));
    }
}

void RLGoal::gameEnded() 
{

    cvarManager->log("Game ended Recorded");
    std::thread httpThread(&RLGoal::sendHTTP, this, "192.168.1.88/win");
    httpThread.detach();
}

void RLGoal::goalScored() 
{
    //Quick and dirty send a message to a hard coded IP Address that a goal was scored...  

    cvarManager->log("goal Recorded");
    std::thread httpThread(&RLGoal::sendHTTP, this, "192.168.1.88/goal");
    httpThread.detach();

}

//OFF Limits until the API is fixed
void RLGoal::getPlayerIDs()
{
    if (gameWrapper->IsInOnlineGame())
    {
        ServerWrapper server = gameWrapper->GetOnlineGame();
        ArrayWrapper<TeamWrapper> teams = server.GetTeams();

        for (int i = 0; i < 2; i++) //Each team
        {
            for (int j = 0; j < 3; j++) //Each Player
            {
                if (!teams.Get(i).IsNull()) {
                    cvarManager->log("Getting info");
                    auto members = teams.Get(i).GetMembers();
                    cvarManager->log("Got Member Info");
                    auto player = members.Get(0);
                    cvarManager->log(to_string(teams.Get(i).GetScore()));
                    cvarManager->log(player.GetPlayerName().ToString());


                    //PriWrapper playerInfo = teams.Get(i).GetMembers().Get(j);
                    //cvarManager->log("Got Player Info");
                    //playerData[i][j][0] = playerInfo.GetSpectatorShortcut();
                    //playerData[i][j][1] = 0;
                    //playerData[i][j][2] = 0;
                }
                else {
                    cvarManager->log("Could Not get teams");
                }
            }
        }
        //printGameData(teams);
    }
    cvarManager->log("Game Started");
}



//OFF Limits until the API is fixed
//void RLGoal::goalScored()
//{
//    if (gameWrapper->IsInOnlineGame())
//    {
//        ServerWrapper server = gameWrapper->GetOnlineGame();
//        ArrayWrapper<TeamWrapper> teams = server.GetTeams();
//
//        for (int i = 0; i < 2; i++)
//        {
//            for (int j = 0; j < 3; j++)
//            {
//                PriWrapper playerInfo = teams.Get(i).GetMembers().Get(j);
//                int currentSpecShortcut = playerInfo.GetSpectatorShortcut();
//                for (int x = 0; x < 2; x++)//check who scored or assisted
//                {
//                    for (int y = 0; y < 3; y++)
//                    {
//                        if (currentSpecShortcut == playerData[x][y][0])
//                        {
//                            if (playerInfo.GetMatchGoals() > playerData[x][y][1])
//                            {
//                                if (!playerInfo.GetTeam().GetTeamName().IsNull())
//                                {
//                                    teamThatScored = playerInfo.GetTeam().GetTeamName().ToString();
//                                }
//                                else if (playerInfo.GetTeam().GetTeamNum() == 0)
//                                    teamThatScored = "BLUE";
//                                else
//                                    teamThatScored = "ORANGE";
//                                playerWhoScored = playerInfo.GetPlayerName().ToString();
//                                cvarManager->log(playerWhoScored + "Scored!");
//                                if (playerInfo.GetMatchGoals() % 3 == 0)
//                                    playerGotHatTrick = true;
//                            }
//                            if (playerInfo.GetMatchAssists() > playerData[x][y][2])
//                                playerWhoAssisted = playerInfo.GetPlayerName().ToString();
//                        }
//                    }
//                }
//            }
//        }
//    }
//}

void RLGoal::printGameData(ArrayWrapper<TeamWrapper> teams) {
    for (int i = 0; i < 2; i++) //Each team
    {
        teams.Get(i).GetTeamName();
        std::string teamName;
        if (!teams.Get(i).GetTeamName().IsNull())
        {
            teamName = teams.Get(i).GetTeamName().ToString();
        }
        else if (teams.Get(i).GetTeamNum() == 0)
            teamName = "BLUE";
        else
            teamName = "ORANGE";

        cvarManager->log("Team: " + teamName);


        for (int j = 0; j < 3; j++) //Each Player
        {
            PriWrapper playerInfo = teams.Get(i).GetMembers().Get(j);

            cvarManager->log("Player: " + playerInfo.GetPlayerName().ToString());
            //playerData[i][j][0] = playerInfo.GetSpectatorShortcut();
            //playerData[i][j][1] = 0;
            //playerData[i][j][2] = 0;
            

        }
    }
}

ServerWrapper RLGoal::GetCurrentGameState()
{
    if (gameWrapper->IsInReplay())
        return gameWrapper->GetGameEventAsReplay().memory_address;
    else if (gameWrapper->IsInOnlineGame())
        return gameWrapper->GetOnlineGame();
    else
        return gameWrapper->GetGameEventAsServer();
}