#ifndef KITCHEN_H
#define KITCHEN_H
#include "simulator.h"
#include <tr1/unordered_map>


enum ObjectClass {APPLEJUICE = 0, CALGONIT, GRANINI, MEASURINGCUP, RICEBOX, CEREAL, PLATE, CUP};

enum LocationType {CUPBOARD = 8, DISHWASHER, FRIDGE, SIDEBOARD, STOVE, NO_LOCATION};

enum GripperType {LEFT = 14, RIGHT, NO_GRIPPER};

enum ActionType {CLOSE = 17, GRASP, GRASP_FROM_EDGE, MOVE_ROBOT, NUDGE, OPEN, OPEN_PARTIAL, OPEN_COMPLETE,
    PASS_OBJECT, PLACE_UPRIGHT, PUT_DOWN, PUT_IN, REMOVE_FROM};

//typedef std::tr1::unordered_map< ObjectClass, int, std::tr1::hash<int> >  hashmap;
    
struct KitchenAction
{
    ActionType type;
    ObjectClass objectclass;
    int objectindex;
    LocationType location;
    LocationType location2;
    GripperType gripper;
    GripperType gripper2;
};

class KITCHEN_STATE : public STATE
{
public:
    LocationType RobotLocation;
    std::vector<bool> AtEdge;
    //std::vector<bool> IsFlat;
    std::vector<bool> GripperEmpty;
    //std::vector<bool> IsHand;
    std::vector<GripperType> InWhichGripper;
    //std::vector<bool> IsObject;
    std::vector<LocationType> ObjectLocations;
    std::vector<bool> LocationOpen;
    std::vector<bool> LocationPartiallyOpen;
    std::vector<bool> IsToppled;
};

class KITCHEN : public SIMULATOR
{
public:
    
    KITCHEN(int nplates = 1, int ncups = 1);
    
    virtual STATE* Copy(const STATE& state) const;
    virtual void Validate(const STATE& state) const;
    virtual STATE* CreateStartState() const;
    virtual void FreeState(STATE* state) const;
    virtual bool Step(STATE& state, int action, 
        int& observation, double& reward) const;
	
    void GenerateLegal(const STATE& state, const HISTORY& history,
        std::vector<int>& legal, const STATUS& status) const;
    void GeneratePreferred(const STATE& state, const HISTORY& history,
        std::vector<int>& actions, const STATUS& status) const;
	
    virtual void DisplayState(const STATE& state, std::ostream& ostr) const;
    virtual void DisplayAction(int action, std::ostream& ostr) const;
    
protected:
    
    int NumPlates;
    int NumCups;
    int NumLocations;
    std::vector<ObjectClass> ObjectTypes;
    bool IsLocation(const int& l) const { return l >= CUPBOARD && l <= STOVE; }
    bool IsObject(const int& o) const { return o >= APPLEJUICE && o <= CUP; }
    bool IsFlat(const int& o) const { return o == PLATE; }
    bool IsHand(const int& h) const {return h == LEFT || h == RIGHT;}
    bool HaveAppleJuice, HaveCalgonit, HaveGranini, HaveMeasuringCup, HaveRiceBox, HaveCereal;
    int NumObjects;
    int LOCATION_OFFSET, GRIPPER_OFFSET, ACTION_OFFSET;
    bool TestCerealInCupboard, TestPlate1InDishwasher, TestAppleJuiceInFridge;
    bool IsCerealInCupboard(const KITCHEN_STATE& state, double& reward) const;
    bool IsPlate1InDishwasher(const KITCHEN_STATE& state, double& reward) const;
    bool IsAppleJuiceInFridge(const KITCHEN_STATE& state, double& reward) const;
    int CerealIndex, Plate1Index, AppleJuiceIndex;
    //std::tr1::unordered_map< int, int > ObjectIndexMap;
    
    int ActionToInt(const KitchenAction& ka) const;
    KitchenAction IntToAction(int action) const;
    LocationType IntToLocation(const int& i) const;
    GripperType IntToGripper(const int& i) const;
    std::string LocationToString(const LocationType& t) const;
    std::string ActionToString(const ActionType& t) const;
    std::string ObjectToString(const ObjectClass& t) const;
    std::string GripperToString(const GripperType& t) const;
    
    std::vector<int> PreferredObjects;
    std::vector<LocationType> PreferredLocations;
    
private:
    mutable MEMORY_POOL<KITCHEN_STATE> MemoryPool;
    
};

#endif