// RP_Greedy.cc 

#include "RP_Greedy.hh"

using namespace std;


bool IsFeasibleRouter (const RP_Input& in, RP_Output& out, point r)
{
    return (in.IsDot(r.row, r.col) && (in.RouterPrice() + (ManhattanDistance(r, out.ClosestBackbonePoint(r)) * in.BackbonePrice()) <= out.RemainingBudget()));
}


point SelectRandomRouter(const RP_Input& in, RP_Output& out)
{
    point r; 

    do
    {
        r.row = Random(0, in.Rows()-1);
        r.col = Random(0, in.Columns()-1);
    } while (!IsFeasibleRouter(in, out, r));

    return r;
}


void ConnectRouterWithBackbone(RP_Output& out, const RP_Input& in, point router)
{
    point temp, selected_bb_cell;
    unsigned new_distance;

    // select the backbone point
    selected_bb_cell = out.ClosestBackbonePoint(router);
    // cout << "selectedbbcell: " << selected_bb_cell.row << " " << selected_bb_cell.col << endl;

    unsigned current_distance = ManhattanDistance(selected_bb_cell, router);

    while (current_distance > 0)
    {
        temp = MakeRandomMove(selected_bb_cell);
        new_distance = ManhattanDistance(temp, router);
        if (new_distance < current_distance)
        {
            current_distance = new_distance;
            out.InsertBackboneCell(temp.row, temp.col);
            selected_bb_cell = temp;
        }
    }
}


// move from p to one point of his neighborhood
point MakeRandomMove(point p) 
{  
    point p2;

    p2.row = p.row + Random(-1, 1);
    p2.col = p.col + Random (-1, 1);

    return p2;
}


unsigned PlaceRandomizedRouter(const RP_Input& in, RP_Output& out)
{
    int current_coverage;
    unsigned attempts = 0;
    int max_coverage = 0;
    const unsigned MAX_ATTEMPTS = 10000;
    point selected, max_coverage_found;

    selected = SelectRandomRouter(in, out);
    max_coverage_found.row = selected.row;
    max_coverage_found.col = selected.col;

    while (attempts < MAX_ATTEMPTS)
    {    
        selected = SelectRandomRouter(in, out);

        if (out.RouterScore (selected) > out.RouterScore (max_coverage_found))        
        {
            max_coverage_found.row = selected.row;
            max_coverage_found.col = selected.col;
            max_coverage = current_coverage;
        }
        attempts++;
    }
    out.InsertRouter(max_coverage_found.row, max_coverage_found.col);
    cout << "Covered: " << out.TotalCoveredPoints() << endl;

    ConnectRouterWithBackbone(out, in, max_coverage_found);

    return max_coverage;
}


void GreedyRP(const RP_Input& in, RP_Output& out)
{ 
    unsigned new_coverage;
    point router;

    do
    {    
        if (out.RemainingBudget() >= int(in.RouterPrice()))
        {
            new_coverage = PlaceRandomizedRouter(in, out);
            // cout << "Remaining Budget: "  << out.RemainingBudget() << endl;

            // cout << "New Backbone:" << endl;
            // PrintVec(out.Backbone());
            // // cout << "Remaining Budget: "  << out.RemainingBudget() << endl;
        }
        else
        {
            cout << "No router placed." << endl;
            new_coverage = 0;
        }
    } while(new_coverage > 0);
    // cout << "Covered: " << endl;
    // PrintVec(out.Covered());
    cout << out;
    cout << "Final Score: " << out.ComputeTotalScore() << endl;
    cout << "Remaining budget: " << out.RemainingBudget() << endl;
    cout << "OutTotalCoveredPts: " << out.TotalCoveredPoints() << endl;
    cout << "Covered: " << out.Covered().size() << endl;
}


bool PointIsInVec (point pt, vector<point> vec) 
{
    for (unsigned i = 0; i < vec.size(); i++)
        if (pt.row == vec[i].row && pt.col == vec[i].col)
            return 1;

    return 0;
}


void PrintVec (vector<point> vec) 
{
    for (unsigned i = 0; i < vec.size(); i++)
    {
        cout << vec[i].row << " " << vec[i].col;
        if (i != vec.size()-1)
            cout << " -> ";
    }
        
    cout << endl;
}

