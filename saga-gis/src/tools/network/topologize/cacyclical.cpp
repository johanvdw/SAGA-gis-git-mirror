#include "cacyclical.h"
#include <map>
#include <algorithm>
#include <list>

CAcyclical::CAcyclical(void)
{
    Set_Name(_TL("DAG Polylines"));

    Set_Author(_TL("Copyrights (c) 2018 by Johan Van de Wauw"));

    Set_Description(_TL(
        "DAG Polylines.")
    );

    //----------------------------------------------------

    Parameters.Add_Shapes(
        NULL, "INPUTLINES"	, _TL("Input Topology"),
        _TL(""),
        PARAMETER_INPUT,SHAPE_TYPE_Line
    );

    Parameters.Add_Table(
        NULL, "UPSTREAM_EDGES", _TL("upstream edges"),
        "",
        PARAMETER_OUTPUT);

}

CAcyclical::~CAcyclical(void)
{}

bool CAcyclical::On_Execute(void)
{
    std::map<int, node> node_links;
    auto pInLines	= Parameters("INPUTLINES")->asShapes();
    auto pUpstream_Edges	= Parameters("UPSTREAM_EDGES")->asTable();

    pUpstream_Edges->Add_Field("edge", SG_DATATYPE_Int);
    pUpstream_Edges->Add_Field("upstream", SG_DATATYPE_Int);
    pUpstream_Edges->Add_Field("contribution", SG_DATATYPE_Double);

    int start_field = pInLines->Get_Field("start_id");
    int end_field = pInLines->Get_Field("end_id");
    // loop over segments --> create nodes + link these to segments
    for (int iLine = 0; iLine < pInLines->Get_Count() && SG_UI_Process_Set_Progress(iLine, pInLines->Get_Count()); iLine++)
    {
        CSG_Shape * pLine = pInLines->Get_Shape(iLine);
        int start_id = pLine->Get_Value(start_field)->asInt();
        int end_id = pLine->Get_Value(end_field)->asInt();
        node_links[start_id].to.push_back(iLine);
        node_links[end_id].from.push_back(iLine);
        node_links[end_id].finished.push_back(false);
    }

    std::list<int> todo;

    for (auto it=node_links.begin(); it != node_links.end(); it++)
    {
        // loop over nodes without end_id --> this are starting points.
        if (it->second.from.size() == 0)
        {
            it->second.order = 0;
            todo.push_back(it->first);
        }
    }

    while (todo.size() >0)
        for (auto it=todo.begin(); it!=todo.end();)
        {
            node *j = &node_links[*it];
            node * prev_j = j;

            // track these nodes downstream if all upstream links have been evaluated
              // pseudocode= if not all finished: it++
              for(int i=0; i < j->finished.size(); i++){
                  if (!j->finished[i])
                  {
                     it++;
                     break;
                  }
              }


            int orig_edge = *it;
            while (j->to.size()>0)
            {
                CSG_Shape* edge = pInLines->Get_Shape(j->to.front());
                int end_node_id = edge->Get_Value(end_field)->asInt();

                /*if(std::find(j.upstream.begin(), j.upstream.end(), end_node_id) != j.upstream.end()) {
                    break; // don't follow if we hit a circle
                }*/


                j = &node_links[end_node_id];

                // kopieer bovenliggende punten
                j->upstream.insert(j->upstream.end(), prev_j->upstream.begin(), prev_j->upstream.end());

                // en het punt zelf
                j->upstream.push_back(orig_edge);

                // indien eind van enkelvoudig stuk: toevoegen aan todo list
                if (todo.end()!=it && j->from.size()>1)
                {
                    for(int i=0; i < j->from.size(); i++){
                       j->finished[i]=true;
                    }

                    if (std::find(it, todo.end(), orig_edge) != todo.end())
                    {
                        todo.push_back(end_node_id);
                    }

                    break;
                }


                prev_j = j;

                orig_edge = end_node_id;

            }
            it = todo.erase(it);
        }

    // convert results to a nice table

    for (auto it=node_links.begin(); it != node_links.end(); it++)
    {

        for (auto up=it->second.upstream.begin(); up != it->second.upstream.end(); up++)
        {
            auto *pRecord = pUpstream_Edges->Add_Record();
            pRecord->Set_Value("edge", it->first);
            pRecord->Set_Value("upstream", *up);
        }

    }

    return true;
}
