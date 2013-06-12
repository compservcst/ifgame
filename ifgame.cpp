#include <iostream>
#include <vector>
#include <sstream>
#include <map>

using namespace std;

class item
{
     public:
        string name;
        string description;
        bool getable;
        item(string n, string d, bool g)
        {
            name = n;
            description = d;
            getable = g;
        }
};

class npc
{
      public:
             npc(string n, string d)
             {
                 name = n;
                 description = d;
             }
             string name;
             string description;
             map<string, string> responses;
             void addResponse(string q, string a)
             {
                 responses[q] = a;
             }
};

class room
{
      public:
             room(string rn, string d, room* n, room* s, room* w, room* e)
             {
                 roomName = rn;
                 description = d;
                 toNorth = n;
                 toSouth = s;
                 toWest = w;
                 toEast = e;
             }
             void addItem(item* item)
             {
                 items.push_back(item);
             }
             void addNpc(npc* npc)
             {
                 npcs[npc->name] = npc;
             }
             void connectRoom(room* rm, char direction)
             {
                 switch(direction)
                 {
                     case 'n':
                          toNorth = rm;
                          toNorth->toSouth = this;
                          break;
                     case 's':
                          toSouth = rm;
                          toSouth->toNorth = this;
                          break;
                     case 'e':
                          toEast = rm;
                          toEast->toWest = this;
                          break;
                     case 'w':
                          toWest = rm;
                          toWest->toEast = this;
                          break;

                 }
             }
             vector<item*> items;
             map<string, npc*> npcs;
             string roomName;
             string description;
             room* toNorth;
             room* toSouth;
             room* toWest;
             room* toEast;
};

class player
{
      public:
             player(string n)
             {
                 name = n;
             }
             vector<item*> inventory;
             room* location;
             string name;
             void look()
             {
                  cout << location->roomName << endl << location->description << endl;
                  if(location->items.size() > 0)
                  {
                      if(location->items.size() > 1)
                      {
                          cout << "There are ";
                          for(vector<item*>::iterator it = location->items.begin(); it != location->items.end(); it++)
                          {
                              cout << (*it)->name << ", ";
                          }
                          cout << " here." << endl;
                      }
                      else
                      {
                          cout << "There is a " << (location->items.front())->name << " here." << endl;
                      }
                  }
                  else
                  {
                      cout << "There are no items here." << endl;
                  }
                  for(map<string, npc*>::iterator it = location->npcs.begin(); it != location->npcs.end(); it++)
                  {
                      cout << "There is a " << it->first << " here." << endl;
                  }

             }
             bool move(string direction)
             {
                 bool moved = false;
                 if(direction == "north" || direction == "n")
                 {
                     if(location->toNorth == NULL)
                     {
                         cout << "You can't move north." << endl;
                     }
                     else
                     {
                         location = location->toNorth;
                         moved = true;
                     }
                 }
                 if(direction == "south" || direction == "s")
                 {
                     if(location->toSouth == NULL)
                     {
                         cout << "You can't move south." << endl;
                     }
                     else
                     {
                         location = location->toSouth;
                         moved = true;
                     }
                 }
                 if(direction == "west" || direction == "w")
                 {
                     if(location->toWest == NULL)
                     {
                         cout << "You can't move west." << endl;
                     }
                     else
                     {
                         location = location->toWest;
                         moved = true;
                     }
                 }
                 if(direction == "east" || direction == "e")
                 {
                     if(location->toEast == NULL)
                     {
                         cout << "You can't move east." << endl;
                     }
                     else
                     {
                         location = location->toEast;
                         moved = true;
                     }
                 }
                 if(moved == true)
                 {
                     look();
                 }
                 return moved;
             }
             void teleport(room* rm)
             {
                 if(rm)
                 {
                     location = rm;
                 }
             }
             void take(string iname)
             {
                 bool found = false;
                 for(vector<item*>::iterator it = location->items.begin(); it != location->items.end(); it++)
                 {
                     if((*it)->name == iname)
                     {
                         found = true;
                         if((*it)->getable == false)
                         {
                             cout << "You can't take the " << (*it)->name << "." << endl;
                             break;
                         }
                         else
                         {
                             inventory.push_back(*it);
                             location->items.erase(it);
                             cout << iname << " taken." << endl;
                         }
                         break;
                     }
                 }
                 if(found == false)
                 {
                     cout << "There is no " << iname << " here." << endl;
                 }
             }
             void examine(string iname)
             {
                 bool found = false;
                 for(vector<item*>::iterator it = location->items.begin(); it != location->items.end(); it++)
                 {
                     if((*it)->name == iname)
                     {
                         cout << (*it)->description << endl;
                         found = true;
                         break;
                     }
                 }
                 for(vector<item*>::iterator it = inventory.begin(); it != inventory.end(); it++)
                 {
                     if((*it)->name == iname)
                     {
                         cout << (*it)->description << endl;
                         found = true;
                         break;
                     }
                 }
                 if(location->npcs.find(iname) != location->npcs.end())
                 {
                      cout << (location->npcs[iname])->description << endl;
                      found = true;
                 }
                 if(found == false)
                 {
                      cout << "There is no " << iname << " here." << endl;
                 }
             }
             void ask(string iname, string question)
             {
                 if(location->npcs.find(iname) != location->npcs.end())
                 {
                      if((location->npcs[iname])->responses.find(question) != (location->npcs[iname])->responses.end())
                      cout << "The " << iname << " says: \"" << (location->npcs[iname])->responses[question] << "\"" << endl;
                 }
                 else
                 {
                      cout << "The " << iname << " doesn't know anything about that." << endl;
                 }
             }
             void drop(string iname)
             {
                 bool found = false;
                 for(vector<item*>::iterator it = inventory.begin(); it != inventory.end(); it++)
                 {
                     if((*it)->name == iname)
                     {
                         found = true;
                         location->addItem(*it);
                         inventory.erase(it);
                         cout << "Dropped." << endl;
                         break;
                     }
                 }
                 if(found == false)
                 {
                      cout << "You aren't carrying that." << endl;
                 }
             }
             void printInv()
             {
                 for(vector<item*>::iterator it = inventory.begin(); it != inventory.end(); it++)
                 {
                     cout << (*it)->name;
                     if(distance(it, inventory.end()) == 1)
                     {
                         cout << "." << endl;
                     }
                     else
                     {
                         cout << ", ";
                     }
                 }
             }
};

