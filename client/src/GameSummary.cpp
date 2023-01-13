#include "GameSummary.h"

void GameSummary::saveSummaryToFile(std::string gamename,std::string filename){
};

void GameSummary::addEventToGame(std::string gameName,Event event){
    // 
    if(this->gameNameToEvents.find(gameName)==this->gameNameToEvents.end()){
        this->gameNameToEvents.insert({gameName,std::vector<Event>()});
    };

    //
    this->gameNameToEvents.at(gameName).push_back(event);

};


GameSummary::GameSummary():username(),gameNameToEvents(){
    
};


std::vector<Event> GameSummary::getEvents(std::string gamename){
    return gameNameToEvents[gamename];
};


bool GameSummary::hasSummary(std::string gamename){
    return gameNameToEvents.find(gamename)!=gameNameToEvents.end();
};


 std::string GameSummary::firstTeam(std::string gameName){
    std::string teamA=getEvents(gameName)[0].get_team_a_name();
    transform(teamA.begin(), teamA.end(), teamA.begin(), ::tolower);
    return teamA;
};
std::string GameSummary::secondTeam(std::string gameName){
    std::string teamB=getEvents(gameName)[0].get_team_b_name();
    transform(teamB.begin(), teamB.end(), teamB.begin(), ::tolower);
    return teamB;
};


std::string statsToString(std::map<std::string,std::string> stats){
    std::string output="";
     for(auto pair=stats.begin();pair!=stats.end();pair++){
        output+=pair->first+": "+pair->second+"\n";
    }
    return output;
}
void updateMap(std::map<std::string,std::string> &stats,const std::map<std::string,std::string> &updatedValues  ){
    for(auto pair=updatedValues.begin();pair!=updatedValues.end();pair++){
        std::cout<<"[updateMap] [key] "<<pair->first<<" [value]"<<pair->second<<std::endl;
        stats[pair->first]=pair->second;
    }
}
 
std::string GameSummary::getGeneralStats(std::string gameName){
    std::cout<<"[GameSummary::getGeneralStats]"<<"[gameName] "<<gameName<<std::endl;
    std::map<std::string,std::string> stats;
    std::vector<Event> events=getEvents(gameName);
    for(auto eventIterator=events.begin();eventIterator!=events.end();eventIterator++){
        updateMap(stats,eventIterator->get_game_updates());
    }
    return statsToString(stats);
};
std::string GameSummary::getTeamAStats(std::string gameName){
    std::cout<<"[GameSummary::getTeamAStats]"<<"[gameName] "<<gameName<<std::endl;

    std::map<std::string,std::string> stats;
    std::vector<Event> events=getEvents(gameName);
    for(auto eventIterator=events.begin();eventIterator!=events.end();eventIterator++){
        updateMap(stats,eventIterator->get_team_a_updates());
    }
    return statsToString(stats);
};
std::string GameSummary::getTeamBStats(std::string gameName){
    std::cout<<"[GameSummary::getTeamBStats]"<<"[gameName] "<<gameName<<std::endl;

    std::map<std::string,std::string> stats;
    std::vector<Event> events=getEvents(gameName);
    for(auto eventIterator=events.begin();eventIterator!=events.end();eventIterator++){
        updateMap(stats,eventIterator->get_team_b_updates());
    }
    return statsToString(stats);
};
 
 std::string GameSummary::getEventsReport(std::string gameName){
    std::cout<<"[GameSummary::getEventsReport]"<<"[gameName] "<<gameName<<std::endl;

    std::vector<Event> events=getEvents(gameName);
    std::string output="";
    for(int i=0; i<(int) events.size();i++){
        output+=std::to_string(events[i].get_time())+" - "+events[i].get_name()+":\n\n";
        output+=events[i].get_discription()+"\n\n";
    }
    return output;
 }