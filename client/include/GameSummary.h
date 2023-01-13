#pragma once
#include "event.h"
#include <unordered_map>
#include <algorithm>

class GameSummary
{
    public:
    GameSummary();
    // GameSummary(std::string username);
    void saveSummaryToFile(std::string gamename,std::string filename);
    void addEventToGame(std::string gameName,Event event);
    std::vector<Event> getEvents(std::string gamename);
    bool hasSummary(std::string gamename);
    std::string getSummaryTitle(std::string gameName);
    std::string getGeneralStats(std::string gameName);
    std::string getTeamAStats(std::string gameName);
    std::string getTeamBStats(std::string gameName);
    std::string firstTeam(std::string gameName);
    std::string secondTeam(std::string gameName);
    std::string getEventsReport(std::string gameName);

    private:
        std::string username;
        std::unordered_map<std::string,std::vector<Event>> gameNameToEvents;
     
    

};