vector<string> splitLine(string input)
{
     stringstream ss(input);
     vector<string> tokens;

     string buf;
     while(ss.good())
     {
         ss >> buf;
         tokens.push_back(buf);
         buf = string();
     }
     return tokens;
}

player* Bob = new player("Bob");

int prompt()
{
     cout << ">";
     string input;
     getline(cin, input);
     vector<string> words(splitLine(input));
     /*
     for(vector<string>::iterator it = words.begin(); it != words.end(); it++)
     {
         cout << *it << "-";
     }
     */

     if(input == "l" || input == "look")
     {
         Bob->look();
     }
     else if(input == "i" || input == "inventory")
     {
         cout << "Your inventory contains: " << endl;
         Bob->printInv();
         cout << endl;
     }
     else if(input == "quit")
     {
         cout << "Quitting..." << endl;
         return 0;
     }
     else if(input == "help")
     {
         cout << "-----------------------" << endl;
         cout << "Available commands: " << endl;
         cout << "look / l" << endl;
         cout << "north/n , south/s , east/e , west/w" << endl;
         cout << "examine/x ITEM" << endl;
         cout << "take/get ITEM" << endl;
         cout << "drop ITEM" << endl;
         cout << "inventory/i" << endl;
         cout << "ask NPC_NAME about QUESTION" << endl;
         cout << "quit" << endl;
         cout << "-----------------------" << endl;
     }
     else if((words.at(0) == "take" || words.at(0) == "get") && words.size() > 1)
     {
          Bob->take(words.at(1));
     }
     else if(words.at(0) == "drop" && words.size() > 1)
     {
          Bob->drop(words.at(1));
     }
     else if((words.at(0) == "examine" || words.at(0) == "x") && words.size() > 1)
     {
          Bob->examine(words.at(1));
     }
     else if(Bob->move(words.at(0)) == true)
     {
     }
     else if((words.at(0) == "ask" && words.at(2) == "about") && words.size() > 3)
     {
         Bob->ask(words.at(1), words.at(3));
     }
     else
     {
         cout << "No." << endl;
     }
     return 1;
}

room* headHall = new room("Long Hallway", "You are standing at the beginning of a long hallway.\nThe hallway stretches about 1000m to the north.", NULL, NULL, NULL, NULL);

int main()
{
    Bob->teleport(headHall);
    item* iBuf = new item("note", "The note reads: \"There is treasure at the other end of this hallway. Good luck!\"", true);
    npc* nBuf = NULL;
    room* rBuf = NULL;
    headHall->addItem(iBuf);

    room* proom = headHall;
    for(int i = 0; i < 100; i++)
    {
        rBuf = new room("Long Hallway", "You are in between the two ends of the hallway.", NULL, NULL, NULL, NULL);
        proom->connectRoom(rBuf, 'n');
        proom = proom->toNorth;
    }
    proom->roomName = "End of hallway";
    proom->description = "You are standing at the northern end of the hallway.";
    proom->toNorth = NULL;
    nBuf = new npc("troll", "It is a huge, haughty troll.");
    nBuf->addResponse("treasure", "There is no treasure here.");
    proom->addNpc(nBuf);

    cout << "Annoyotron" << endl << endl;
    Bob->look();
    int status = 1;

    while(status != 0)
    {
        status = prompt();
    }

    //system("PAUSE"); //windows only crap
    //std::cin.get();
    return 0;
}
