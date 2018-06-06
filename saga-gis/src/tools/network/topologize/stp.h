#ifndef __STP_H__
#define __STP_H__

#include "TLB_Interface.h"
#include <set>
#include <map>
#define INFI (double)1E+38
using namespace std;

#pragma pack(push)
#pragma pack(1)
struct node;
struct link;

struct node
{
	int ID;
	double pv;
    typedef ::link *pl;
    typedef ::link *link;
	bool sign;
	node *pi;
	node *ln;
	node *rn;
	CSG_Shape *pShape;
	friend bool operator<(const node& n1,const node& n2)
	{	return n1.ID < n2.ID;}
};
struct link
{
	int ID;
	int o;
	int ld;
	node *d;
	double dis;
	double flow;
	double dflow;
	double capa;
	double t0;
	link *next;
	CSG_Table_Record *pRecord;
	friend bool operator<(const link& l1,const link& l2)
	{	return l1.ID < l2.ID; }
};
struct dtrip
{
	node *d;
	double amount;
	friend bool operator<(const dtrip& t1, const dtrip& t2)
	{ return t1.d->ID < t2.d->ID; }
};
struct otrip
{
	node *o;
	set<dtrip> d;
	friend bool operator<(const otrip& t1, const otrip& t2)
	{	return t1.o->ID < t2.o->ID; }
};
#pragma pack(pop)

class CNetwork
{
private:
	struct Vertex : TSG_Point
	{
		friend bool operator < (const Vertex& v1, const Vertex& v2)
		{
			if(v1.x<v2.x)
				return true;
			else if(v1.x==v2.x)
				return v1.y<v2.y;
			else
				return false;
		}
		void operator = (const TSG_Point& v)
		{
			x = v.x;
			y = v.y;
		}
	};

	CSG_Shapes_Search *pSearch;
	map<Vertex,int> vertices;
	double m_tolerance;
	node node_Head,node_Tail;
	set<node> set_Node;
	set<link> set_Link;
	set<otrip> trips;

public:
	bool m_lock;
	bool IsSpherical;

	CSG_Shapes *ptmpShpLink;
	CSG_Shapes *ptmpShpNode;
	CNetwork(void)
	{
		pSearch = NULL;
		ptmpShpLink = NULL;
		ptmpShpNode = NULL;
		IsSpherical = false;
		Init();
	}
	virtual ~CNetwork()
	{
		delete pSearch;
		delete ptmpShpNode;
		delete ptmpShpLink;
	}

	int	InsLink(int LinkID,int O,int D,double Impedance,int Direction=1, double capa=INFI, CSG_Table_Record *pL=NULL);
	int DelLink(int LinkID);

	int	GetPrecursor(int CurID);
	int GetPrelink(int CurID);
	double GetPathLength(int CurID);
	double GetPathLength(int StartID, int EndID);
	double GetPathLength(double x1, double y1, double x2, double y2);
	int	GetLinkID(int O, int D);
	node * GetNode(int NodeID);
	link * GetLink(int LinkID);
	int	SaveTopoFile(SG_Char FileName[]);
	int	LoadTopoFile(SG_Char FileName[]);
	
	int SetTrip(int O, int D, double amount);
	int AssignTraffic(double tol_root, double tol_final, int max_iter);
	double GetTraffic(int linkID);

	int ValidateVertices(CSG_Table	*pTable);
	int ValidateTopology(CSG_Table	*pTable);
	int GetNearNodeID(double x, double y, double radius=0, int index=0);
	int AllocNodeID(double x, double y);

	void Init();
	int Init(CSG_Shapes	*pShapes, double tolerance = 0);
	int Init(CSG_Shapes	*pPoints,CSG_Shapes	*pLines);

	int MaxLinkID;
	int MaxNodeID;

	bool Topologize(CSG_Shapes *pInLines, CSG_Shapes *pOutPoints, CSG_Shapes *pOutLines, double tolerance);

	void bracket(double& a, double& b, double c, double& fa, double& fb, double& d, double& fd);
	double toms748_solve(const double& ax, const double& bx, const double& tol, const int& max_iter);
	double bisect(double min, double max, const double& tol, const int& max_iter);
	double Sigma(double _lambda);

private:
	void Insert_A_Link(int lid, int lo, int ld, double ldis, double capa, set<node>& target, CSG_Table_Record *pL=NULL);
};

#endif
