#ifndef BOXPUSHING_H
#define BOXPUSHING_H

#include "simulator.h"
#include "coord.h"
#include "grid.h"

#define stringify( name ) # name

struct PUSH_ENTITY
{
    COORD Position;
    int Direction;
    int Length;
};

enum CellContent {NONE = -1, AGENT = 0, SMALL_BOX = 1, LARGE_BOX = 2};
   
enum ActionTypes {STAY = 0, TURN_CW = 1, TURN_CCW = 2, MOVE = 3};

enum ObservationTypes {EMPTY_OBS = 0, WALL_OBS = 1, AGENT_OBS = 2, SMALL_BOX_OBS = 3, LARGE_BOX_OBS = 4, LARGE_BOX_AGENT_OBS = 5};

class BOXPUSHING_STATE : public STATE
{
public:

    struct CELL
    {
        bool Occupied;
	CellContent Content;
    };
    
    GRID<CELL> Cells;
    std::vector<PUSH_ENTITY> Agents;
    std::vector<PUSH_ENTITY> SmallBoxes;
    std::vector<PUSH_ENTITY> LargeBoxes;
    int NumBoxesRemaining;
};

class BOXPUSHING : public SIMULATOR
{
public:

    BOXPUSHING(int numsmallboxes=2, double probLargeAgentBox=0.0);

    virtual STATE* Copy(const STATE& state) const;
    virtual void Validate(const STATE& state) const;
    virtual STATE* CreateStartState() const;
    virtual void FreeState(STATE* state) const;
    virtual bool Step(STATE& state, int action, 
        int& observation, double& reward, STATUS& status) const;
        
    virtual void GeneratePreferred(const STATE& state, const HISTORY& history,
        std::vector<int>& actions, const STATUS& status) const;
    virtual void GeneratePreferredAgent(const STATE& state, const HISTORY& history,
        std::vector<int>& actions, const STATUS& status, const int& index) const;
    virtual void GenerateLegal(const STATE& state, const HISTORY& history,
        std::vector<int>& legal, const STATUS& status) const;
    virtual void GenerateLegalAgent(const STATE& state, const HISTORY& history, 
	std::vector<int>& actions, const STATUS& status, const int& index) const;
    virtual bool LocalMove(STATE& state, const HISTORY& history,
        int stepObs, const STATUS& status) const;

    virtual void DisplayBeliefs(const BELIEF_STATE& beliefState, 
        std::ostream& ostr) const;
    virtual void DisplayState(const STATE& state, std::ostream& ostr) const;
    virtual void DisplayObservation(const STATE& state, int observation, std::ostream& ostr) const;
    virtual void DisplayAction(int action, std::ostream& ostr) const;
    
    virtual bool IsActionMultiagent(const int& action, const HISTORY& history) const;

protected:

    int GetAgentObservation(const BOXPUSHING_STATE& bpstate, const int& agentindex) const;
    double StepAgent(BOXPUSHING_STATE& bpstate, int agentindex, int agentaction) const;
    bool Collision(const BOXPUSHING_STATE& bpstate, const PUSH_ENTITY& pushentity) const;
    void MarkCell(BOXPUSHING_STATE& bpstate, const COORD& coord, const CellContent& content) const;
    void UnmarkCell(BOXPUSHING_STATE& bpstate, const COORD& coord) const;
    void MarkPushEntity(BOXPUSHING_STATE& bpstate, const PUSH_ENTITY& pushentity, const CellContent& content) const;
    void UnmarkPushEntity(BOXPUSHING_STATE& bpstate, const PUSH_ENTITY& pushentity) const;
    void GeneratePreferredAgentAction(const BOXPUSHING_STATE& bpstate, const int& lastObs, 
				      std::vector<int>& actions, const int& index, const bool& humanDefined) const;
    
    int XSize;
    int YSize;
    int NumSmallBoxes;
    int NumLargeBoxes;
    
    bool RandomiseInitialState;
    bool ResetAtCompletion;
    double ProbLargeAgentBox;
    double ProbObservation;
    
    std::vector< std::vector< std::vector<bool> > > MultiAgentLabels;
    std::vector<double> quantiles;
    
private:

    mutable MEMORY_POOL<BOXPUSHING_STATE> MemoryPool;
};

#endif