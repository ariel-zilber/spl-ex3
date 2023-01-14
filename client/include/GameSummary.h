#pragma once
#include "event.h"
#include <unordered_map>
#include <algorithm>

class GameSummary
{
    public:

    // constructors
    GameSummary();

    // methods:

    /**
     * Record the event
     * @param gameName
     * @param event
    */
    void addEventToGame(std::string gameName,Event event);


    /***
     * Checks whenever a given gamename has summary records
     * @param gamename
    */
    bool hasSummary(std::string gamename);

    /***
     * Returns all events
     * @param gamename
     * @return all events as vector 
    */
    std::vector<Event> getEvents(std::string gamename);

    /***
     * Generate the  summary title for a given game
     * @param gameName
    */
    std::string getSummaryTitle(std::string gameName);

    /***
     * Generate general stats as a string
     * @param gameName
    */
    std::string getGeneralStats(std::string gameName);

    /***
     * Generate Team A stats as a string
     * @param gameName
    */
    std::string getTeamAStats(std::string gameName);

    /***
     * Generate Team B stats as a string
     * @param gameName
    */
    std::string getTeamBStats(std::string gameName);

    /**
     * Get the name of the first team for a given game
     * @param gameName
    */
    std::string firstTeam(std::string gameName);

    /**
     * Get the name of the second team for a given game
     * @param gameName
    */
    std::string secondTeam(std::string gameName);
    
    
    /***
     * Generate events report for a givem gameName as string
     * @param gameName
    */
    std::string getEventsReport(std::string gameName);

    private:
        // data memebers 
        std::string username;
        std::unordered_map<std::string,std::vector<Event>> gameNameToEvents;
     
    

};