/**
*
* Most optimal graph implementation
*
* O(1) Edge Lookup
* O(1) Insert Edge
* O(1) Insert Vertex
* O(|in|) Get Input Vertices
* O(|out|) Get Output Vertices
* O(|in| + |out|) Get Adjacent Vertices
*
* Edge<Dim> holds a source and destination vertex
* Point<Dim> is a vertex
*/

#ifndef HASH_GRAPH__H
#define HASH_GRAPH__H

#include "point.h"
#include "edge.h"

#include <vector>
#include <unordered_map>

template <int Dim>
class HashList
{
private:
	typedef unsigned long long Code_;
	typedef std::pair<Edge<Dim> *, bool> EdgeInfo_;
	typedef std::unordered_map<Code_, EdgeInfo_ > EdgeData_;
	typedef std::unordered_map<Code_, Code_> Next_;
	typedef std::unordered_map<Point<Dim> *, Code_> Heads_;

	Code_ ptrToCode_(Point<Dim> *x, Point<Dim> *y) const;
	Point<Dim> *codeToDst_(Code_ code) const;
	Point<Dim> *codeToSrc_(Code_ code) const;

	EdgeData_ edgeData_;

	Next_ nextIn_;
	Next_ nextOut_;
	Heads_ headsIn_;
	Heads_ headsOut_;
public:
	HashList(){};
	~HashList(){};

	Edge<Dim> * insertEdge(Point<Dim> * x, Point<Dim> * y);
	void insertVertex(Point<Dim> * v);

	std::vector<Point<Dim> *> getAdjacent(Point<Dim> *v) const;
	std::vector<Edge<Dim> *> getIn(Point<Dim> *v) const;
	std::vector<Edge<Dim> *> getOut(Point<Dim> *v) const;
};

template <int Dim>
Point<Dim> *HashList<Dim>::codeToDst_(Code_ code) const
{
	return  (Point<Dim> *) code;
}

template <int Dim>
Point<Dim> *HashList<Dim>::codeToSrc_(Code_ code) const
{
	return (Point<Dim> *) (code >> (sizeof(long) * 8));
}

template <int Dim>
typename HashList<Dim>::Code_ HashList<Dim>::ptrToCode_(Point<Dim> *src, Point<Dim> *dst) const
{
	return (Code_(src) << (sizeof(long) * 8)) | Code_(dst);
}

template <int Dim>
void HashList<Dim>::insertVertex(Point<Dim> *v)
{
	headsIn_[v] = 0;
	headsOut_[v] = 0;
}

template <int Dim>
Edge<Dim> * HashList<Dim>::insertEdge(Point<Dim> *src, Point<Dim> *dst)
{
	Edge<Dim> * ret;
	
	Code_ src_code = ptrToCode_(src, dst);
	Code_ dst_code = ptrToCode_(dst, src);

	EdgeData_::iterator edge_it;
	if ((edge_it = edgeData_.find(src_code)) == edgeData_.end())
	{
		ret = new Edge<Dim>(src, dst);
		edgeData_[src_code] = EdgeInfo_(ret, true);
	}
	else
	{
		ret = (edge_it->second).first;
		if ((edge_it->second).second == false)
			(edge_it->second).second = true;
		else
		{
			ret->weight ++;
			return ret;
		}
	}
	
	if (edgeData_.find(dst_code) == edgeData_.end())
		edgeData_[dst_code] = EdgeInfo_(new Edge<Dim>(dst, src), false);

	nextOut_[src_code] = headsOut_[src];
	headsOut_[src] = src_code;

	nextIn_[src_code] = headsIn_[dst];
	headsIn_[dst] = src_code;

	return ret;
}

template <int Dim>
std::vector<Point<Dim>* > HashList<Dim>::getAdjacent(Point<Dim> *v) const
{
	std::vector<Point<Dim> *> ret;
	std::unordered_map<Point<Dim> *, Code_>::iterator it = heads_.find(v);
	if (it == heads_.end())
		return ret;
	Code_ head = it->second;
	while(head != 0)
	{
		ret.push_back(codeToDst_(head));
		head = nextIn_[head];
	}

	head = it->second;
	while(head != 0)
	{
		ret.push_back(codeToSrc_(head));
		head = nextOut_[head];
	}

	return ret;
}

template <int Dim>
std::vector<Edge<Dim>* > HashList<Dim>::getIn(Point<Dim> *v) const
{
	std::vector<Edge<Dim> *> ret;
	std::unordered_map<Point<Dim> *, Code_>::const_iterator it = headsIn_.find(v);
	if (it == headsIn_.end())
		return ret;

	Code_ head = it->second;
	std::unordered_map<Code_, Code_>::const_iterator head_it = nextIn_.find(head);
	while( (head_it = nextIn_.find(head)) != nextIn_.end() )
	{
		EdgeData_::const_iterator edge_it = edgeData_.find(head);
		ret.push_back((edge_it->second).first);
		head = head_it->second;
	}
	
	return ret;
}


template <int Dim>
std::vector<Edge<Dim>* > HashList<Dim>::getOut(Point<Dim> *v) const
{
	std::vector<Edge<Dim> *> ret;
	std::unordered_map<Point<Dim> *, Code_>::const_iterator it = headsOut_.find(v);
	if (it == headsOut_.end())
		return ret;

	Code_ head = it->second;
	std::unordered_map<Code_, Code_>::const_iterator head_it = nextIn_.find(head);
	while( (head_it = nextOut_.find(head)) != nextOut_.end() )
	{
		EdgeData_::const_iterator edge_it = edgeData_.find(head);
		ret.push_back((edge_it->second).first);
		head = head_it->second;
	}
	
	return ret;
}

#endif /* HASH_GRAPH__H */
